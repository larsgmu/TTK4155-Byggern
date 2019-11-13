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
//#include "slider_driver.h"

/*This is a global variable which enables sending CAN-msg
  with joystick position to node 2. In order to acess from other
	files, initialize it with extern int*/

uint8_t prev_joy[2] = {0,0};


void joystick_init(Joystick* joy){
	uint16_t x_sum = 0;
	uint16_t y_sum = 0;

	/*Sampling 4 times to determine initial position*/
	for (int i = 0; i < JOYSTICK_SAMPLE_NO; i++) {
		x_sum += adc_read(X_axis);
		y_sum += adc_read(Y_axis);
		_delay_ms(10);
	}
  joy->neutralx = JOYSTICK_CONSTANT*(x_sum / JOYSTICK_SAMPLE_NO) - JOYSTICK_OFFSET;
  joy->neutraly = JOYSTICK_CONSTANT*(y_sum / JOYSTICK_SAMPLE_NO) - JOYSTICK_OFFSET;
	joy->x = 0;
  joy->y = 0;
  joy->dir = NEUTRAL;
}

void analog_position(Joystick* joy){
  joy->x = (int)(JOYSTICK_CONSTANT*adc_read(X_axis) - JOYSTICK_OFFSET) ;//Må fikse offset TODO
  joy->y = (int)(JOYSTICK_CONSTANT*adc_read(Y_axis) - JOYSTICK_OFFSET) ;

}

void analog_direction(Joystick* joy) {
  /*Threshold on 15 percent*/
  int threshold = 15;

  /*Calculates direction based on angle*/
  double anglerad = atan2(joy->y,joy->x);
	int angle = 180 * anglerad / M_PI;

  if ((abs(joy->x - joy->neutralx) < threshold) && (abs(joy->y - joy->neutraly) < threshold))  {
		joy->dir = NEUTRAL;
	}
	else if (angle < 135 		&& angle >= 45) 						{ joy->dir = UP; }
	else if (angle < 45  		&& angle >= -45) 						{ joy->dir = RIGHT; }
	else if (angle < -45 		&& angle >= -135) 					{ joy->dir = DOWN; }
	else if (angle <= -135 	|| angle >= 135) 						{ joy->dir = LEFT; }
}

void send_joystick_pos(Joystick* joy){
	CANmsg joystick_msg;
	joystick_msg.id = 1;
	joystick_msg.length = 2;
	if ((abs(joy->x + 100 - prev_joy[0]) > JOYSTICK_THRESHOLD) || (abs(joy->y + 100 - prev_joy[1]) > JOYSTICK_THRESHOLD)) {
		joystick_msg.data[0] = (uint8_t)joy->x + 100;
		prev_joy[0] = (uint8_t)joy->x + 100;
		joystick_msg.data[1] = (uint8_t)joy->y + 100;
		prev_joy[1] = (uint8_t)joy->y + 100;
		can_send_msg(&joystick_msg);
		//printf("CAN MSG SENT JOYSTICK \n\r");
	}
}

void joystick_run(Joystick* joy) {
	analog_position(joy);
	analog_direction(joy);
}
