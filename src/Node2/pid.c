/*!@file
* This file cointains functions to use a PID position/speed controller on the motor.
*/

#include "pid.h"
#include "can_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "motor_driver.h"
/*TUNING*/

double Kp = 0.8;
double Kd = 0.05;
double Ki = 0.5;

uint8_t count = 0;



int16_t integral = 0;
int16_t derivative = 0;
int16_t prev_error = 0;

void pid_init() {
  // we want this to send interrupts at our sample time frequency!

  /* Sets prescalar to 8*/
  TCCR3B |= (1 << CS31);
  /*Timer overflow interrupt enable*/
  TIMSK3 |= (1 << TOIE3);
  /*Clear interrupt flag by writing 1*/
  TIFR3 |= (1 << TOV3);
}

void pid_controller(uint8_t ref) {
    ref = 255 - ref;
    uint16_t position  = motor_get_position();
    int16_t error     = ref-position;

    count ++;
    if ((-7 < error && error < 7) || (count == 40)) {
      integral = 0;
      count = 0;
    }

    integral     += error;
    derivative   = error - prev_error;
    prev_error   = error;

    int16_t  u    = (int8_t) (Kp*error + Ki*integral + Kd*derivative);

    printf("PID U: %d   Ref:  %d  Pos:  %d   Error: %d   \n\r", u, ref, position, error);
    if (u < 255 && u > -255) {
      motor_run_slider(u);
    }

}

ISR(TIMER3_OVF_vect) {
  //pid_controller(ref);
}
