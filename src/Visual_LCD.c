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

void state_init(struct state *fase, int duration_s, int begin, const char *name_state, int temp_state)
{
    // save name in struct for printing on LCD
    strcpy(fase->state_name, name_state);
    // save begin temp of state
    fase->begin_temp = begin;
    // save duration to struct
    fase->duration = (duration_s);
    //calc time per step
    fase->step_time = (duration_s) / 20;
    // save state_temp in struct
    fase->state_temp = temp_state;
}

void progressbar_run(struct state *fase)
{
    int block_count = ((State_cur_time_s()) / (fase->step_time));

    state_display(fase, &block_count);
}

void progressbar_run_total(struct state *fase)
{
    int block_count = ((Total_cur_time_s()) / (fase->step_time));

    state_display(fase, &block_count);
}

void state_display(struct state *fase, int *perc_progress)
{
    char buffer[100];
    char spaties[21] = "                    ";
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

    int aantal_block = progress / 5;
    int aantal_spaties = (20 - aantal_block);

    LCD_setCursor(0, 0);
    snprintf(buffer, sizeof(buffer), "%-13s RL:%3s", fase->state_name, relais_state(Relais_status)); //14-20 vrij
    LCD_write_string(buffer);

    LCD_setCursor(0, 1);
    snprintf(buffer, sizeof(buffer), "       %3d%s         ", progress, "%"); // percentage progress berekening
    LCD_write_string(buffer);

    LCD_setCursor(0, 2);
    for (int i = 0; i < aantal_block; i++)
    {
        LCD_write(block_char);
    }
    for (int i = 0; i < aantal_spaties; i++)
    {
        LCD_write(spaties[i]);
    }

    LCD_setCursor(0, 3);
    snprintf(buffer, sizeof(buffer), "Tijd %.2d:%.2d Temp:%3dC", Total_cur_time_s() / 60, Total_cur_time_s() % 60, Meet_Temp()); // percentage progress berekening
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

void Start_Screen(void)
{
    char buffer1[100];
    snprintf(buffer1, sizeof(buffer1), "  Jitter   Temp:%3dC", Meet_Temp()); // percentage progress berekening
    LCD_write_string(buffer1);
    LCD_write_string("   Smart-Reflower   ");
    LCD_write_string("  Press the Button  ");
    LCD_write_string("To Begin The Process");
}

void End_Screen(void)
{
    LCD_write_string("Process is Finished!");
    LCD_write_string("     Wait 30s to    ");
    LCD_write_string("  Return to start   ");
    LCD_write_string("       Jitter       ");
}