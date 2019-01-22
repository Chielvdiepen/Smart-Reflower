#include "board.h"
#include "board_GPIO_ID.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <chip.h>
#include <lpc_tools/clock.h>
#include <lpc_tools/boardconfig.h>
#include <lpc_tools/GPIO_HAL.h>
#include <lpc_tools/GPIO_HAL_LPC.h>
#include <mcu_timing/delay.h>

// max31855 includes
#include "MAX31855.h"

// LCD includes
#include "i2c.h"
#include "LCD.h"
#include "Visual_LCD.h"

//PWM includes
#include <c_utils/assert.h>
#include <c_utils/max.h>
#include "Buzzer_Motor.h"

// USB include
#include "usb_init.h"
#include "usb/cdc_vcom.h"

//global variable
uint64_t start_timestamp;

void Timer_init(void)
{
    start_timestamp = delay_get_timestamp();
}

int Cur_Time_s()
{
    int tijd_s = (delay_calc_time_us(start_timestamp, delay_get_timestamp())) / 1000000;
    return tijd_s;
}

int Cur_Time_ms()
{
    int tijd_ms = (delay_calc_time_us(start_timestamp, delay_get_timestamp())) / 1000000;
    return tijd_ms;
}