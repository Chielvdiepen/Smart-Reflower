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

// define of notes
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS 455
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GSH 830
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

// Alarm tune
void Alarm(void)
{
    for (int i = 0; i < 3; i++) // x keer alarm
    {
        Buzzer_tone(NOTE_B4, 400);
        Buzzer_tone(NOTE_B5, 400);
        Buzzer_tone(NOTE_FS6, 400);
        Buzzer_tone(NOTE_B6, 400);
    }
}

//******* Starwars coté obscure****************
void starwars(void)
{
    Buzzer_tone(NOTE_A4, 500);
    Buzzer_tone(NOTE_A4, 500);
    Buzzer_tone(NOTE_A4, 500);
    Buzzer_tone(NOTE_F4, 350);
    Buzzer_tone(NOTE_C5, 150);
    Buzzer_tone(NOTE_A4, 500);
    Buzzer_tone(NOTE_F4, 350);
    Buzzer_tone(NOTE_C5, 150);
    Buzzer_tone(NOTE_A4, 650);
    delay_us(400000);
    Buzzer_tone(NOTE_E5, 500);
    Buzzer_tone(NOTE_E5, 500);
    Buzzer_tone(NOTE_E5, 500);
    Buzzer_tone(NOTE_F5, 350);
    Buzzer_tone(NOTE_C5, 150);
    Buzzer_tone(NOTE_GS4, 500);
    Buzzer_tone(NOTE_F4, 350);
    Buzzer_tone(NOTE_C5, 150);
    Buzzer_tone(NOTE_A4, 650);
    delay_us(400000);
    Buzzer_tone(NOTE_A5, 500);
    Buzzer_tone(NOTE_A4, 300);
    Buzzer_tone(NOTE_A4, 150);
    Buzzer_tone(NOTE_A5, 500);
    Buzzer_tone(NOTE_GSH, 325);
    Buzzer_tone(NOTE_G5, 175);
    Buzzer_tone(NOTE_FS5, 125);
    Buzzer_tone(NOTE_F5, 125);
    Buzzer_tone(NOTE_FS5, 250);
    delay_us(325000);
    Buzzer_tone(NOTE_AS, 250);
    Buzzer_tone(NOTE_DS5, 500);
    Buzzer_tone(NOTE_D5, 325);
    Buzzer_tone(NOTE_CS5, 175);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_AS4, 125);
    Buzzer_tone(NOTE_C5, 250);
    delay_us(350000);
    Buzzer_tone(NOTE_F4, 250);
    Buzzer_tone(NOTE_GS4, 500);
    Buzzer_tone(NOTE_F4, 375);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_A4, 500);
    Buzzer_tone(NOTE_F4, 375);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_A4, 650);
}

//******* GameOfThrones ****************
void GameOfThrones(void)
{
    for (int i = 0; i < 3; i++)
    {
        Buzzer_tone(NOTE_G4, 500);
        Buzzer_tone(NOTE_C4, 500);
        Buzzer_tone(NOTE_DS4, 250);
        Buzzer_tone(NOTE_F4, 250);
    }

    for (int i = 0; i < 3; i++)
    {
        Buzzer_tone(NOTE_G4, 500);
        Buzzer_tone(NOTE_C4, 500);
        Buzzer_tone(NOTE_E4, 250);
        Buzzer_tone(NOTE_F4, 250);
    }

    Buzzer_tone(NOTE_G4, 500);
    Buzzer_tone(NOTE_C4, 500);
    Buzzer_tone(NOTE_DS4, 250);
    Buzzer_tone(NOTE_F4, 250);
    Buzzer_tone(NOTE_D4, 500);

    for (int i = 0; i < 2; i++)
    {
        Buzzer_tone(NOTE_G3, 500);
        Buzzer_tone(NOTE_AS3, 250);
        Buzzer_tone(NOTE_C4, 250);
        Buzzer_tone(NOTE_D4, 500);
    }

    Buzzer_tone(NOTE_G3, 500);
    Buzzer_tone(NOTE_AS3, 250);
    Buzzer_tone(NOTE_C4, 250);
    Buzzer_tone(NOTE_D4, 1000);
    Buzzer_tone(NOTE_F4, 1000);
    Buzzer_tone(NOTE_AS3, 1000);
    Buzzer_tone(NOTE_DS4, 250);
    Buzzer_tone(NOTE_D4, 250);
    Buzzer_tone(NOTE_F4, 1000);
    Buzzer_tone(NOTE_AS3, 1000);
    Buzzer_tone(NOTE_DS4, 250);
    Buzzer_tone(NOTE_D4, 250);
    Buzzer_tone(NOTE_C4, 500);

    for (int i = 0; i < 2; i++)
    {
        Buzzer_tone(NOTE_GS3, 250);
        Buzzer_tone(NOTE_AS3, 250);
        Buzzer_tone(NOTE_C4, 500);
        Buzzer_tone(NOTE_F3, 500);
    }

    Buzzer_tone(NOTE_G4, 1000);
    Buzzer_tone(NOTE_C4, 1000);
    Buzzer_tone(NOTE_DS4, 250);
    Buzzer_tone(NOTE_F4, 250);
    Buzzer_tone(NOTE_G4, 1000);
    Buzzer_tone(NOTE_C4, 1000);
    Buzzer_tone(NOTE_DS4, 250);
    Buzzer_tone(NOTE_F4, 250);
    Buzzer_tone(NOTE_D4, 500);

    for (int i = 0; i < 3; i++)
    {
        Buzzer_tone(NOTE_G3, 500);
        Buzzer_tone(NOTE_AS3, 250);
        Buzzer_tone(NOTE_C4, 250);
        Buzzer_tone(NOTE_D4, 500);
    }
}

