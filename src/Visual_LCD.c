#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <chip.h>
#include <lpc_tools/GPIO_HAL.h>
#include "board.h"
#include "board_GPIO_ID.h"
#include "LCD.h"
#include "Buzzer_Motor.h"
#include <mcu_timing/delay.h>
#include <stdio.h>
#include <string.h>
#include "Visual_LCD.h"
#include "MAX31855.h"

static const uint8_t block_char = 255; // block character int block_char = 255;

void state_init(struct state *fase, uint64_t duration_s, uint64_t def_start_time, const char *name_state, int temp_state)
{
    // save name in struct for printing on LCD
    strcpy(fase->state_name, name_state);
    // save duration to struct
    fase->duration = (duration_s * 1000000);
    //set the begin time stamp
    fase->begin_time = (def_start_time * 1000000);
    //calc end time stamp
    fase->end_time = (def_start_time * 1000000) + (duration_s * 1000000);
    //calc time per step
    fase->step_time = (duration_s * 1000000) / 20;
    // save state_temp in struct
    fase->state_temp = temp_state;
}

void progressbar_run(struct state *fase, int alarm_tune, uint64_t cur_time)
{
    int block_count = ((cur_time * 1000000) - (fase->begin_time)) / ((fase->step_time));

    state_display(fase, &block_count);
    LCD_setCursor(0, 2);

    for (int i = 0; i < block_count; i++)
    {
        LCD_write(block_char);
    }
    if (block_count == 20)
    {
        if (strcmp(fase->state_name, "Cooldown fase") == 0 || strcmp(fase->state_name, "Totaal fase") == 0)
        {
            Buzzer_alarm(alarm_tune);
        }
    }
}

void state_display(struct state *fase, int *perc_progress)
{
    char buffer[100];
    int progress = (*perc_progress * 100) / 20;

    LCD_setCursor(0, 0);
    snprintf(buffer, sizeof(buffer), "%13s Rl:%3s", fase->state_name, "aan"); //14-20 vrij
    LCD_write_string(buffer);

    LCD_setCursor(0, 1);
    snprintf(buffer, sizeof(buffer), "       %3d%s     ", progress, "%"); // percentage progress berekening
    LCD_write_string(buffer);
}