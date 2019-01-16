#include "board.h"
#include "board_GPIO_ID.h"
#include <string.h>

#include <chip.h>
#include <lpc_tools/clock.h>
#include <lpc_tools/boardconfig.h>
#include <lpc_tools/GPIO_HAL.h>
#include <lpc_tools/GPIO_HAL_LPC.h>
#include <mcu_timing/delay.h>

// max31855 includes
#include "MAX31855.h"

// LCD includes
#include "i2c.h"
#include "LCD.h"

// USB include
#include "usb_init.h"
#include "usb/cdc_vcom.h"

#define CLK_FREQ (48e6)

int main(void)
{
	board_setup();
	board_setup_NVIC();
	board_setup_pins();

	// configure System Clock at 48MHz
	clock_set_frequency(CLK_FREQ);
	USBclockinit();
	SystemCoreClockUpdate();

	delay_init();
	usb_init();

	// get the GPIO's
	const GPIO *led_STATUS = board_get_GPIO(GPIO_ID_LED_STATUS);

	LCD_begin();
	delay_us(3000000);
	LCD_blink();
	///string size///|....................|///string size///
	LCD_write_string("   Smart-Reflower   ");
	delay_us(2000000);
	LCD_write_string("   Smart-Reflower   ");
	delay_us(3000000);
	LCD_write_string("   Smart-Reflower   ");
	delay_us(3000000);
	LCD_write_string("   Smart-Reflower   ");
	delay_us(3000000);
	// LCD_write(255); // block character

	int prompt = 0;
	while (true)
	{
		if ((vcom_connected() != 0) && (prompt == 0))
		{
			vcom_write("Jitter Smart-Reflower VCOM.\r\n", 29);
			prompt = 1;
		}
		if (prompt)
		{
			GPIO_HAL_toggle(led_STATUS);
			GeefTemp();
			delay_us(5E6);
		}
	}
	return 0;
}
