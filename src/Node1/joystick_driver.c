/*!@file
* This file contains functions to use the joystick and buttons on the game controller
*/
#define F_CPU 4915200
#define JOYSTICK_THRESHOLD 5
#define JOYSTICK_CONSTANT 0.78431
#define JOYSTICK_SAMPLE_NO 4
#define JOYSTICK_OFFSET 100

#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#include "joystick_driver.h"
#include "adc_driver.h"
#include "can_driver.h"


/* This is a global variable which enables sending CAN-msg 
 * with joystick position to node 2. In order to acess from other
 * files, initialize it with extern int*/
static uint8_t prev_joy[2] = {0,0};
/*Global variables containing the actual center position of joystick*/
static int8_t  neutralx;
static int8_t  neutraly;

/*-------------------------------------------------------*/
/********Function declarations*********/

/*!
*@brief Updates current X and Y-position as integers between -100 and 100.
*@param[out] @c Joystick_ pos -> Joystick position struct.
*/
Joystick_pos joystick_get_position();

/*-------------------------------------------------------*/
/********Function implementations*********/

void joystick_init(){
	uint16_t x_sum = 0;
	uint16_t y_sum = 0;

	/*Sampling 4 times to determine initial position*/
	for (int i = 0; i < JOYSTICK_SAMPLE_NO; i++) {
		x_sum += adc_read(X_axis);
		y_sum += adc_read(Y_axis);
		_delay_ms(10);
	}
  neutralx = JOYSTICK_CONSTANT*(x_sum / JOYSTICK_SAMPLE_NO) - JOYSTICK_OFFSET;
  neutraly = JOYSTICK_CONSTANT*(y_sum / JOYSTICK_SAMPLE_NO) - JOYSTICK_OFFSET;
}

Joystick_pos joystick_get_position(){
	Joystick_pos joy_pos;
  joy_pos.x 	= (uint8_t)(JOYSTICK_CONSTANT*adc_read(X_axis) - JOYSTICK_OFFSET) ;
  joy_pos.y 	= (uint8_t)(JOYSTICK_CONSTANT*adc_read(Y_axis) - JOYSTICK_OFFSET) ;
	return joy_pos;
}

uint8_t joystick_get_direction() {
  /*Threshold on 15 percent*/
  uint8_t 			threshold = 15;
	Joystick_pos 	joy_pos		= joystick_get_position();

  /*Calculates direction based on angle*/
	int angle = (int)(180 * atan2(joy_pos.y,joy_pos.x) / M_PI);
  if ((abs(joy_pos.x - neutralx) < threshold) && (abs(joy_pos.y - neutraly) < threshold))  {
		return NEUTRAL;
	}
	else if (angle < 135 		&& angle >= 45) 						{ return UP; }
	else if (angle < 45  		&& angle >= -45) 						{ return RIGHT; }
	else if (angle < -45 		&& angle >= -135) 					{ return DOWN; }
	else if (angle <= -135 	|| angle >= 135) 						{ return LEFT; }
	return NEUTRAL;
}

void joystick_send_pos(){
	Joystick_pos joy_pos = joystick_get_position();
	CANmsg joystick_msg;
	joystick_msg.id 		= 1;
	joystick_msg.length	= 2;
	if ((abs(joy_pos.x + 100 - prev_joy[0]) > JOYSTICK_THRESHOLD) || (abs(joy_pos.y + 100 - prev_joy[1]) > JOYSTICK_THRESHOLD)) {
		joystick_msg.data[0] 	= (uint8_t)joy_pos.x + 100;
		prev_joy[0] 					= (uint8_t)joy_pos.x + 100;
		joystick_msg.data[1] 	= (uint8_t)joy_pos.y + 100;
		prev_joy[1] 					= (uint8_t)joy_pos.y + 100;
		can_send_msg(&joystick_msg);
	}
}

