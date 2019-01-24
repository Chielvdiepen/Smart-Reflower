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
uint64_t total_start_timestamp;
uint64_t state_start_timestamp;

void Total_timer_init(void)
{
    total_start_timestamp = delay_get_timestamp();
}

int Total_cur_time_s()
{
    int tijd_s = (delay_calc_time_us(total_start_timestamp, delay_get_timestamp())) / 1000000;
    return tijd_s;
}

void State_timer_init(void)
{
    state_start_timestamp = delay_get_timestamp();
}

int State_cur_time_s()
{
    int time_s = (delay_calc_time_us(state_start_timestamp, delay_get_timestamp())) / 1000000;
    return time_s;
}