//******* Harry Potter ****************
void HarryPotter(void)
{
    Buzzer_tone(NOTE_B4, 333);
    Buzzer_tone(NOTE_E5, 500);
    Buzzer_tone(NOTE_G5, 166);
    Buzzer_tone(NOTE_FS5, 333);
    Buzzer_tone(NOTE_E5, 666);
    Buzzer_tone(NOTE_B5, 333);
    Buzzer_tone(NOTE_A5, 1000);
    Buzzer_tone(NOTE_FS5, 1000);
    Buzzer_tone(NOTE_E5, 500);
    Buzzer_tone(NOTE_G5, 166);
    Buzzer_tone(NOTE_FS5, 333);
    Buzzer_tone(NOTE_DS5, 666);
    Buzzer_tone(NOTE_F5, 333);
    Buzzer_tone(NOTE_B4, 1666);
    Buzzer_tone(NOTE_B4, 333);
    Buzzer_tone(NOTE_E5, 500);
    Buzzer_tone(NOTE_G5, 166);
    Buzzer_tone(NOTE_FS5, 333);
    Buzzer_tone(NOTE_E5, 666);
    Buzzer_tone(NOTE_B5, 333);
    Buzzer_tone(NOTE_D6, 666);
    Buzzer_tone(NOTE_CS6, 333);
    Buzzer_tone(NOTE_C6, 666);
    Buzzer_tone(NOTE_GS5, 333);
    Buzzer_tone(NOTE_C6, 500);
    Buzzer_tone(NOTE_B5, 166);
    Buzzer_tone(NOTE_AS5, 333);
    Buzzer_tone(NOTE_AS4, 666);
    Buzzer_tone(NOTE_G5, 333);
    Buzzer_tone(NOTE_E5, 1666);
    Buzzer_tone(NOTE_G5, 333);
    Buzzer_tone(NOTE_B5, 666);
    Buzzer_tone(NOTE_G5, 333);
    Buzzer_tone(NOTE_B5, 666);
    Buzzer_tone(NOTE_G5, 333);
    Buzzer_tone(NOTE_C6, 666);
    Buzzer_tone(NOTE_B5, 333);
    Buzzer_tone(NOTE_AS5, 666);
    Buzzer_tone(NOTE_FS5, 333);
    Buzzer_tone(NOTE_G5, 500);
    Buzzer_tone(NOTE_B5, 166);
    Buzzer_tone(NOTE_AS5, 333);
    Buzzer_tone(NOTE_AS4, 666);
    Buzzer_tone(NOTE_B4, 333);
    Buzzer_tone(NOTE_B5, 1666);
    Buzzer_tone(NOTE_G5, 333);
    Buzzer_tone(NOTE_B5, 666);
    Buzzer_tone(NOTE_G5, 333);
    Buzzer_tone(NOTE_B5, 666);
    Buzzer_tone(NOTE_G5, 333);
    Buzzer_tone(NOTE_D6, 666);
    Buzzer_tone(NOTE_CS6, 333);
    Buzzer_tone(NOTE_C6, 666);
    Buzzer_tone(NOTE_GS5, 333);
    Buzzer_tone(NOTE_C6, 500);
    Buzzer_tone(NOTE_B5, 166);
    Buzzer_tone(NOTE_AS5, 333);
    Buzzer_tone(NOTE_AS4, 666);
    Buzzer_tone(NOTE_G5, 333);
    Buzzer_tone(NOTE_E5, 1666);
}

