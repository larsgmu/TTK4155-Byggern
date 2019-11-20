/*!@file
* This file cointains functions to use a PID position/speed controller on the motor.
*/
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

#include "motor_driver.h"
#include "pid.h"
#include "can_driver.h"
#include <util/delay.h>
/*TUNING*/

#define DT 0.03  //our PID frequency

/*PID Tuning - Default is NTH(Hard)*/
double Kp = 1;
double Kd = 1;
double Ki = 3;

static int16_t integral;
static int16_t derivative;
static int16_t prev_error;
static int16_t u;
static uint8_t ref;

/*-------------------------------------------------------*/
/********Function implementations*********/

void pid_init() {
  integral    = 0;
  derivative  = 0;
  prev_error  = 0;
  u           = 0;
  ref         = 0;
  /*we want this to send interrupts at our sample time frequency!*/

  /*CTC MODE*/
  TCCR4B |= (1 << WGM42);

  /* Sets prescalar to 1024*/
  TCCR4B |= (1 << CS42) | (1 << CS40);

  /*Running with 33 Hz*/
  OCR4A = 468;
  TCNT4 = 0;

  /*Clear interrupt flag by writing 1*/
  TIFR4 |= (1 << OCF4A);

  /*CTC INTERRUPT ENBALE*/
  TIMSK4 |= (1 << OCIE4A);

}

void pid_controller() {
    if (get_CAN_msg().id == 2) {
       ref = get_CAN_msg().data[0];
       ref = 255 - ref;
       uint16_t position   = motor_get_position();
       int16_t error       = ref-position;
       integral     += (uint16_t)(error*DT);
       derivative   = error - prev_error;
       prev_error   = error;
       u = (int16_t) (Kp*error + Ki*integral + Kd*derivative);
  }
  motor_run_slider(u);
}

void pid_set_difficulty(uint8_t difficulty) {
  if (difficulty == 1) {
      Kp = 1;
      Kd = 1;
      Ki = 3;
  }
  else if (difficulty == 0) {
      Kp = 0.7;
      Kd = 2;
      Ki = 3;
  }
}

ISR(TIMER4_COMPA_vect) {
  pid_controller();
  TIFR4 |= (1 << OCF4A);
  TCNT4 = 0;
}
