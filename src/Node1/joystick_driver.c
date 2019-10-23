/*!@file
* Joystick interface
*/
#include <util/delay.h>
#include "joystick_driver.h"
#include "adc_driver.h"
#include "can_driver.h"

/*This is a global variable which enables sending CAN-msg
  with joystick position to node 2. In order to acess from other
	files, initialize it with extern int*/
extern int Jayballs;
int Jayballs = 1;

void joystick_init(Joystick* joy){
	uint16_t x_sum = 0;
	uint16_t y_sum = 0;

	/*Sampling 4 times to determine initial position*/
	for (int i = 0; i < JOYSTICK_SAMPLE_NO; i++) {
		x_sum += adc_read(X_axis);
		y_sum += adc_read(Y_axis);
		_delay_ms(10);
	}
  joy->neutralx = JOYSTICK_CONSTANT*(x_sum / JOYSTICK_INIT_NO) - JOYSTICK_OFFSET;
  joy->neutraly = JOYSTICK_CONSTANT*(y_sum / JOYSTICK_INIT_NO) - JOYSTICK_OFFSET;
	joy->x = 0;
  joy->y = 0;
  joy->dir = NEUTRAL;
}

void analog_position(Joystick* joy){
  joy->x = (int)(JOYSTICK_CONSTANT*adc_read(X_axis) - JOYSTICK_OFFSET) ;//Må fikse offset TODO
  joy->y = (int)(JOYSTICK_CONSTANT*adc_read(Y_axis) - JOYSTICK_OFFSET) ;

}

void analog_direction(Joystick* joy) {
  /*Threshold on 10 percent*/
  int threshold = 10;

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
	_delay_ms(10);
	CANmsg joystick_pos_msg;
	joystick_pos_msg.id = 1;
	joystick_pos_msg.length = 2;
	joystick_pos_msg.data[0] = (uint8_t)joy->x + 100;
	joystick_pos_msg.data[1] = (uint8_t)joy->y + 100;
	can_send_msg(&joystick_pos_msg);
}

void joystick_run(Joystick* joy) {
	analog_position(joy);
	analog_direction(joy);
	if (Jayballs) {
		send_joystick_pos(joy);
	}
}