//******* Pirate des caraïbes ****************
void Pirate(void)
{
    Buzzer_tone(NOTE_E4, 125);
    Buzzer_tone(NOTE_G4, 125);
    Buzzer_tone(NOTE_A4, 250);
    Buzzer_tone(NOTE_A4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_B4, 125);
    Buzzer_tone(NOTE_C5, 250);
    Buzzer_tone(NOTE_C5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_D5, 125);
    Buzzer_tone(NOTE_B4, 250);
    Buzzer_tone(NOTE_B4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_G4, 125);
    Buzzer_tone(NOTE_A4, 375);
    delay_us(50000);
    Buzzer_tone(NOTE_E4, 125);
    Buzzer_tone(NOTE_G4, 125);
    Buzzer_tone(NOTE_A4, 250);
    Buzzer_tone(NOTE_A4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_B4, 125);
    Buzzer_tone(NOTE_C5, 250);
    Buzzer_tone(NOTE_C5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_D5, 125);
    Buzzer_tone(NOTE_B4, 250);
    Buzzer_tone(NOTE_B4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_G4, 125);
    Buzzer_tone(NOTE_A4, 375);
    delay_us(50000);
    Buzzer_tone(NOTE_E4, 125);
    Buzzer_tone(NOTE_G4, 125);
    Buzzer_tone(NOTE_A4, 250);
    Buzzer_tone(NOTE_A4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_D5, 250);
    Buzzer_tone(NOTE_D5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_D5, 125);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_F5, 250);
    Buzzer_tone(NOTE_F5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_D5, 125);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_A4, 250);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_B4, 125);
    Buzzer_tone(NOTE_C5, 250);
    Buzzer_tone(NOTE_C5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_D5, 250);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_A4, 250);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_B4, 250);
    Buzzer_tone(NOTE_B4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_B4, 375);
    delay_us(200000);
    Buzzer_tone(NOTE_A4, 250);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_E4, 125);
    Buzzer_tone(NOTE_G4, 125);
    Buzzer_tone(NOTE_A4, 250);
    Buzzer_tone(NOTE_A4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_B4, 125);
    Buzzer_tone(NOTE_C5, 250);
    Buzzer_tone(NOTE_C5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_D5, 125);
    Buzzer_tone(NOTE_B4, 250);
    Buzzer_tone(NOTE_B4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_G4, 125);
    Buzzer_tone(NOTE_A4, 375);
    delay_us(50000);
    Buzzer_tone(NOTE_E4, 125);
    Buzzer_tone(NOTE_G4, 125);
    Buzzer_tone(NOTE_A4, 250);
    Buzzer_tone(NOTE_A4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_B4, 125);
    Buzzer_tone(NOTE_C5, 250);
    Buzzer_tone(NOTE_C5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_D5, 125);
    Buzzer_tone(NOTE_B4, 250);
    Buzzer_tone(NOTE_B4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_G4, 125);
    Buzzer_tone(NOTE_A4, 375);
    delay_us(50000);
    Buzzer_tone(NOTE_E4, 125);
    Buzzer_tone(NOTE_G4, 125);
    Buzzer_tone(NOTE_A4, 250);
    Buzzer_tone(NOTE_A4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_D5, 250);
    Buzzer_tone(NOTE_D5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_D5, 125);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_F5, 250);
    Buzzer_tone(NOTE_F5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_D5, 125);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_A4, 250);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_B4, 125);
    Buzzer_tone(NOTE_C5, 250);
    Buzzer_tone(NOTE_C5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_D5, 250);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_A4, 250);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_B4, 250);
    Buzzer_tone(NOTE_B4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_C5, 125);
    Buzzer_tone(NOTE_A4, 125);
    Buzzer_tone(NOTE_B4, 375);
    delay_us(200000);
    Buzzer_tone(NOTE_E5, 250);
    delay_us(400000);
    Buzzer_tone(NOTE_F5, 250);
    delay_us(400000);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_E5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_G5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_D5, 125);
    delay_us(400000);
    Buzzer_tone(NOTE_D5, 250);
    delay_us(400000);
    Buzzer_tone(NOTE_C5, 250);
    delay_us(400000);
    Buzzer_tone(NOTE_B4, 125);
    Buzzer_tone(NOTE_C5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_B4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 500);
    Buzzer_tone(NOTE_E5, 250);
    delay_us(400000);
    Buzzer_tone(NOTE_F5, 250);
    delay_us(400000);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_E5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_G5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_E5, 125);
    Buzzer_tone(NOTE_D5, 125);
    delay_us(400000);
    Buzzer_tone(NOTE_D5, 250);
    delay_us(400000);
    Buzzer_tone(NOTE_C5, 250);
    delay_us(400000);
    Buzzer_tone(NOTE_B4, 125);
    Buzzer_tone(NOTE_C5, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_B4, 125);
    delay_us(50000);
    Buzzer_tone(NOTE_A4, 500);
}

