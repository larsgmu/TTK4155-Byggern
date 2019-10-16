/*!@file
* Joystick interface
*/
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdlib.h>
#include <math.h>

#define JOYSTICK_CONSTANT 0.78431
#define JOYSTICK_INIT_NO 4

enum joystick_dir {
  NEUTRAL = 0,
  UP = 1,
  DOWN = 2,
  LEFT = 3,
  RIGHT = 4,
};

/*Joystick-struct containing current position, direction and center-point*/
typedef struct Joystick_struct {
  int x;
  int y;
  int neutralx;
  int neutraly;
  enum joystick_dir dir;
} Joystick;

/*initializing and calibrating*/
void joystick_init(Joystick* joy);

/*Sets current joystick X and Y-position as integers between -100 and 100*/
void analog_position(Joystick* joy);

/*Sets current direction of Joystick as an enum*/
void analog_direction(Joystick* joy);

#endif
