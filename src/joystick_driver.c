#include "joystick_driver.h"

void joystick_init(struct joystick* joy){
  joy->neutralx = adc_read(X_axis);
  joy->neutraly = adc_read(Y_axis);
  joy->x = 0;
  joy->y = 0;
  joy->dir = NEUTRAL;
}

void analog_position(struct joystick* joy){
  //Returns joystick x and y position as integers between -100 and 100
  joy->x = 222;//(adc_read(X_axis)-joy->neutralx)*JOYSTICK_CONSTANT - 100;
  joy->y = round((adc_read(Y_axis)-joy->neutraly)*JOYSTICK_CONSTANT); //Stemmer ganske bra, men konstanten påvirker joy->x av en eller annen rar grunn!!! TODO

}

void analog_direction(struct joystick* joy) {
  //Will return NEUTRAL, UP, DOWN, LEFT or RIGHT

  int threshold = 5; //Threshold on 5 percent

  // if ( ( ((abs(joy->x)) - abs(joy->neutralx)) < threshold ) && ( (abs(joy->y)) - abs(joy->neutraly)) < threshold ) {
  //   joy->dir =  NEUTRAL;
  //   return;
  // }
  double angle = atan2(joy->y,joy->x);

  // if (angle >= M_PI/4 && angle < 3*M_PI/4) {  joy->dir = UP;  }
  // if (angle > -3*M_PI/4 && angle <= -M_PI/4) { joy->dir = DOWN; }
  // else if (angle > 3*M_PI/4 || angle <= -3*M_PI/4) { joy->dir = LEFT; }
  // else if (angle > -M_PI/4  && angle <= M_PI/4 ) { joy->dir = RIGHT; }

}