//******* Mario ****************
void mario(void)
{
    Buzzer_tone(NOTE_E7, 120);
    Buzzer_tone(NOTE_E7, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_E7, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_C7, 120);
    Buzzer_tone(NOTE_E7, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_G7, 120);
    delay_us(240000);
    Buzzer_tone(NOTE_G6, 120);
    delay_us(360000);
    Buzzer_tone(NOTE_C7, 120);
    delay_us(240000);
    Buzzer_tone(NOTE_G6, 120);
    delay_us(240000);
    Buzzer_tone(NOTE_E6, 120);
    delay_us(240000);
    Buzzer_tone(NOTE_A6, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_B6, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_AS6, 120);
    Buzzer_tone(NOTE_A6, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_G6, 90);
    Buzzer_tone(NOTE_E7, 90);
    Buzzer_tone(NOTE_G7, 90);
    Buzzer_tone(NOTE_A7, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_F7, 120);
    Buzzer_tone(NOTE_G7, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_E7, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_C7, 120);
    Buzzer_tone(NOTE_D7, 120);
    Buzzer_tone(NOTE_B6, 120);
    delay_us(240000);
    Buzzer_tone(NOTE_C7, 120);
    delay_us(240000);
    Buzzer_tone(NOTE_G6, 120);
    delay_us(240000);
    Buzzer_tone(NOTE_E6, 120);
    delay_us(240000);
    Buzzer_tone(NOTE_A6, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_B6, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_AS6, 120);
    Buzzer_tone(NOTE_A6, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_G6, 90);
    Buzzer_tone(NOTE_E7, 90);
    Buzzer_tone(NOTE_G7, 90);
    Buzzer_tone(NOTE_A7, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_F7, 120);
    Buzzer_tone(NOTE_G7, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_E7, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_C7, 120);
    Buzzer_tone(NOTE_D7, 120);
    Buzzer_tone(NOTE_B6, 120);
    delay_us(240000);
    Buzzer_tone(NOTE_C4, 120);
    Buzzer_tone(NOTE_C5, 120);
    Buzzer_tone(NOTE_A3, 120);
    Buzzer_tone(NOTE_A4, 120);
    Buzzer_tone(NOTE_AS3, 120);
    Buzzer_tone(NOTE_AS4, 120);
    delay_us(90000);
    Buzzer_tone(NOTE_C4, 120);
    Buzzer_tone(NOTE_C5, 120);
    Buzzer_tone(NOTE_A3, 120);
    Buzzer_tone(NOTE_A4, 120);
    Buzzer_tone(NOTE_AS3, 120);
    Buzzer_tone(NOTE_AS4, 120);
    delay_us(90000);
    Buzzer_tone(NOTE_F3, 120);
    Buzzer_tone(NOTE_F4, 120);
    Buzzer_tone(NOTE_D3, 120);
    Buzzer_tone(NOTE_D4, 120);
    Buzzer_tone(NOTE_DS3, 120);
    Buzzer_tone(NOTE_DS4, 120);
    delay_us(90000);
    Buzzer_tone(NOTE_F3, 120);
    Buzzer_tone(NOTE_F4, 120);
    Buzzer_tone(NOTE_D3, 120);
    Buzzer_tone(NOTE_D4, 120);
    Buzzer_tone(NOTE_DS3, 120);
    Buzzer_tone(NOTE_DS4, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_DS4, 180);
    Buzzer_tone(NOTE_CS4, 180);
    Buzzer_tone(NOTE_D4, 180);
    Buzzer_tone(NOTE_CS4, 60);
    Buzzer_tone(NOTE_DS4, 60);
    Buzzer_tone(NOTE_DS4, 60);
    Buzzer_tone(NOTE_GS3, 60);
    Buzzer_tone(NOTE_G3, 60);
    Buzzer_tone(NOTE_CS4, 60);
    Buzzer_tone(NOTE_C4, 180);
    Buzzer_tone(NOTE_FS4, 180);
    Buzzer_tone(NOTE_F4, 180);
    Buzzer_tone(NOTE_E3, 180);
    Buzzer_tone(NOTE_AS4, 180);
    Buzzer_tone(NOTE_A4, 180);
    Buzzer_tone(NOTE_GS4, 100);
    Buzzer_tone(NOTE_DS4, 100);
    Buzzer_tone(NOTE_B3, 100);
    Buzzer_tone(NOTE_AS3, 100);
    Buzzer_tone(NOTE_A3, 100);
    Buzzer_tone(NOTE_GS3, 100);
}

