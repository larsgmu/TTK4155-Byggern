/*!@file
* This file contains functions to use fastPWM for the servo.
*/

#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

/*!
*@brief Enables fastPWM, interrupts and configures atmega2560 pins.
*/
void timer_init();

/*!
*@brief Sets the dutycycle of the pwm signal that goes to the servo.
*@param[in] @c uint16_t cycle -> Dutycycle to send to servo.
*/
void set_duty_cycle(uint16_t cycle);

#endif
