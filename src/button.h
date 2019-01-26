#ifndef BUTTON_H
#define BUTTON_H

#include <lpc_tools/GPIO_HAL.h>

enum BUTTON_STATUS {
    BUTTON_STATUS_NOTHING,
    BUTTON_STATUS_STOP = 1,
    BUTTON_STATUS_START
};

void button_init(const GPIO *button_pin);

enum BUTTON_STATUS button_get_status();

#endif