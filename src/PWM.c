#include "PWM.h"
#include <c_utils/assert.h>
#include <c_utils/max.h>

bool PWM_init(PWM *ctx, LPC_TIMER_T *timer, unsigned int pwm_channels, unsigned int target_frequency, unsigned int target_resolution)
{
    if (!target_frequency)
    {
        return 0;
    }

    ctx->timer = timer;
    ctx->resolution = 0;

    const uint32_t clk_freq = Chip_Clock_GetMainClockRate();

    // Try to create a pwm frequency >= the requested frequency
    const uint32_t ticks_per_cycle = clk_freq / target_frequency;

    // this hs the actual frequency:
    //const uint32_t actual_pwm_frequency = clk_freq / ticks_per_cycle;

    // Default: max resolution
    uint32_t prescaler = 1;

    // Try to select a resolution >= the requested resolution
    if (target_resolution)
    {
        prescaler = ticks_per_cycle / target_resolution;
        // prescaler is 16 bit and should be >= 1
        if (!(prescaler & 0xFFFF))
        {
            return false;
        }
    }

    unsigned int resolution = ticks_per_cycle / prescaler;

    // resolution < 2 will obviously not work
    if (resolution < 2)
    {
        return false;
    }

    ctx->resolution = ticks_per_cycle / prescaler;

    Chip_TIMER_Init(timer);
    Chip_TIMER_Reset(timer);
    Chip_TIMER_PrescaleSet(LPC_TIMER16_1, (prescaler - 1));

    // Init all channels to fully off
    unsigned int ch = 0;
    for (unsigned int i = 0; ch < PWM_CH_MAX; i++)
    {
        ch = (1 << i);

        if (pwm_channels & ch)
        {

            // default to off
            PWM_set(ctx, ch, 0);

            Chip_TIMER_ResetOnMatchDisable(LPC_TIMER16_1, i);
            Chip_TIMER_StopOnMatchDisable(LPC_TIMER16_1, i);

            // Set the channel to PWM mode (instead of EMR register)
            LPC_TIMER16_1->PWMC |= ch;
        }
    }

    // set PWM frequency using match 3
    const int period_ch = 3;
    assert((1 << period_ch) == PWM_CH_MAX);
    Chip_TIMER_SetMatch(timer, period_ch, ctx->resolution - 1);
    Chip_TIMER_ResetOnMatchEnable(timer, period_ch);
    Chip_TIMER_StopOnMatchDisable(timer, period_ch);

    return true;
}

unsigned int PWM_get_resolution(PWM *ctx)
{
    return ctx->resolution;
}

bool PWM_start(PWM *ctx)
{
    if (!ctx->resolution)
    {
        return false;
    }

    Chip_TIMER_Enable(ctx->timer);
    return true;
}

bool PWM_set(PWM *ctx, enum PWMChannel channel, unsigned int pwm_value)
{
    if (!ctx->resolution)
    {
        return false;
    }

    // constrain pwm_value within bounds
    pwm_value = min(pwm_value, ctx->resolution);

    unsigned int index = 0;
    switch (channel)
    {
    case PWM_CH0:
        index = 0;
        break;
    case PWM_CH1:
        index = 1;
        break;
    case PWM_CH2:
        index = 2;
        break;
    default:
        return false;
    }

    // Timer logic is inverted: 0 = fully ON, resolution = fully OFF
    Chip_TIMER_SetMatch(ctx->timer, index, ctx->resolution - pwm_value);
    return true;
}
