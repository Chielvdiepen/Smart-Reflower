#ifndef Visual_LCD_H
#define Visual_LCD_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <chip.h>
#include <lpc_tools/GPIO_HAL.h>
#include "board.h"
#include "board_GPIO_ID.h"
#include "LCD.h"
#include <mcu_timing/delay.h>
#include "Buzzer_Motor.h"

// state data struct, filled with init
struct state
{
    char state_name[14];
    uint64_t begin_time;
    uint64_t duration;
    uint64_t end_time;
    uint64_t step_time;
    int state_temp;
};

void state_init(struct state *fase, uint64_t duration_s, uint64_t def_start_time, const char *name_state, int state_temp);
void progressbar_run(struct state *fase, int alarm_tune, uint64_t cur_time);
void state_display(struct state *fase, int *perc_progress);
char* relais_state(char *Relais_status);

#endif