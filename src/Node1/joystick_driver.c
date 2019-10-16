/*!@file
* Joystick interface
*/
#include "joystick_driver.h"
#include "adc_driver.h"
#include <util/delay.h>


double JOYSTICK_C_UP;		//ikke slett, skal brukes
double JOYSTICK_C_DOWN;
double JOYSTICK_C_LEFT;
double JOYSTICK_C_RIGHT;

void joystick_init(Joystick* joy){
	uint8_t x_init[JOYSTICK_INIT_NO];
	uint8_t y_init[JOYSTICK_INIT_NO];
	uint16_t x_sum = 0;
	uint16_t y_sum = 0;
	
	/*Sampling 4 times to determine initial position*/
	for (int i = 0; i < JOYSTICK_INIT_NO; i++) {
		x_init[i] = adc_read(X_axis)
		y_init[i] = adc_read(Y_axis)
		x_sum += x_init[i];
		y_sum += y_init[i];
		_delay_ms(10);
	}
  joy->neutralx = x_sum / JOYSTICK_INIT_NO;
  joy->neutraly = y_sum / JOYSTICK_INIT_NO;
  joy->x = 0;
  joy->y = 0;
  joy->dir = NEUTRAL;
}

void analog_position(Joystick* joy){
 /* JOYSTICK_C_UP = 	(255 - joy->neutraly) / 100;	//IKKE SLETT
 JOYSTICK_C_DOWN = 	  (joy->neutraly) / 100;
 JOYSTICK_C_RIGHT = 	  (joy->neutralx) / 100;
JOYSTICK_C_LEFT = (255 - joy->neutralx) / 100;
	 if (adc_read(Y_axis) > joy->neutraly) {
		joy->y = (int)(JOYSTICK_C_UP*adc_read(Y_axis));
	}
	else if (adc_read(Y_axis) <= joy->neutraly) {
		joy->y = (int)(JOYSTICK_C_DOWN*adc_read(Y_axis));
	}
	if (adc_read(X_axis) > joy->neutralx) {
		joy->x = (int)(JOYSTICK_C_RIGHT*adc_read(X_axis));
	}
	else if (adc_read(X_axis) <= joy->neutralx) {
		joy->x = (int)(JOYSTICK_C_LEFT*adc_read(X_axis));
	} */

  joy->x = (int)(JOYSTICK_CONSTANT*adc_read(X_axis) - 100) ;//Må fikse offset TODO
  joy->y = (int)(JOYSTICK_CONSTANT*adc_read(Y_axis) - 100) ;

}

void analog_direction(Joystick* joy) {
  /*Threshold on 15 percent*/
  int threshold = 15;
  /*Calculates direction based on angle*/
  double angle = atan2(joy->y,joy->x);

  if ( ((abs(joy->neutralx - joy->x)) < threshold) && ((abs(joy->neutraly - joy->y)) < threshold) ) { 
		joy->dir = NEUTRAL; 
	}
  else if (angle >= M_PI/4 && angle < 3*M_PI/4) 										{ joy->dir = UP; }
  else if (angle > -3*M_PI/4 && angle <= -M_PI/4) 									{ joy->dir = DOWN; }
  else if (angle > 3*M_PI/4 || angle <= -3*M_PI/4) 									{ joy->dir = LEFT; }
  else if (angle > -M_PI/4  && angle <= M_PI/4 ) 										{ joy->dir = RIGHT; }
}
