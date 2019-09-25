#include "joystick_driver.h"
#include "adc_driver.h"


double JOYSTICK_C_UP;		//ikke slett, skal brukes 
double JOYSTICK_C_DOWN;
double JOYSTICK_C_LEFT;
double JOYSTICK_C_RIGHT;

void joystick_init(struct joystick* joy){
  joy->neutralx = adc_read(X_axis);
  joy->neutraly = adc_read(Y_axis);
  joy->x = 0;
  joy->y = 0;
  joy->dir = NEUTRAL;
}

void analog_position(struct joystick* joy){
  //Returns joystickx and y position as integers between -100 and 100
  
 // JOYSTICK_C_UP = 	(255 - joy->neutraly) / 100;			//Ikke slett
 // JOYSTICK_C_DOWN = 	  (joy->neutraly) / 100;
 // JOYSTICK_C_RIGHT = 	  (joy->neutralx) / 100;
 // JOYSTICK_C_LEFT = (255 - joy->neutralx) / 100;

  joy->x = (int)(JOYSTICK_CONSTANT*adc_read(X_axis) - 100) ;//Må fikse offset
  joy->y = (int)(JOYSTICK_CONSTANT*adc_read(Y_axis) - 100) ;
  

}

void analog_direction(struct joystick* joy) {
  //Will return NEUTRAL, UP, DOWN, LEFT or RIGHT

  int threshold = 15; //Threshold on 15 percent

  double angle = atan2(joy->y,joy->x);

  if ( ((abs(joy->x)) < threshold) && ((abs(joy->y)) < threshold) ) { joy->dir = NEUTRAL; }
  else if (angle >= M_PI/4 && angle < 3*M_PI/4) { joy->dir = UP; }
  else if (angle > -3*M_PI/4 && angle <= -M_PI/4) { joy->dir = DOWN; }
  else if (angle > 3*M_PI/4 || angle <= -3*M_PI/4) { joy->dir = LEFT; }
  else if (angle > -M_PI/4  && angle <= M_PI/4 ) { joy->dir = RIGHT; }

}
