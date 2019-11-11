/*!@file
* This file cointains functions to controll the motor.
*/

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

/*!
*@brief Configures the atmega2560 pins and enables the motor.
*/
void motor_init();

/*!
*@brief Sets the direction and speed of the motor.
*@param[in] @c uint8_t val -> Value sent from joystick x position to control speed and direction of motor.
*/
void motor_run(uint8_t val);

/*!
*@brief Returns the current position of the motor.
*/
void motor_get_position();

/*!
*@brief Reads the encoder value.
*@return @c int16_t -> Encoder value.
*/
int16_t encoder_read();


/*!
*@brief Resets the encoder.
*/
void reset_encoder();

#endif
