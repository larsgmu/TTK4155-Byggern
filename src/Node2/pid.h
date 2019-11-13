/*!@file
* This file cointains functions to use a PID position/speed controller on the motor.
*/

#ifndef PID_H
#define PID_H
#include <stdint.h>


/*!
*@brief Enables timer and interrupt.
*/
void pid_init();


/*!
*@brief PID controller.
*@return @c int -> PID-controlled motor voltage.
*@param[in] @c uint8_t ref -> Reference position, from 0 to 255.
*/
void pid_controller(uint8_t ref);

#endif
