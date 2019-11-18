/*!@file
* This file contains functions to play the pingpong game including the IR-sensor.
*/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

/*!
*@biref Current game init
*/
void game_board_init();

/*!
*@brief Controls the servo, motor and solenoid from joystick CAN message from Node 1.
* Sends a stop pingpong CAN message when the ball crosses the line.
*/
void play_pingpong();

/*!
*@brief Enables ADC on the atmega2560, interrupts and configuers pins.
*/
void ir_adc_init();

/*!
*@brief Configures the atmega2560 pins.
*/
void solenoid_init();


#endif
