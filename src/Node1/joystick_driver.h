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
  NEUTRAL = 0,
  UP = 1,
  DOWN = 2,
  LEFT = 3,
  RIGHT = 4,
};

/*!
*@brief Sruct containing the current position, direction and center-point of the joystick.
*/
typedef struct Joystick_struct {
  int16_t x;
  int16_t y;
  int16_t neutralx;
  int16_t neutraly;
  enum joystick_dir dir;
} Joystick;

/*!
*@brief Initializes and calibrate the joystick.
* The center of the joystick is updated.
* Current direction and position is set to 0.
*@param[in] @c Joystick* joy -> Pointer to game controller joystick struct.
*/
void joystick_init(Joystick* joy);

/*!
*@brief Updates current X and Y-position as integers between -100 and 100.
*@param[in] @c Joystick* joy -> Pointer to game controller joystick struct.
*/
void analog_position(Joystick* joy);

/*!
*@brief Updates the current direction of the joystick.
*@param[in] @c Joystick* joy -> Pointer to game controller joystick struct.
*/
void analog_direction(Joystick* joy);

/*!
*@brief Constructs a CAN message with data from specified joystick and sends the message.
* X and Y position is updated from [-100,100] to [0,200].
*@param[in] @c Joystick* joy -> Pointer to game controller joystick struct.
*/
void send_joystick_pos(Joystick* joy);

/*!
*@brief Global function to update joystick data.
*@param[in] @c Joystick* joy -> Pointer to game controller joystick struct.
*/
void joystick_run(Joystick* joy);

#endif
