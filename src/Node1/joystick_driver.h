/*!@file
* Joystick interface
*/
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdlib.h>
#include <math.h>

#define JOYSTICK_CONSTANT 0.78431
#define JOYSTICK_SAMPLE_NO 4
#define JOYSTICK_OFFSET 100



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

/*Sends CANmsg with joystick position, ID = 1*/
void send_joystick_pos(Joystick* joy);

/*General joystick function. Sends message if condition = true */
void joystick_run(Joystick* joy);

/*Check if left button on Gameboard is pressed*/
int8_t left_button_pressed();

/*Check if right button on Gameboard is pressed*/
int8_t right_button_pressed();
#endif
