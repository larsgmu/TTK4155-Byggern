/*!@file
* This file contains functions to play the pingpong game including the IR-sensor.
*/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include "can_driver.h"
#include <stdint.h>



/*!
*@brief Sets the servo dutycycle from joystick input.
*@param[in] @c uint8_t pos_msg -> Joystick Y position.
*/
void servo_joystick_control(uint8_t pos_msg);

/*!
*@brief Enables ADC on the atmega2560, interrupts and configuers pins.
*/
void ir_adc_init();

/*!
*@brief Reads the IR sensor with digital filtering.
*@return @c uint16_t -> Digital IR-sensor value.
*/
uint16_t ir_adc_read();

/*!
*@brief Controls the servo, motor and solenoid from joystick CAN message from Node 1.
* Sends a stop pingpong CAN message when the ball crosses the line.
*/
void play_pingpong();

/*!
*@brief Configures the atmega2560 pins.
*/
void solenoid_init();

/*!
*@brief Extends the solenoid.
*/
void solenoid_extend();


#endif
