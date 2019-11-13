/*!@file
* This file cointains functions to use a PID position/speed controller on the motor.
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "motor_driver.h"
#include "pid.h"
/*TUNING*/

#define DT 0.02  //our PID frequency (20ms rn)

/*PID Tuning - Normal mode*/
double Kp = 1.6;
double Kd = 1;
double Ki = 3;


int16_t integral;
int16_t derivative;
int16_t prev_error;

void pid_init() {
  integral    = 0;
  derivative  = 0;
  prev_error  = 0;
  // we want this to send interrupts at our sample time frequency!

  /* Sets prescalar to 8*/
  TCCR3B |= (1 << CS31);
  /*Timer overflow interrupt enable*/
  TIMSK3 |= (1 << TOIE3);
  /*Clear interrupt flag by writing 1*/
  TIFR3 |= (1 << TOV3);
}

void pid_controller(uint8_t ref) {
    ref                 = 255 - ref;
    uint16_t position   = motor_get_position();
    int16_t error       = ref-position;

    integral     += (uint16_t)(error*DT);
    derivative   = error - prev_error;
    prev_error   = error;

    int16_t  u    = (int16_t) (Kp*error + Ki*integral + (Kd)*derivative);

    //printf("PID U: %d   Ref:  %d  Pos:  %d   Error: %d   \n\r", u, ref, position, error);

    motor_run_slider(u);
}

ISR(TIMER3_OVF_vect) {
  //pid_controller(ref);
}
