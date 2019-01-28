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
#include "Relais_control.h"
#include "button.h"

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

// defined durations of the reflow states in seconds
static int PREHEAT_DurS = 180; // 180 sec
static int SOAK_DurS = 120;	// 120 sec
static int REFLOW_DurS = 50;   // 50 sec
static int DWELL_DurS = 40;	// 40 sec
static int COOLDOWN_DurS = 60; // 60 sec
static int END_DurS = 30;
static int TOTAL_DurS = 450;

// defined temperatures of the reflow states in Celsius
static int Begin_temp = 20;
static int PREHEAT_temp = 150;  // 150 C
static int SOAK_temp = 200;		// 200 C
static int REFLOW_temp = 230;   // 230 C
static int DWELL_temp = 255;	// 255 c
static int COOLDOWN_temp = 100; // 100 C
static int TOTAL_temp = 0;

static const char state_buffer[6][14] = {"Preheat fase", "Soak fase", "Reflow fase", "Dwell fase", "Cooldown fase", "Totaal fase"};

// state data struct, filled with init
struct state PREHEAT, SOAK, REFLOW, DWELL, COOLDOWN, TOTAL;

// config of state machine
enum ProfielState
{
	ProfielStateIdle = 0,
	ProfielStatePreheat,
	ProfielStateSoak,
	ProfielStateReflow,
	ProfielStateDwell,
	ProfielStateCooldown,
	ProfielStateEnd
};

volatile enum ProfielState cur_state;

enum ProfielState Profiel_state_idle(void);
enum ProfielState Profiel_state_Preheat(void);
enum ProfielState Profiel_state_Soak(void);
enum ProfielState Profiel_state_Reflow(void);
enum ProfielState Profiel_state_Dwell(void);
enum ProfielState Profiel_state_Cooldown(void);
enum ProfielState Profiel_state_End(void);

int setpoint_calc(struct state *fase);
void Display_switch(struct state *fase);
bool temp_check(struct state *fase);
void app_button_poll(void);

