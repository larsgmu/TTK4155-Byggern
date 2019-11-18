/*!@file
* This file contains functions to play music from the piezo buzzer on the
* USB Multifuncion card by sending PWM signals from the atmega2560.
*/
#ifndef MUSIC_DRIVER_H
#define MUSIC_DRIVER_H

typedef enum {MARIO = 0, UNDERWORLD = 1, SOVJET = 2}song;

/*!
*@brief Function to initialize the music driver
*/
void music_init();

/*!
*@brief Function to play music.
*/
void music_play(song title);


#endif
