/*!@file
* This file contains functions to use fastPWM for the servo.
*/
#define PWM_MIN 1800
#define PWM_MAX 4200

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer_driver.h"


void timer_init(){

  /*Sets Waveform gen mode to  14 and clock to prescalar 8*/
  TCCR1A |= (1 << COM1B1) | (1 << WGM11); // COM1C0 = 0 implisitt for non-inverting (C = pin7)
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);

  /*Our desired TOP*/
  ICR1 = 40000; //PWM resolutino = lo

  /*Set dutycycle*/
  OCR1B = 3000;

  /*Enable interrupt*/
  TIMSK1 |= (1 << OCIE1A);

  /*Clear interrupt flag*/
  TIFR1 |= (1 << OCF1A);

  /*Set PB6 as output*/
  DDRB |= (1 << PB6);

}

void set_duty_cycle(uint16_t cycle) {
  if ((cycle >= PWM_MIN) && (cycle <= PWM_MAX)){
    OCR1B = cycle;
  }
}

ISR(TIMER1_COMPA_vect) {
  /*Clear interrupt flag (if necessary)*/
  TIFR1 |= (1 << OCF1A);
}
