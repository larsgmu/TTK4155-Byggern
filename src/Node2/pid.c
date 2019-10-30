
#include "pid.h"

void pid_init() {
  cli();

  // we want this to send interrupts at our sample time frequency!

  /* Sets prescalar to 8*/
  TCCR3B |= (1 << CS31);
  /*Timer overflow interrupt enable*/
  TIMSK3 |= (1 << TOIE3);
  /*Clear interrupt flag by writing 1*/
  TIFR3 |= (1 << TOV3);

  sei();

}


void pid_controller() {
  int8_t = reference;

  position = motor_get_position();
  error = reference - position;

  integral += error;

  derivative = error - prev_error;

  prev_error = error;

  u = Kp*error + Ki*integral + Kd*derivative

}

ISR(TIMER3_OVF_vect) {
  pid_controller();
}
