#ifndef Visual_LCD_H
#define Visual_LCD_H

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

// state data struct, filled with init
struct state
{
    char state_name[14];
    int duration;
    int begin_temp;
    int step_time;
    int state_temp;
};

void state_init(struct state *fase, int duration_s, int begin, const char *name_state, int state_temp);
void progressbar_run(struct state *fase);
void progressbar_run_total(struct state *fase);
void state_display(struct state *fase, int *perc_progress);
char *relais_state(char *Relais_status);
void Start_Screen(void);
void End_Screen(void);

#endif