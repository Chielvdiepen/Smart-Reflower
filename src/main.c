#include "board.h"
#include "board_GPIO_ID.h"
#include <string.h>

#include <chip.h>
#include <lpc_tools/clock.h>
#include <lpc_tools/boardconfig.h>
#include <lpc_tools/GPIO_HAL.h>
#include <lpc_tools/GPIO_HAL_LPC.h>
#include <mcu_timing/delay.h>

// max31855 includes
#include "MAX31855.h"

// USB include
#include "usb_init.h"
#include "usb/cdc_vcom.h"

#define CLK_FREQ (48e6)

int main(void)
{
	board_setup();
	board_setup_NVIC();
	board_setup_pins();

	// configure System Clock at 48MHz
	clock_set_frequency(CLK_FREQ);
	USBclockinit();
	SystemCoreClockUpdate();

	delay_init();
	usb_init();

	// get the GPIO's
	const GPIO *led_STATUS = board_get_GPIO(GPIO_ID_LED_STATUS);
	const GPIO *led_R = board_get_GPIO(GPIO_ID_LED_R);
	const GPIO *led_Y = board_get_GPIO(GPIO_ID_LED_Y);
	const GPIO *led_G = board_get_GPIO(GPIO_ID_LED_G);
	const GPIO *pwm_RELAIS = board_get_GPIO(GPIO_ID_PWM_RELAIS);
	const GPIO *buzzer = board_get_GPIO(GPIO_ID_BUZZER);
	const GPIO *CS1 = board_get_GPIO(GPIO_ID_CS1);
	const GPIO *CS2 = board_get_GPIO(GPIO_ID_CS2);

	int prompt = 0;

	while (true)
	{
		if ((vcom_connected() != 0) && (prompt == 0))
		{
			vcom_write("Jitter Smart-Reflower VCOM.\r\n", 29);
			prompt = 1;
		}
		if (prompt)
		{
			GeefTemp();
			delay_us(5E6);
		}
	}
	return 0;
}
