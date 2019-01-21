#ifndef Buzzer_songs_H
#define Buzzer_songs_H

#include "board.h"
#include "board_GPIO_ID.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <chip.h>
#include <lpc_tools/clock.h>
#include <lpc_tools/boardconfig.h>
#include <lpc_tools/GPIO_HAL.h>
#include <lpc_tools/GPIO_HAL_LPC.h>
#include <mcu_timing/delay.h>
#include "Buzzer_Motor.h"

void starwars(void);
void GameOfThrones(void);
void HarryPotter(void);
void Pirate(void);
void mario(void);
void IndianaJones(void);
void twentyCenFox(void);
void Bond(void);
void Alarm(void);

#endif