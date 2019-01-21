#ifndef PWM_H
#define PWM_H

#include <stdbool.h>

#include <chip.h>

enum PWMChannel
{
    PWM_CH0 = (1 << 0),
    PWM_CH1 = (1 << 1),
    PWM_CH2 = (1 << 2),

    // Not an actual channel, for counting only
    PWM_CH_MAX = (1 << 3),
};

typedef struct
{
    LPC_TIMER_T *timer;
    unsigned int resolution;
} PWM;

bool PWM_init(PWM *ctx, LPC_TIMER_T *timer, unsigned int pwm_channels, unsigned int pwm_frequency, unsigned int resolution);

unsigned int PWM_get_resolution(PWM *ctx);

/**
 * Start running an initialized PWM instance
 *
 * Usually you should call this right after PWM_init(),
 * but you have the option to adjust the values with PWM_set() before starting
 */
bool PWM_start(PWM *ctx);

/**
 * Set PWM value for a given channel.
 *
 * @param pwm_value     Vary the duty cycle (0-'resolution')
 *                      0 = fully off,
 *                      'resolution' = fully on
 *                      anything in between proportionally sets the duty cycle.
 */
bool PWM_set(PWM *ctx, enum PWMChannel channel, unsigned int pwm_value);

#endif
