#ifndef BOARD_GPIO_ID_H
#define BOARD_GPIO_ID_H

enum GPIO_ID
{
	GPIO_ID_LED_STATUS,
	GPIO_ID_LED_R,
	GPIO_ID_LED_Y,
	GPIO_ID_LED_G,
	GPIO_ID_PWM_RELAIS,
	GPIO_ID_PWM_DC,
	GPIO_ID_PWM_EXT,
	GPIO_ID_PWM_SERVO,
	GPIO_ID_CS1,
	GPIO_ID_CS2,
	GPIO_ID_BUTTON,
	GPIO_ID_BUZZER,
	GPIO_ID_MAX // This should be last: it is used to count
};

#endif