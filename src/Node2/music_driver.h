/*!@file
* This file contains functions to play music from the piezo buzzer on the
* USB Multifuncion card by sending PWM signals from the atmega2560.
*/
#ifndef MUSIC_DRIVER_H
#define MUSIC_DRIVER_H
#include <stdint.h>

typedef enum {MARIO = 0, UNDERWORLD = 1, SOVJET = 2}song;

/*!
*@brief Function to initialize the music driver
*/
void music_init();

/*!
*@brief Function to play music.
*/
void music_play(song title);


/*!
*@brief Function to make the piezo buzzer buzz.
*@param[in] @c uint16_t freq -> Frequency of note to be played.
*@param[in] @c uint8_t data -> Lenght of note.
*/
void music_buzzer(float freq, int length);
void set_tone(float freq);


#endif
