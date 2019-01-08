#include "board.h"
#include "board_GPIO_ID.h"

#include <lpc_tools/boardconfig.h>
#include <lpc_tools/GPIO_HAL.h>
#include <c_utils/static_assert.h>

#include <chip.h>

// Oscillator frequency, needed by chip libraries
const uint32_t OscRateIn = 12000000;


static const NVICConfig NVIC_config[] = {
    {TIMER_32_0_IRQn,       1},     // delay timer: high priority
};

static const PinMuxConfig pinmuxing[] = { // config of pin and function from datasheet

        // {Poort,Pin,(Functie)}
        {0,   7, (IOCON_FUNC0)},          // LED STATUS
	{0,   4, (IOCON_FUNC1)},          // SCL
	{0,   5, (IOCON_FUNC1)},          // SDA
	{0,   0, (IOCON_FUNC0)},          // RESET
	{1,  26, (IOCON_FUNC0)},          // PWM DC
	{0,  14, (IOCON_FUNC1)},          // PWM RELAIS
	{1,  25, (IOCON_FUNC0)},          // PWM EXTERNAL
	{1,  19, (IOCON_FUNC0)},          // PWM SERVO
	{0,  22, (IOCON_FUNC0)},          // BUZZER
	{1,  20, (IOCON_FUNC0)},          // CS1
	{1,  27, (IOCON_FUNC0)},          // CS2
	{0,   3, (IOCON_FUNC1)},          // VBUS
	{1,  31, (IOCON_FUNC0)},          // LED RED
	{1,  21, (IOCON_FUNC0)},          // LED YELLOW
	{1,   9, (IOCON_FUNC0)},          // LED GREEN
	{0,  13, (IOCON_FUNC1)},          // BUTTON
	{0,  10, (IOCON_FUNC0)},          // SWCLK
	{1,  29, (IOCON_FUNC1)},          // SCK0
	{0,   8, (IOCON_FUNC1)},          // MISO0
	{0,  15, (IOCON_FUNC0)},          // SWDIO
};

static const GPIOConfig pin_config[] = { // config of pin/name and default value

	//[alias],{Poort,Pin,Default-config}
    	[GPIO_ID_LED_STATUS]=	{{0,   7}, GPIO_CFG_DIR_OUTPUT_LOW},
    	[GPIO_ID_LED_R]=	{{1,  31}, GPIO_CFG_DIR_OUTPUT_LOW},  
    	[GPIO_ID_LED_Y]=	{{1,  21}, GPIO_CFG_DIR_OUTPUT_LOW},  
    	[GPIO_ID_LED_G]=	{{1,   9}, GPIO_CFG_DIR_OUTPUT_LOW},  
    	[GPIO_ID_PWM_DC]=	{{1,  26}, GPIO_CFG_DIR_OUTPUT_LOW},  
    	[GPIO_ID_PWM_RELAIS]=	{{0,  14}, GPIO_CFG_DIR_OUTPUT_LOW},  
    	[GPIO_ID_PWM_EXT]=	{{1,  25}, GPIO_CFG_DIR_OUTPUT_LOW},  
    	[GPIO_ID_PWM_SERVO]=	{{1,  19}, GPIO_CFG_DIR_OUTPUT_LOW},  
    	[GPIO_ID_BUZZER]=	{{0,  22}, GPIO_CFG_DIR_OUTPUT_LOW},  
    	[GPIO_ID_CS1]=		{{1,  20}, GPIO_CFG_DIR_OUTPUT_LOW},  
    	[GPIO_ID_CS2]=		{{1,  27}, GPIO_CFG_DIR_OUTPUT_LOW},  
    	[GPIO_ID_BUTTON]=	{{0,  13},  GPIO_CFG_DIR_INPUT_LOW},    
};

// pin config struct should match GPIO_ID enum
STATIC_ASSERT( (GPIO_ID_MAX == (sizeof(pin_config)/sizeof(GPIOConfig))));


static const BoardConfig config = {
    .nvic_configs = NVIC_config,
    .nvic_count = sizeof(NVIC_config) / sizeof(NVIC_config[0]),

    .pinmux_configs = pinmuxing,
    .pinmux_count = sizeof(pinmuxing) / sizeof(pinmuxing[0]),

    .GPIO_configs = pin_config,
    .GPIO_count = sizeof(pin_config) / sizeof(pin_config[0]),

    .ADC_configs = NULL,
    .ADC_count = 0
};


void board_setup(void)
{
    board_set_config(&config);
}

