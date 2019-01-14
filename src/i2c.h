#ifndef i2c_H
#define i2c_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <chip.h>
#include <lpc_tools/GPIO_HAL.h>
#include "board.h"
#include "board_GPIO_ID.h"

void i2c_init (void);
void i2c_read (uint8_t* rxBuff,int rxSz);
void i2c_send (uint8_t* txBuff,int txSz);

#endif
