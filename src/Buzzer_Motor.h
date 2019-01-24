#ifndef Buzzer_Motor_H
#define Buzzer_Motor_H

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

// buzzer functions
void Buzzer_init(unsigned int bzzr_frequency);
void Buzzer_tone(unsigned int bzfrequentie, uint32_t tone_duration_ms);
void Buzzer_alarm(int keuze);

// DCmotor functions
void DCmotor_init(unsigned int dcmotor_frequency);
void DCmotor_drive(unsigned int dcfrequentie, uint32_t drive_duration_ms);

// end of class definition
#endif