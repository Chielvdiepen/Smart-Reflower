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

static const uint8_t block_char = 255; // block character int block_char = 255;

void state_init(struct state *fase, uint64_t duration_s, uint64_t def_start_time, const char *name_state, int temp_state)
{
    // save name in struct for printing on LCD
    strcpy(fase->state_name, name_state);
    // save duration to struct
    fase->duration = (duration_s);
    //set the begin time stamp
    fase->begin_time = (def_start_time);
    //calc end time stamp
    fase->end_time = (def_start_time) + (duration_s);
    //calc time per step
    fase->step_time = (duration_s) / 20;
    // save state_temp in struct
    fase->state_temp = temp_state;
}

void progressbar_run(struct state *fase, int alarm_tune)
{
    int block_count = ((Cur_Time()) - (fase->begin_time)) / ((fase->step_time));

    if (block_count > 20)
    {
        if (strcmp(fase->state_name, "Cooldown fase") == 0 || strcmp(fase->state_name, "Totaal fase") == 0)
        {
            Buzzer_alarm(alarm_tune);
        }
    }

    state_display(fase, &block_count);
}

void state_display(struct state *fase, int *perc_progress)
{
    char buffer[100];
    char Relais_status[4];
    int progress;

    if (*perc_progress < 21)
    {
        progress = (*perc_progress * 100) / 20;
    }
    else
    {
        progress = 100;
    }

    LCD_setCursor(0, 0);
    snprintf(buffer, sizeof(buffer), "%-13s Rl:%3s", fase->state_name, relais_state(Relais_status)); //14-20 vrij
    LCD_write_string(buffer);

    LCD_setCursor(0, 1);
    snprintf(buffer, sizeof(buffer), "       %3d%s     ", progress, "%"); // percentage progress berekening
    LCD_write_string(buffer);

    LCD_setCursor(0, 2);
    for (int i = 0; i < progress/5; i++)
    {
        LCD_write(block_char);
    }

    LCD_setCursor(0, 3);
    snprintf(buffer, sizeof(buffer), "Tijd %.2d:%.2d Temp:%3dC", Cur_Time() / 60, Cur_Time() % 60, Meet_Temp()); // percentage progress berekening
    LCD_write_string(buffer);
}

char *relais_state(char *Relais_status)
{
    const GPIO *pwm_RELAIS = board_get_GPIO(GPIO_ID_PWM_RELAIS);

    int check = GPIO_HAL_get(pwm_RELAIS);

    if (check == 1)
    {
        strcpy(Relais_status, "Aan");
    }
    else
    {
        strcpy(Relais_status, "Uit");
    };

    return Relais_status;
}
