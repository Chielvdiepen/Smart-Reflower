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
#include "process_time.h"

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

void Control_Relais(int Temp_Set)
{
    const GPIO *pwm_RELAIS = board_get_GPIO(GPIO_ID_PWM_RELAIS);

    int Cur_Temp = Meet_Temp();

    if (Cur_Temp < Temp_Set)
    {
        GPIO_HAL_set(pwm_RELAIS, HIGH);
    }
    else
    {
        GPIO_HAL_set(pwm_RELAIS, LOW);
    }
}