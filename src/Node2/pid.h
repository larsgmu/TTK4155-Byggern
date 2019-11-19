/*!@file
* This file cointains functions to use a PID position/speed controller on the motor.
*/

#ifndef PID_H
#define PID_H

/*!
*@brief Enables timer and interrupt.
*/
void pid_init();

/*!
*@brief PID controller.
*@return @c int -> PID-controlled motor voltage.
*@param[in] @c uint8_t ref -> Reference position, from 0 to 255.
*/
void pid_controller();

/*!
*@brief Change Difficulty of the ping pong game by altering regulator paramters
*@param[in] @c uint8_t difficulty -> Either Dragvoll(0) or NTH(1).
*/
void pid_set_difficulty(uint8_t difficulty);

#endif