int switch_time = 5; // display switch time, 5s
bool display_total = false;
bool Check_switch = true;
int last_switch_time;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*																													*/
/*											  State functions														*/
/*																													*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum ProfielState Profiel_state_idle(void)
{
	enum ProfielState next_state = ProfielStateIdle;

	const GPIO *LED_R = board_get_GPIO(GPIO_ID_LED_R);
	const GPIO *LED_G = board_get_GPIO(GPIO_ID_LED_G);
	GPIO_HAL_set(LED_R, HIGH);
	GPIO_HAL_set(LED_G, LOW);

	temp_check(&PREHEAT); // for lightning up yellow led if sensors are connected

	Start_Screen();

	return next_state;
}

enum ProfielState Profiel_state_Preheat(void)
{
	enum ProfielState next_state = ProfielStatePreheat;

	// char buffer[20];
	// snprintf(buffer, sizeof(buffer), "%d", setpoint_calc(&PREHEAT));
	// LCD_setCursor(0, 1);
	// LCD_write_string(buffer);
	Control_Relais(setpoint_calc(&PREHEAT));
	Display_switch(&PREHEAT);

	if ((State_cur_time_s() >= PREHEAT_DurS) && (temp_check(&PREHEAT) == false))
	{
		next_state = ProfielStatePreheat;
	}

	if ((State_cur_time_s() >= PREHEAT_DurS) && (temp_check(&PREHEAT) == true))
	{
		next_state = ProfielStateSoak;
	}

	return next_state;
}

enum ProfielState Profiel_state_Soak(void)
{
	enum ProfielState next_state = ProfielStateSoak;

	Control_Relais(setpoint_calc(&SOAK));
	progressbar_run(&SOAK);

	if ((State_cur_time_s() >= SOAK_DurS) && (temp_check(&SOAK) == false))
	{
		next_state = ProfielStateSoak;
	}

	if ((State_cur_time_s() >= SOAK_DurS) && (temp_check(&SOAK) == true))
	{
		next_state = ProfielStateReflow;
	}

	return next_state;
}

enum ProfielState Profiel_state_Reflow(void)
{
	enum ProfielState next_state = ProfielStateReflow;

	Control_Relais(setpoint_calc(&REFLOW));
	progressbar_run(&REFLOW);

	if ((State_cur_time_s() >= REFLOW_DurS) && (temp_check(&REFLOW) == false))
	{
		next_state = ProfielStateReflow;
	}

	if ((State_cur_time_s() >= REFLOW_DurS) && (temp_check(&REFLOW) == true))
	{
		next_state = ProfielStateDwell;
	}

	return next_state;
}

enum ProfielState Profiel_state_Dwell(void)
{
	enum ProfielState next_state = ProfielStateDwell;

	Control_Relais(setpoint_calc(&DWELL));
	progressbar_run(&DWELL);

	if ((State_cur_time_s() >= DWELL_DurS) && (temp_check(&DWELL) == false))
	{
		next_state = ProfielStateDwell;
	}

	if ((State_cur_time_s() >= DWELL_DurS) && (temp_check(&DWELL) == true))
	{
		next_state = ProfielStateCooldown;
	}

	return next_state;
}

enum ProfielState Profiel_state_Cooldown(void)
{
	enum ProfielState next_state = ProfielStateCooldown;

	Control_Relais(setpoint_calc(&COOLDOWN));
	progressbar_run(&COOLDOWN);

	if ((State_cur_time_s() >= COOLDOWN_DurS) && (temp_check(&COOLDOWN) == false))
	{
		next_state = ProfielStateCooldown;
	}

	if ((State_cur_time_s() >= COOLDOWN_DurS) && (temp_check(&COOLDOWN) == true))
	{
		Buzzer_alarm(0); // alarm song config
		next_state = ProfielStateEnd;
	}

	return next_state;
}

enum ProfielState Profiel_state_End(void)
{
	enum ProfielState next_state = ProfielStateEnd;
	const GPIO *pwm_RELAIS = board_get_GPIO(GPIO_ID_PWM_RELAIS);
	const GPIO *LED_G = board_get_GPIO(GPIO_ID_LED_G);

	GPIO_HAL_set(pwm_RELAIS, LOW);
	End_Screen();

	if (State_cur_time_s() == END_DurS)
	{
		next_state = ProfielStateIdle;
	}

	return next_state;
}

bool temp_check(struct state *fase)
{
	if (Meet_Temp() >= (fase->state_temp))
	{
		return true;
	}

	return false;
}

int setpoint_calc(struct state *fase)
{
	float Helling_Temp_S = (((fase->state_temp) - (fase->begin_temp)) / (float)(fase->duration));
	int setpoint_temp = (State_cur_time_s() * Helling_Temp_S) + (fase->begin_temp);

	if (setpoint_temp > (fase->state_temp))
	{
		return (fase->state_temp);
	}
	else
	{
		return setpoint_temp;
	}
}

void Display_switch(struct state *fase)
{
	if ((Total_cur_time_s() % switch_time) == 0)
	{
		if (Total_cur_time_s() != last_switch_time)
		{
			display_total = !display_total;
			last_switch_time = Total_cur_time_s();
		}
	}

	if (display_total == true)
	{
		progressbar_run(fase);
	}
	else
	{
		progressbar_run_total(&TOTAL);
	}
}

void state_machine(void)
{
	app_button_poll();

	enum ProfielState next_state = cur_state;
	switch (cur_state)
	{
	case ProfielStateIdle:
		next_state = Profiel_state_idle();
		break;
	case ProfielStatePreheat:
		next_state = Profiel_state_Preheat();
		break;
	case ProfielStateSoak:
		next_state = Profiel_state_Soak();
		break;
	case ProfielStateReflow:
		next_state = Profiel_state_Reflow();
		break;
	case ProfielStateDwell:
		next_state = Profiel_state_Dwell();
		break;
	case ProfielStateCooldown:
		next_state = Profiel_state_Cooldown();
		break;
	case ProfielStateEnd:
		next_state = Profiel_state_End();
		break;
	default:
		next_state = Profiel_state_idle();
	}

	if (next_state != cur_state)
	{
		State_timer_init();
		cur_state = next_state;
	}
}

void app_button_poll(void)
{
	enum BUTTON_STATUS button = button_get_status();
	const GPIO *LED_R = board_get_GPIO(GPIO_ID_LED_R);
	const GPIO *LED_G = board_get_GPIO(GPIO_ID_LED_G);

	if (button == BUTTON_STATUS_START)
	{
		if (cur_state == ProfielStateIdle)
		{
			State_timer_init();
			Total_timer_init();
			GPIO_HAL_set(LED_R, LOW);
			GPIO_HAL_set(LED_G, HIGH);
			cur_state = ProfielStatePreheat;
			return;
		}
	}

	if (button == BUTTON_STATUS_STOP)
	{
		if (cur_state != ProfielStateIdle)
		{
			State_timer_init();
			cur_state = ProfielStateEnd;
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*																													*/
/*											Main code: state machine												*/
/*																													*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	board_setup();
	board_setup_NVIC();
	board_setup_pins();

	const GPIO *Button = board_get_GPIO(GPIO_ID_BUTTON);

	// configure System Clock at 48MHz
	clock_set_frequency(CLK_FREQ);
	USBclockinit();
	SystemCoreClockUpdate();

	// start/init delay timers, usb, LCD and Button
	delay_init();
	usb_init();
	LCD_begin();
	button_init(Button);

	// config the state with values
	state_init(&PREHEAT, PREHEAT_DurS, Begin_temp, state_buffer[0], PREHEAT_temp);
	state_init(&SOAK, SOAK_DurS, PREHEAT_temp, state_buffer[1], SOAK_temp);
	state_init(&REFLOW, REFLOW_DurS, SOAK_temp, state_buffer[2], REFLOW_temp);
	state_init(&DWELL, DWELL_DurS, REFLOW_temp, state_buffer[3], DWELL_temp);
	state_init(&COOLDOWN, COOLDOWN_DurS, DWELL_temp, state_buffer[4], COOLDOWN_temp);
	state_init(&TOTAL, TOTAL_DurS, TOTAL_temp, state_buffer[5], TOTAL_temp);

	while (true)
	{
		state_machine();
	}

	return 0;
}