#include "board.h"
#include "board_GPIO_ID.h"
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

//PWM includes
#include "PWM.h"
#include <c_utils/assert.h>
#include <c_utils/max.h>

//Songs includes
#include "Buzzer_songs.h"

// buzzer config
PWM bzzr;
const uint32_t bzzr_req_resolution = 100;
//const uint32_t bzzr_frequency = 1.5 * 1000;

// dcmotor config
PWM dcmotor;
const uint32_t dcmotor_req_resolution = 1000;
//const uint32_t dcmotor_frequency = 2.4 * 1000;

void Buzzer_init(unsigned int bzzr_frequency)
{
    // init PWM: timer, freq, res, checks with assert
    assert(PWM_init(&bzzr, LPC_TIMER16_1, PWM_CH1, bzzr_frequency, bzzr_req_resolution));
    // start PWM: checks with assert
    assert(PWM_start(&bzzr));

    // Requests the PWM of automated resolution value
    // const uint32_t maximum = PWM_get_resolution(&bzzr);
}

void Buzzer_tone(unsigned int bzfrequentie, uint32_t tone_duration_ms)
{
    Buzzer_init(bzfrequentie);

    // Assigning PWM resolution, 0 = off
    assert(PWM_set(&bzzr, PWM_CH1, 50));
    delay_us(tone_duration_ms * 1000);
    assert(PWM_set(&bzzr, PWM_CH1, 0));
}

void Buzzer_alarm(int keuze)
{
    if (keuze == 0)
    {
        Alarm();
    }
    else if (keuze == 1)
    {
        starwars();
    }
    else if (keuze == 2)
    {
        GameOfThrones();
    }
    else if (keuze == 3)
    {
        HarryPotter();
    }
    else if (keuze == 4)
    {
        Pirate();
    }
    else if (keuze == 5)
    {
        mario();
    }
    else if (keuze == 6)
    {
        IndianaJones();
    }
    else if (keuze == 7)
    {
        twentyCenFox();
    }
}

void DCmotor_init(unsigned int dcmotor_frequency)
{
    // init PWM: timer, freq, res, checks with assert
    assert(PWM_init(&dcmotor, LPC_TIMER32_0, PWM_CH2, dcmotor_frequency, dcmotor_req_resolution));
    // start PWM: checks with assert
    assert(PWM_start(&dcmotor));

    // Requests the PWM of automated resolution value
    // const uint32_t maximum = PWM_get_resolution(&DCmotor);

    // Assigning PWM resolution, 0 = off
    assert(PWM_set(&dcmotor, PWM_CH1, 0));
}

void DCmotor_drive(unsigned int dcfrequentie, uint32_t drive_duration_ms)
{
    DCmotor_init(dcfrequentie);

    // Assigning PWM resolution, 0 = off
    assert(PWM_set(&bzzr, PWM_CH1, 500));
    delay_us(drive_duration_ms * 1000);
    assert(PWM_set(&bzzr, PWM_CH1, 0));
}