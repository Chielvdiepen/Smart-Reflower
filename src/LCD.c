#include "LCD.h"
#include "i2c.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <chip.h>
#include <mcu_timing/delay.h>
#include <lpc_tools/GPIO_HAL.h>
#include "board.h"
#include "board_GPIO_ID.h"
#include <string.h>

static const int HD44780_CHEXECTIME = 2000; // time in us for clear&home
static const int HD44780_INSEXECTIME = 38;

// API return values
// 0 means successful, less than zero means unsuccessful
static const int RV_ENOERR = 0; // no error. Do not change!
// static const int RV_EIO = -1;      // i/o operation failed (generic/default error)
// static const int RV_EINVAL = -2;   // invalid argument/parameter
static const int RV_ENOTSUP = -3; // not supported
// static const int RV_ENXIO = -4;    // no such device or address
// static const int RV_EMSGSIZE = -5; // Message/data too long

// commands
static const uint8_t HD44780_CLEARDISPLAY = 0x01;
static const uint8_t HD44780_RETURNHOME = 0x02;
static const uint8_t HD44780_ENTRYMODESET = 0x04;
static const uint8_t HD44780_DISPLAYCONTROL = 0x08;
static const uint8_t HD44780_CURDISPSHIFT = 0x10;
static const uint8_t HD44780_FUNCTIONSET = 0x20;
// static const uint8_t HD44780_SETCGRAMADDR = 0x40;
static const uint8_t HD44780_SETDDRAMADDR = 0x80;

// flags for entry mode set;
static const uint8_t HD44780_ENTRYLEFT2RIGHT = 0x02;
static const uint8_t HD44780_ENTRYAUTOSHIFT = 0x01;

// flags for display on/off control;
static const uint8_t HD44780_DISPLAYON = 0x04;

//	static const uint8_t HD44780_DISPLAYOFF = 0x00;
static const uint8_t HD44780_CURSORON = 0x02;
static const uint8_t HD44780_CURSOROFF = 0x00;
static const uint8_t HD44780_BLINKON = 0x01;
static const uint8_t HD44780_BLINKOFF = 0x00;

// flags for cursor/display shift;
static const uint8_t HD44780_DISPLAYMOVE = 0x08;
static const uint8_t HD44780_CURSORMOVE = 0x00;
static const uint8_t HD44780_MOVERIGHT = 0x04;
static const uint8_t HD44780_MOVELEFT = 0x00;

// flags for function set;
static const uint8_t HD44780_8BITMODE = 0x10;
static const uint8_t HD44780_4BITMODE = 0x00;
static const uint8_t HD44780_2LINE = 0x08;
// static const uint8_t HD44780_1LINE = 0x00;
// static const uint8_t HD44780_5x10DOTS = 0x04;
// static const uint8_t HD44780_5x8DOTS = 0x00;

// RS signal in HD44780 interface
// set for data writes, clear for cmds
static uint8_t I2Clcd_RS = (1 << 6);

// set if another control byte following data
// clear if last control byte
// static  uint8_t I2Clcd_CO = (1 << 7);

// List of important variables
static uint8_t _rowOffsets[4]; // memory address of start of each row/line
static uint8_t _cols = 20;
static uint8_t _rows = 4;
static uint8_t _curcol;    // current LCD col if doing char & line processing
static uint8_t _currow;    // current LCD row if doing char & line processing
static uint8_t _wraplines; // set to nonzero if wrapping long lines
static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;

// global variable
uint32_t _chExecTime;  // time in Us of execution time for clear/home
uint32_t _insExecTime; // time in Us of execution time for instructions or data
uint32_t _startTime;   // 'start' time of last thing sent to LCD (cmd or data)
uint32_t _execTime;    // execution time in Us of last thing sent to LCD (cmd or data)

enum iotype // type of data being sent through ioread()/iowrite()
{
    HD44780_IOcmd,    //    IOcmd  - read/write an 8 bit status/command to/from display
    HD44780_IOdata,   // IOdata - read/writes 8 bits of data to/from display
    HD44780_IOcmd4bit // IOcmd4bit - write D4-d7 cmd bits to display using a single EN strobe. 8 bit host interfaces can treate this same as IOcmd
};

// wait for lcd to be ready
void waitReady(int32_t offsetUs)
{
    while ((((uint64_t)delay_get_timestamp()) - (_startTime + offsetUs)) < _execTime)
    {
    }
}

// stuff for tracking execution times
void markStart(uint32_t exectime)
{
    _startTime = (uint64_t)delay_get_timestamp(); // in micro seconds
    _execTime = exectime;
}

// set execution times for commmands to override defaults
void setExecTimes(uint32_t chExecTimeUs, uint32_t insExecTimeUs)
{
    _chExecTime = chExecTimeUs;
    _insExecTime = insExecTimeUs;
}

////////////////////////////////////////////////////

// ioinit() - Returns non zero if initialization failed.
int ioinit()
{
    int status = RV_ENOERR;

    i2c_init();

    _displayfunction = HD44780_8BITMODE;

    return (status);
}

