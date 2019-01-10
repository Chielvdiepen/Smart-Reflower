#include <string.h>
#include "usb/app_usbd_cfg.h"
#include "usb/cdc_vcom.h"
#include <chip.h>

static USBD_HANDLE_T g_hUsb;
USBD_API_T *gUSB_API;

void USBclockinit(void)
{
	/* Set USB PLL input to main oscillator */
	Chip_Clock_SetUSBPLLSource(SYSCTL_PLLCLKSRC_MAINOSC);
	/* Setup USB PLL  (FCLKIN = 12MHz) * 4 = 48MHz
	   MSEL = 3 (this is pre-decremented), PSEL = 1 (for P = 2)
	   FCLKOUT = FCLKIN * (MSEL + 1) = 12MHz * 4 = 48MHz
	   FCCO = FCLKOUT * 2 * P = 48MHz * 2 * 2 = 192MHz (within FCCO range) */
	Chip_Clock_SetupUSBPLL(3, 1);

	/* Powerup USB PLL */
	Chip_SYSCTL_PowerUp(SYSCTL_POWERDOWN_USBPLL_PD);

	/* Wait for PLL to lock */
	while (!Chip_Clock_IsUSBPLLLocked())
	{
	}
}

/* Initialize pin and clocks for USB0/USB1 port */
static void usb_pin_clk_init(void)
{
    /* enable USB main clock */
    Chip_Clock_SetUSBClockSource(SYSCTL_USBCLKSRC_PLLOUT, 1);
    /* Enable AHB clock to the USB block and USB RAM. */
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_USB);
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_USBRAM);
    /* power UP USB Phy */
    Chip_SYSCTL_PowerUp(SYSCTL_POWERDOWN_USBPAD_PD);
}
void USB_IRQHandler(void)
{
    uint32_t *addr = (uint32_t *)LPC_USB->EPLISTSTART;
    // WORKAROUND for artf32289 ROM driver BUG:
    // As part of USB specification the device should respond
    // with STALL condition for any unsupported setup packet. The host will send
    // new setup packet/request on seeing STALL condition for EP0 instead of sending
    // a clear STALL request. Current driver in ROM doesn't clear the STALL
    // condition on new setup packet which should be fixed.

    if (LPC_USB->DEVCMDSTAT & _BIT(8))
    {                           // if setup packet is received
        addr[0] &= ~(_BIT(29)); // clear EP0_OUT stall
        addr[2] &= ~(_BIT(29)); // clear EP0_IN stall
    }

    gUSB_API->hw->ISR(g_hUsb);
}

// Workaround for USB_ROM.1 bug
typedef volatile struct _EP_LIST
{
    uint32_t buf_ptr;
    uint32_t buf_length;
} EP_LIST;
ErrorCode_t workaround_stall(USBD_HANDLE_T hUsb)
{
    ErrorCode_t ret = LPC_OK;
    USB_CORE_CTRL_T *pCtrl = (USB_CORE_CTRL_T *)hUsb;
    EP_LIST *epQueue;
    int32_t i;
    /*    WORKAROUND for Case 2:
          Code clearing STALL bits in endpoint reset routine corrupts memory area
          next to the endpoint control data.
          */
    if (pCtrl->ep_halt != 0)
    { /* check if STALL is set for any endpoint */
        /* get pointer to HW EP queue */
        epQueue = (EP_LIST *)LPC_USB->EPLISTSTART;
        /* check if the HW STALL bit for the endpoint is cleared due to bug. */
        for (i = 1; i < pCtrl->max_num_ep; i++)
        {
            /* check OUT EPs */
            if (pCtrl->ep_halt & (1 << i))
            {
                /* Check if HW EP queue also has STALL bit = _BIT(29) is set */
                if ((epQueue[i << 1].buf_ptr & _BIT(29)) == 0)
                {
                    /* bit not set, cleared by BUG. So set it back. */
                    epQueue[i << 1].buf_ptr |= _BIT(29);
                }
            }
            /* Check IN EPs */
            if (pCtrl->ep_halt & (1 << (i + 16)))
            {
                /* Check if HW EP queue also has STALL bit = _BIT(29) is set */
                if ((epQueue[(i << 1) + 1].buf_ptr & _BIT(29)) == 0)
                {
                    /* bit not set, cleared by BUG. So set it back. */
                    epQueue[(i << 1) + 1].buf_ptr |= _BIT(29);
                }
            }
        }
    }
    return ret;
}

/* Find the address of interface descriptor for given class type. */
USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc, uint32_t intfClass)
{
    USB_COMMON_DESCRIPTOR *pD;
    USB_INTERFACE_DESCRIPTOR *pIntfDesc = 0;
    uint32_t next_desc_adr;

    pD = (USB_COMMON_DESCRIPTOR *)pDesc;
    next_desc_adr = (uint32_t)pDesc;

    while (pD->bLength)
    {
        /* is it interface descriptor */
        if (pD->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE)
        {

            pIntfDesc = (USB_INTERFACE_DESCRIPTOR *)pD;
            /* did we find the right interface descriptor */
            if (pIntfDesc->bInterfaceClass == intfClass)
            {
                break;
            }
        }
        pIntfDesc = 0;
        next_desc_adr = (uint32_t)pD + pD->bLength;
        pD = (USB_COMMON_DESCRIPTOR *)next_desc_adr;
    }

    return pIntfDesc;
}

void usb_init(void)
{
    // USB init
    gUSB_API = (USBD_API_T *)LPC_ROM_API->usbdApiBase; //0x1FFF1F24

    USBD_API_INIT_PARAM_T usb_param;
    USB_CORE_DESCS_T desc;
    ErrorCode_t ret = LPC_OK;

    /* enable clocks and pinmux */
    usb_pin_clk_init();

    /* initilize call back structures */
    memset((void *)&usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
    usb_param.usb_reg_base = LPC_USB0_BASE;

    // See errata USB_ROM.1: max_num_ep should be at least n+1, where n
    // is the amount of endpoints in use (ep1_in + ep1_out = 2).
    usb_param.max_num_ep = 3; // TODO n+1?
    usb_param.mem_base = USB_STACK_MEM_BASE;
    usb_param.mem_size = USB_STACK_MEM_SIZE;

    usb_param.USB_Interface_Event = workaround_stall; // See errata USB_ROM.1

    /* Set the USB descriptors */
    desc.device_desc = (uint8_t *)&USB_DeviceDescriptor[0];
    desc.string_desc = (uint8_t *)&USB_StringDescriptor[0];
    /* Note, to pass USBCV test full-speed only devices should have both
	   descriptor arrays point to same location and device_qualifier set to 0.
	 */
    desc.high_speed_desc = (uint8_t *)&USB_FsConfigDescriptor[0];
    desc.full_speed_desc = (uint8_t *)&USB_FsConfigDescriptor[0];
    desc.device_qualifier = 0;

    /* USB Initialization */
    ret = gUSB_API->hw->Init(&g_hUsb, &desc, &usb_param);
    if (ret == LPC_OK)
    {

        /* Init VCOM interface */
        ret = vcom_init(g_hUsb, &desc, &usb_param);
        if (ret == LPC_OK)
        {
            /*  enable USB interrrupts */
            NVIC_EnableIRQ(USB0_IRQn);
            /* now connect */
            gUSB_API->hw->Connect(g_hUsb, 1);
        }
    }
}