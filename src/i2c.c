#include <stdlib.h>
#include "board.h"
#include <chip.h>

// Initialize the I2C bus
static void i2c_app_init()
{
	Chip_SYSCTL_PeriphReset(RESET_I2C0);

	// Initialize I2C
	Chip_I2C_Init(I2C0);
	Chip_I2C_SetClockRate(I2C0, 100000); //100kHz of 400kHz
	Chip_I2C_SetMasterEventHandler(I2C0, Chip_I2C_EventHandlerPolling);
}

int SlaveAddr = 0x3F;

void i2c_init (void)
{
	i2c_app_init();
}

void i2c_read (uint8_t* rxBuff, int rxSz)
{
	Chip_I2C_MasterRead(I2C0,SlaveAddr,rxBuff,rxSz);
}

void i2c_send (uint8_t* txBuff, int txSz)
{
	Chip_I2C_MasterSend(I2C0,SlaveAddr,txBuff,txSz);
}