int ioread(enum iotype type)
{
    if (type)
    {
        return (RV_ENOTSUP);
    }
    else
    {
        return (RV_ENOTSUP);
    } // optional, return fail if not implemented
}

// iowrite(type, value) - send either a command or data byte to lcd
// returns zero on success, non zero on failure
int iowrite(enum iotype type, uint8_t value)
{
    uint8_t ctlbyte;

    if (type == HD44780_IOdata)
    {
        ctlbyte = I2Clcd_RS; // control byte with RS and no continue
    }
    else
    {
        ctlbyte = 0; // control byte with no RS and no continue
    }

    waitReady(-25);

    uint8_t txBuffer[2];

    txBuffer[0] = ctlbyte;
    txBuffer[1] = value;

    i2c_send(txBuffer, sizeof(txBuffer));

    return (RV_ENOERR);
}

int LCD_begin()
{
    int rval = 0;

    setExecTimes(HD44780_CHEXECTIME, HD44780_INSEXECTIME);
    markStart(0); // initialize last start time to 'now'

    if (_rows > sizeof(_rowOffsets) / sizeof(_rowOffsets[0]))
    {
        _rows = sizeof(_rowOffsets) / sizeof(_rowOffsets[0]);
    }
    if (!_rowOffsets[0] && !_rowOffsets[1] && !_rowOffsets[2] && !_rowOffsets[3])
    {
        LCD_setRowOffsets(0x00, 0x40, 0x00 + _cols, 0x40 + _cols);
    }
    delay_us(100000); // 100 mili seconds

    // default to 4 bit mode (can be overridded by i/o class library)
    _displayfunction = HD44780_4BITMODE;

    // call h/w i/o class init function
    if ((rval = ioinit())) // intentional assignment
    {
        return (rval);
    }

    if (_rows > 1)
    {
        _displayfunction |= HD44780_2LINE;
    }

    iowrite(HD44780_IOcmd4bit, HD44780_FUNCTIONSET | HD44780_8BITMODE);
    delay_us(5000); // wait 5ms vs 4.1ms, some are slower than spec

    iowrite(HD44780_IOcmd4bit, HD44780_FUNCTIONSET | HD44780_8BITMODE);
    delay_us(1000); // wait 1ms vs 100us

    iowrite(HD44780_IOcmd4bit, HD44780_FUNCTIONSET | HD44780_8BITMODE);
    delay_us(1000); // wait 1ms vs 100us

    if (!(_displayfunction & HD44780_8BITMODE))
    {
        iowrite(HD44780_IOcmd4bit, HD44780_FUNCTIONSET | HD44780_4BITMODE);
    }
    // set # lines, font size, etc.
    LCD_command(HD44780_FUNCTIONSET | _displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol = HD44780_DISPLAYON | HD44780_CURSOROFF | HD44780_BLINKOFF;
    LCD_display();

    LCD_clear(); // clear display

    // Initialize to default text direction (for romance languages)
    _displaymode = HD44780_ENTRYLEFT2RIGHT;

    // Choose to turn on/off linewrapping
    LCD_lineWrap();

    // set the entry mode
    rval = LCD_command(HD44780_ENTRYMODESET | _displaymode);

    return (rval);
}

int LCD_clear()
{
    return (LCD_command(HD44780_CLEARDISPLAY)); // clear display, set cursor to 0,0
}

int LCD_home()
{
    return (LCD_command(HD44780_RETURNHOME)); // set cursor position to 0,0
}

int LCD_setCursor(uint8_t col, uint8_t row)
{
    if (row >= _rows)
    {
        // set to max line (rows/lines start at 0, _rows is 1 based)
        row = _rows - 1;
    }

    if (_wraplines)
    {
        // wrap while requested col > toal cols
        while (col >= _cols)
        {
            col -= _cols;
            if (++row >= _rows)
                row = 0; // wrap back to top line
        }

        // save position
        _curcol = col;
        _currow = row;
    }

    return (LCD_command(HD44780_SETDDRAMADDR | (col + _rowOffsets[row])));
}

// set row offset
int LCD_setRowOffsets(int row0, int row1, int row2, int row3)
{
    _rowOffsets[0] = row0;
    _rowOffsets[1] = row1;
    _rowOffsets[2] = row2;
    _rowOffsets[3] = row3;
    return (RV_ENOERR);
}

// turn off display pixels
int LCD_noDisplay()
{
    _displaycontrol &= ~HD44780_DISPLAYON;
    return (LCD_command(HD44780_DISPLAYCONTROL | _displaycontrol));
}

// turn on display pixels
int LCD_display()
{
    _displaycontrol |= HD44780_DISPLAYON;
    return (LCD_command(HD44780_DISPLAYCONTROL | _displaycontrol));
}

// Turns off underline cursor
int LCD_noCursor()
{
    _displaycontrol &= ~HD44780_CURSORON;
    return (LCD_command(HD44780_DISPLAYCONTROL | _displaycontrol));
}
// Turn on underline cursor
int LCD_cursor()
{
    _displaycontrol |= HD44780_CURSORON;
    return (LCD_command(HD44780_DISPLAYCONTROL | _displaycontrol));
}

// Turn on off the blinking cursor
int LCD_noBlink()
{
    _displaycontrol &= ~HD44780_BLINKON;
    return (LCD_command(HD44780_DISPLAYCONTROL | _displaycontrol));
}

// Turn on the blinking cursor
int LCD_blink()
{
    _displaycontrol |= HD44780_BLINKON;
    return (LCD_command(HD44780_DISPLAYCONTROL | _displaycontrol));
}

// These API functions scroll/shift the display contents without changing the RAM
int LCD_scrollDisplayLeft(void)
{
    return (LCD_command(HD44780_CURDISPSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVELEFT));
}
int LCD_scrollDisplayRight(void)
{
    return (LCD_command(HD44780_CURDISPSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVERIGHT));
}

// This is for text that flows Left to Right
int LCD_leftToRight(void)
{
    _displaymode |= HD44780_ENTRYLEFT2RIGHT;
    return (LCD_command(HD44780_ENTRYMODESET | _displaymode));
}

// This is for text that flows Right to Left
int LCD_rightToLeft(void)
{
    _displaymode &= ~HD44780_ENTRYLEFT2RIGHT;
    return (LCD_command(HD44780_ENTRYMODESET | _displaymode));
}

// This moves the cursor one space to the right
int LCD_moveCursorRight(void)
{
    return (LCD_command(HD44780_CURDISPSHIFT | HD44780_CURSORMOVE | HD44780_MOVERIGHT));
}

// This moves the cursor one space to the left
int LCD_moveCursorLeft(void)
{
    return (LCD_command(HD44780_CURDISPSHIFT | HD44780_CURSORMOVE | HD44780_MOVELEFT));
}

// This will enable autoshifting display as new characters are written.
// If mode is left to right, shift is left
// if mode is right to left, shift is right
int LCD_autoscroll(void)
{
    _displaymode |= HD44780_ENTRYAUTOSHIFT;
    return (LCD_command(HD44780_ENTRYMODESET | _displaymode));
}

// This will disable autoshifting when new characters are written
int LCD_noAutoscroll(void)
{
    _displaymode &= ~HD44780_ENTRYAUTOSHIFT;
    return (LCD_command(HD44780_ENTRYMODESET | _displaymode));
}

// turn on pixels
int LCD_on(void)
{
    int rval;

    rval = LCD_display();
    return (rval);
}

// turn off pixels
int LCD_off(void)
{
    return (LCD_noDisplay());
}

// command() - send hd44780 command byte to lcd
// Returns 0 on success, non zero if command failed
int LCD_command(uint8_t value)
{
    int status;

    status = iowrite(HD44780_IOcmd, value);

    // executime time depends on command
    if ((value == HD44780_CLEARDISPLAY) || (value == HD44780_RETURNHOME))
    {
        _curcol = 0;
        _currow = 0;
        markStart(_chExecTime);
    }
    else
    {
        markStart(_insExecTime);
    }

    return (status);
}

// status() - read status byte from LCD
// returns:
// 	success: 8 bit hd44780 status byte (busy flag & address)
//	failure: neagative value (error or read/status not supported by i/o subclass
int LCD_status()
{
    int rvalue = ioread(HD44780_IOcmd);
    return (rvalue);
}

// read() - read a data byte from LCD
// returns:
// 	success: 8 bit value read
//	failure: neagative value (error or read not supported by i/o subclass
int LCD_read()
{
    int rvalue = ioread(HD44780_IOdata);
    markStart(_insExecTime);
    return (rvalue);
}

// enable automatic line wrapping (only works in left 2 right mode)
int LCD_lineWrap(void)
{
    if (_displaymode & HD44780_ENTRYLEFT2RIGHT)
    {
        _wraplines = 1;
        return (RV_ENOERR);
    }
    else
    {
        return (RV_ENOTSUP);
    }
}
// disable automatic line wrapping
int LCD_noLineWrap(void)
{
    _wraplines = 0;
    return (RV_ENOERR);
}; // turn off automatic line wrapping

// write() - process data character byte to lcd
// returns number of bytes successfully written to device
// i.e. 1 if success or 0 if no character was processed (error)
int LCD_write(uint8_t value)
{
    int status = 1;

    if (iowrite(HD44780_IOdata, value))
        status = 0;
    markStart(_insExecTime);

    if (_wraplines)
    {
        if (++_curcol >= _cols)
        {
            _curcol = 0;
            _currow++;
            if (_currow >= _rows)
                _currow = 0;
            LCD_setCursor(_curcol, _currow);
        }
    }
    return (status);
}

void LCD_write_string(char *textbuffer)
{
    int char_count = strlen(textbuffer);
    for (int i = 0; i < char_count; i++)
    {
        LCD_write(*textbuffer);
        textbuffer++;
    }
}
