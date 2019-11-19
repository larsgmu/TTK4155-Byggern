/*!@file
* This file contains functions to use the joystick and buttons on the game controller
*/
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

/*!
*@brief Enum cointaining the different directions of the joystick.
*/
enum joystick_dir {
  NEUTRAL   = 0,
  UP        = 1,
  DOWN      = 2,
  LEFT      = 3,
  RIGHT     = 4,
};

/*!
*@brief Sruct containing the current position, direction and center-point of the joystick.
* Values in range of [-100, 100].
*/
typedef struct Joystick_position_struct {
  int8_t  x;
  int8_t  y;
  enum    joystick_dir dir;
} Joystick_pos;

/*!
*@brief Initializes and calibrate the joystick.
* The center of the joystick is updated.
* Current direction and position is set to 0.
*/
void joystick_init();

/*!
*@brief Updates the current direction of the joystick.
@return @c uint8_t -> Returns the direction as an integer value correspondin to @c joystick_dir 
*/
uint8_t joystick_get_direction();

/*!
*@brief Constructs a CAN message with data from specified joystick and sends the message.
* X and Y position is updated from [-100,100] to [0,200].
*/
void joystick_send_pos();

#endif
