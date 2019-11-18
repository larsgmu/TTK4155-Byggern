/*!@file
* This file cointains functions to use a PID position/speed controller on the motor.
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "motor_driver.h"
#include "pid.h"
#include "can_driver.h"
/*TUNING*/

#define DT 0.02  //our PID frequency (20ms rn)

/*PID Tuning - Normal mode*/
double Kp = 1.6;
double Kd = 1;
double Ki = 3;


static int16_t integral;
static int16_t derivative;
static int16_t prev_error;
static int16_t u;
static uint8_t ref;

void pid_init() {
  integral    = 0;
  derivative  = 0;
  prev_error  = 0;
  u           = 0;
  ref         = 0;
  /*we want this to send interrupts at our sample time frequency!*/

  /*CTC MODE*/
  TCCR3B |= (1 << WGM32);

  /*Running with x Hz*/
  OCR3AL = 255;
  OCR3AH = 255;
  /* Sets prescalar to 64*/
  TCCR3B |= (1 << CS32) | (1 << CS30);

  /*CTC INTERRUPT ENBALE*/
  TIMSK3 |= (1 << OCIE3A);

  /*Clear interrupt flag by writing 1*/
  TIFR3 |= (1 << OCF3A);
}

void pid_controller() {
    if(get_CAN_msg().id == 2) {
       ref = get_CAN_msg().data[0];
       ref                 = 255 - ref;
       uint16_t position   = motor_get_position();
       int16_t error       = ref-position;
       integral     += (uint16_t)(error*DT);
       derivative   = error - prev_error;
       prev_error   = error;
       u    = (int16_t) (Kp*error + Ki*integral + Kd*derivative);
  }
  motor_run_slider(u);
}

ISR(TIMER3_COMPA_vect) {
  pid_controller();
  TIFR3 |= (1 << OCF3A);
}
