#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <chip.h>
#include <lpc_tools/GPIO_HAL.h>
#include "board.h"
#include "board_GPIO_ID.h"

int LCD_write(uint8_t value);            // write single char to LCD
void LCD_write_string(char *textbuffer); // writes 20 characters to line
int LCD_clear();
int LCD_home();
int LCD_begin();
int LCD_setCursor(uint8_t col, uint8_t row); // character place (0-19), Textline (0-3)
int LCD_cursor();
int LCD_noCursor();
int LCD_blink();
int LCD_noBlink();
int LCD_display();   // turn on LCD pixels
int LCD_noDisplay(); // turn off LCD pixels
int LCD_scrollDisplayLeft();
int LCD_scrollDisplayRight();
int LCD_autoscroll();   // auto horizontal scrolling
int LCD_noAutoscroll(); // no auto horizontal scrolling
int LCD_leftToRight();
int LCD_rightToLeft();
int LCD_moveCursorLeft();
int LCD_moveCursorRight();
int LCD_setRowOffsets(int row0, int row1, int row2, int row3);
int LCD_command(uint8_t); // command(), returns 0 on success, non zero on command failure
int LCD_on(void);         // turn on LCD pixels
int LCD_off(void);        // turn off LCD pixels
int LCD_status();         // returns: success: 8 bit hd44780 status byte read from LCD (busy flag & address), failure: negataive value (error or read/status not supported)
int LCD_read(void);
int LCD_noLineWrap(void);
int LCD_lineWrap(void);

// end of class definition
#endif
