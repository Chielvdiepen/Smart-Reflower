#include "MAX31855.h"
#include <lpc_tools/GPIO_HAL.h>
#include <lpc_tools/boardconfig.h>
#include <stdio.h>

#include <string.h>
#include "usb/cdc_vcom.h"

// SPI 0,0 mode: clock idles in low state
#define SPI_MODE (SSP_CLOCK_MODE0)

// Frequency: what is the maximum possible freq? MAX31855 fscl = max 5 MHz
#define SPI_BITRATE (5000000)

// Alias functies
static void SPI1_transfer_begin(void)
{
	GPIO_HAL_set(board_get_GPIO(GPIO_ID_CS1), LOW);
}
static void SPI1_transfer_end(void)
{
	GPIO_HAL_set(board_get_GPIO(GPIO_ID_CS1), HIGH);
}
static void SPI2_transfer_begin(void)
{
	GPIO_HAL_set(board_get_GPIO(GPIO_ID_CS2), LOW);
}
static void SPI2_transfer_end(void)
{
	GPIO_HAL_set(board_get_GPIO(GPIO_ID_CS2), HIGH);
}

static size_t SPI_read_blocking(uint8_t *buffer, size_t sizeof_buffer)
{
	return Chip_SSP_ReadFrames_Blocking(LPC_SSP0, buffer, sizeof_buffer);
}

// union struct met memory map voor de 32 bits message
struct memory_map
{
	signed int Temp_ext;
	signed int Temp_int; // float maken voor de berekening, moet nog wel gekeken worden naar printfunctie, float to string (lpc tools)
	unsigned int SCV;
	unsigned int SCG;
	unsigned int OC;
};

// initialisatie van SPI
void SPI_init(void)
{
	static SSP_ConfigFormat ssp_format;
	Chip_SSP_Init(LPC_SSP0);
	ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
	ssp_format.bits = SSP_BITS_8;
	ssp_format.clockMode = SPI_MODE;
	Chip_SSP_SetFormat(LPC_SSP0, ssp_format.bits, ssp_format.frameFormat, ssp_format.clockMode);
	Chip_SSP_SetMaster(LPC_SSP0, true);
	Chip_SSP_SetBitRate(LPC_SSP0, SPI_BITRATE);
	Chip_SSP_Enable(LPC_SSP0);
}

struct memory_map X, Y;

// main
int Meet_Temp(void)
{
	uint8_t buffer[4];
	char waardenX[100], waardenY[100];
	SPI_init();

	// Read out of SPI1, gives temp int,ext and flags
	SPI1_transfer_begin();
	SPI_read_blocking(buffer, sizeof(buffer));
	SPI1_transfer_end();

	X.Temp_ext = (buffer[0] << 6) | (buffer[1] >> 2);
	X.Temp_int = (buffer[2] << 4) | (buffer[3] >> 4);
	X.SCV = buffer[3] & (1 << 2);
	X.SCG = buffer[3] & (1 << 1);
	X.OC = buffer[3] & (1 << 0);

	// Read out of SPI1, gives temp int,ext and flags
	SPI2_transfer_begin();
	SPI_read_blocking(buffer, sizeof(buffer));
	SPI2_transfer_end();

	Y.Temp_ext = (buffer[0] << 6) | (buffer[1] >> 2);
	Y.Temp_int = (buffer[2] << 4) | (buffer[3] >> 4);
	Y.SCV = buffer[3] & (1 << 2);
	Y.SCG = buffer[3] & (1 << 1);
	Y.OC = buffer[3] & (1 << 0);

	//temp conversie
	// signed int Temp_int1 = X.Temp_int / 16;
	signed int Temp_Ext1 = X.Temp_ext / 4;
	// signed int Temp_int2 = Y.Temp_int / 16;
	signed int Temp_Ext2 = Y.Temp_ext / 4;

	signed int Temp_out = (Temp_Ext1 + Temp_Ext2) / 2;

	//Print data over VCOM
	snprintf(waardenX, sizeof(waardenX), "Temp-ext1 = %d\r\n", Temp_Ext1);
	vcom_write((uint8_t *)waardenX, strlen(waardenX));

	snprintf(waardenY, sizeof(waardenY), "Temp-ext2 = %d\r\n", Temp_Ext2);
	vcom_write((uint8_t *)waardenY, strlen(waardenY));

	const GPIO *LED_Y = board_get_GPIO(GPIO_ID_LED_Y);
	if ((X.OC == 0) || (Y.OC == 0))
	{
		GPIO_HAL_set(LED_Y, HIGH);
		return Temp_out;
	}
	else
	{
		GPIO_HAL_set(LED_Y, LOW);	
		return 0;
	}
}

void *_sbrk(int incr)
{
	void *st = 0;
	return st;
}