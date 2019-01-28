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
#include "button.h"

static volatile enum BUTTON_STATUS g_status;

#define GPIO_PININT_FALLING_INDEX 0
#define GPIO_PININT_RISING_INDEX 1
#define PININT_FALLING_IRQ_HANDLER FLEX_INT0_IRQHandler /* PININT IRQ function name */
#define PININT_RISING_IRQ_HANDLER FLEX_INT1_IRQHandler  /* PININT IRQ function name */

const uint64_t BUTTON_TIME_PROGRAM_START_ms = 8;
const uint64_t BUTTON_TIME_PROGRAM_ABORT_ms = 2000;

static uint64_t button_press;

static void reset_status()
{
    g_status = BUTTON_STATUS_NOTHING;
}

enum BUTTON_STATUS button_get_status()
{
    if (button_press)
    {
        uint64_t button_interval_us = delay_calc_time_us(button_press, delay_get_timestamp());

        // Actions on button pressed > predefined time
        if (button_interval_us > BUTTON_TIME_PROGRAM_ABORT_ms * 1000)
        {
            g_status = BUTTON_STATUS_STOP;
        }
    }

    enum BUTTON_STATUS status = g_status;

    if (!button_press)
    {
        reset_status();
    }

    return status;
}

void PININT_FALLING_IRQ_HANDLER(void)
{
    Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(GPIO_PININT_FALLING_INDEX));
    button_press = delay_get_timestamp();
    const GPIO *STATUS_LED = board_get_GPIO(GPIO_ID_LED_STATUS);
    GPIO_HAL_set(STATUS_LED, HIGH);
}

void PININT_RISING_IRQ_HANDLER(void)
{
    Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(GPIO_PININT_RISING_INDEX));

    uint64_t button_interval_us = delay_calc_time_us(button_press, delay_get_timestamp());

    // Actions on button going up
    if ((button_interval_us > BUTTON_TIME_PROGRAM_START_ms * 1000) &&
        (button_interval_us < BUTTON_TIME_PROGRAM_ABORT_ms * 1000))
    {
        g_status = BUTTON_STATUS_START;
    }
    const GPIO *STATUS_LED = board_get_GPIO(GPIO_ID_LED_STATUS);
    GPIO_HAL_set(STATUS_LED, LOW);
    button_press = 0;
}

void button_init(const GPIO *button_pin)
{

    reset_status();
    /* Enable PININT clock */
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PINT);

    // /* Configure interrupt channel for the GPIO pin in SysCon block */
    Chip_SYSCTL_SetPinInterrupt(GPIO_PININT_FALLING_INDEX, button_pin->port, button_pin->pin);

    // /* Configure channel interrupt as edge sensitive and falling edge interrupt */
    Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(GPIO_PININT_FALLING_INDEX));
    Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(GPIO_PININT_FALLING_INDEX));
    NVIC_EnableIRQ(PIN_INT0_IRQn);

    /* Configure interrupt channel for the GPIO pin in SysCon block */
    Chip_SYSCTL_SetPinInterrupt(GPIO_PININT_RISING_INDEX, button_pin->port, button_pin->pin);

    /* Configure channel interrupt as edge sensitive and falling edge interrupt */
    Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(GPIO_PININT_RISING_INDEX));
    Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(GPIO_PININT_RISING_INDEX));

    NVIC_EnableIRQ(PIN_INT1_IRQn);

    /* Configure channel as wake up interrupt in SysCon block */
    // Chip_SYSCTL_EnablePINTWakeup(GPIO_PININT_INDEX);
}