//******* IndianaJones ****************
void IndianaJones(void)
{
    Buzzer_tone(NOTE_E5, 240);
    delay_us(120000);
    Buzzer_tone(NOTE_F5, 120);
    Buzzer_tone(NOTE_G5, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_C6, 960);
    delay_us(180000);
    Buzzer_tone(NOTE_D5, 240);
    delay_us(120000);
    Buzzer_tone(NOTE_E5, 120);
    Buzzer_tone(NOTE_F5, 960);
    delay_us(360000);
    Buzzer_tone(NOTE_G5, 240);
    delay_us(120000);
    Buzzer_tone(NOTE_A5, 120);
    Buzzer_tone(NOTE_B5, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_F6, 960);
    delay_us(240000);
    Buzzer_tone(NOTE_A5, 240);
    delay_us(120000);
    Buzzer_tone(NOTE_B5, 120);
    Buzzer_tone(NOTE_C6, 480);
    Buzzer_tone(NOTE_D6, 480);
    Buzzer_tone(NOTE_E6, 480);
    Buzzer_tone(NOTE_E5, 240);
    delay_us(120000);
    Buzzer_tone(NOTE_F5, 120);
    Buzzer_tone(NOTE_G5, 120);
    delay_us(120000);
    Buzzer_tone(NOTE_C6, 960);
    delay_us(240000);
    Buzzer_tone(NOTE_D6, 240);
    delay_us(120000);
    Buzzer_tone(NOTE_E6, 120);
    Buzzer_tone(NOTE_F6, 1440);
    Buzzer_tone(NOTE_G5, 240);
    delay_us(120000);
    Buzzer_tone(NOTE_G5, 120);
    Buzzer_tone(NOTE_E6, 360);
    delay_us(120000);
    Buzzer_tone(NOTE_D6, 240);
    delay_us(120000);
    Buzzer_tone(NOTE_G5, 120);
    Buzzer_tone(NOTE_E6, 360);
    delay_us(120000);
    Buzzer_tone(NOTE_D6, 240);
    delay_us(120000);
    Buzzer_tone(NOTE_G5, 120);
    Buzzer_tone(NOTE_F6, 360);
    delay_us(120000);
    Buzzer_tone(NOTE_E6, 240);
    delay_us(120000);
    Buzzer_tone(NOTE_D6, 120);
    Buzzer_tone(NOTE_C6, 480);
}

//******* 20thCenFox ****************
void twentyCenFox(void)
{
    Buzzer_tone(NOTE_B5, 107);
    delay_us(214000);
    Buzzer_tone(NOTE_B5, 107);
    Buzzer_tone(NOTE_B5, 107);
    Buzzer_tone(NOTE_B5, 856);
    delay_us(107000);
    Buzzer_tone(NOTE_C6, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_B5, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_C6, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_B5, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_C6, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_B5, 107);
    delay_us(214000);
    Buzzer_tone(NOTE_B5, 107);
    Buzzer_tone(NOTE_B5, 107);
    Buzzer_tone(NOTE_B5, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_B5, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_B5, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_B5, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_B5, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_B5, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_B5, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_GS5, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_A5, 107);
    delay_us(53000);
    Buzzer_tone(NOTE_B5, 107);
    delay_us(214000);
    Buzzer_tone(NOTE_B5, 107);
    Buzzer_tone(NOTE_B5, 107);
    Buzzer_tone(NOTE_B5, 856);
    delay_us(428000);
    Buzzer_tone(NOTE_E5, 214);
    Buzzer_tone(NOTE_GS5, 214);
    Buzzer_tone(NOTE_B5, 214);
    Buzzer_tone(NOTE_CS6, 1712);
    Buzzer_tone(NOTE_FS5, 214);
    Buzzer_tone(NOTE_A5, 214);
    Buzzer_tone(NOTE_CS6, 214);
    Buzzer_tone(NOTE_E6, 1712);
    Buzzer_tone(NOTE_A5, 214);
    Buzzer_tone(NOTE_CS6, 214);
    Buzzer_tone(NOTE_E6, 214);
    Buzzer_tone(NOTE_E6, 1712);
    Buzzer_tone(NOTE_B5, 214);
    Buzzer_tone(NOTE_GS5, 214);
    Buzzer_tone(NOTE_A5, 214);
    Buzzer_tone(NOTE_B5, 856);
}