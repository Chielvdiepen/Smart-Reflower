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

// timing includes
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

#define CLK_FREQ (48e6)

// defined start time of the reflow states in seconds
static uint64_t PREHEAT_startS = 0;
static uint64_t SOAK_startS = 80;
static uint64_t REFLOW_startS = 180;
static uint64_t DWELL_startS = 220;
static uint64_t COOLDOWN_startS = 240;
static uint64_t TOTAL_startS = 0;

// defined durations of the reflow states in seconds
static uint64_t PREHEAT_DurS = 80;
static uint64_t SOAK_DurS = 100;
static uint64_t REFLOW_DurS = 40;
static uint64_t DWELL_DurS = 20;
static uint64_t COOLDOWN_DurS = 60;
static uint64_t TOTAL_DurS = 300;

// defined durations of the reflow states in seconds
static int PREHEAT_temp = 150;
static int SOAK_temp = 100;
static int REFLOW_temp = 40;
static int DWELL_temp = 20;
static int COOLDOWN_temp = 60;
static int TOTAL_temp = 0;

static const char state_buffer[6][14] = {"Preheat fase", "Soak fase", "Reflow fase", "Dwell fase", "Cooldown fase", "Totaal fase"};

// state data struct, filled with init
struct state PREHEAT, SOAK, REFLOW, DWELL, COOLDOWN, TOTAL;

int main(void)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*																													*/
	/*											Setup code: initialisation												*/
	/*																													*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	board_setup();
	board_setup_NVIC();
	board_setup_pins();

	// configure System Clock at 48MHz
	clock_set_frequency(CLK_FREQ);
	USBclockinit();
	SystemCoreClockUpdate();

	// start/init delay timers, usb and LCD
	delay_init();
	usb_init();
	LCD_begin();

	// config the state with values
	state_init(&PREHEAT, PREHEAT_DurS, PREHEAT_startS, state_buffer[0], PREHEAT_temp);
	state_init(&SOAK, SOAK_DurS, SOAK_startS, state_buffer[1], SOAK_temp);
	state_init(&REFLOW, REFLOW_DurS, REFLOW_startS, state_buffer[2], REFLOW_temp);
	state_init(&DWELL, DWELL_DurS, DWELL_startS, state_buffer[3], DWELL_temp);
	state_init(&COOLDOWN, COOLDOWN_DurS, COOLDOWN_startS, state_buffer[4], COOLDOWN_temp);
	state_init(&TOTAL, TOTAL_DurS, TOTAL_startS, state_buffer[5], TOTAL_temp);

	const GPIO *pwm_RELAIS = board_get_GPIO(GPIO_ID_PWM_RELAIS);
	const GPIO *LED_R = board_get_GPIO(GPIO_ID_LED_R);
	const GPIO *led_STATUS = board_get_GPIO(GPIO_ID_LED_STATUS);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*																													*/
	/*											Main code: state machine?												*/
	/*																													*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// /////////////////|						 dit is een 4 regel lange bericht	 					  |/////////////////
	// LCD_write_string("");

	// progressbar_run(&PREHEAT, 0);
	// progressbar_run(&SOAK, 0);
	// progressbar_run(&REFLOW, 0;
	// progressbar_run(&DWELL, 0);

	// GPIO_HAL_toggle(LED_R);
	// GPIO_HAL_toggle(pwm_RELAIS);
	// GPIO_HAL_get(LED_R);

	// GPIO_HAL_set(board_get_GPIO(GPIO_ID_LED_G), GPIO_HAL_get(LED_R));

	// for (int i = 240; i < 301; i++)
	// {
	// 	progressbar_run(&COOLDOWN, 5, i);
	// 	delay_us(1000000);
	// 	if (i == 260 || i == 280)
	// 	{
	// 		GPIO_HAL_toggle(LED_R);
	// 		GPIO_HAL_toggle(pwm_RELAIS);
	// 	}
	// }

	// int prompt = 0;

	// while (true)
	// {
	// 	if ((vcom_connected() != 0) && (prompt == 0))
	// 	{
	// 		vcom_write("Jitter Smart-Reflower VCOM.\r\n", 29);
	// 		prompt = 1;
	// 		break;
	// 	}
	// }

	// if (prompt)
	// {
	int aan = 1;
	Timer_init();
	GPIO_HAL_toggle(led_STATUS);
	while (aan)
	{
		if (Cur_Time() > 80)
		{
			aan = 0;
		}
		else
		{
			progressbar_run(&PREHEAT, 0);
		}
	}

	// if (Cur_Time() % 500 == 0)
	// {
	// 	GPIO_HAL_toggle(LED_R);
	// 	// GPIO_HAL_toggle(pwm_RELAIS);
	// }

	// }

	return 0;
}
