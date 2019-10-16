#include <avr/io.h>
#include "timer_driver.h"
#include <avr/interrupt.h>
#define minPWM 1800
#define maxPWM 4200

void timer_init(){

  /*Sets Waveform gen mode to  14 and clock to prescalar 8*/
  TCCR1A |= (1 << COM1A1) | (1 << WGM11);
  TCCR1A &= ~(1 << WGM10);
  TCCR1A &= ~(1 << COM1A0);
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);

  /*Our desired TOP*/
  ICR1 = 40000; //PWM resolutino = lo

  /*Set dutycycle*/
  OCR1A = 3000;

  /*Enable interrupt*/
  TIMSK1 |= (1 << OCIE1A);

  /*Clear interrupt flag*/
  TIFR1 = 0;

  /*Set PB5 as output*/
  DDRB |= (1 << PB5);
}

void set_duty_cycle(uint16_t cycle) {
  if ((cycle >= minPWM) && (cycle <= maxPWM)){
    OCR1A = cycle;
    printf("Horer\n\r");
  }
  else {
    printf("Invalid dutycycle!\n\r");
  }
}

ISR(TIMER1_COMPA_vect) {
  //printf("Interrupt vector!\n\r");
}

/*
TCNT0(timer/counter) og OCR0A og B(Output compare)  er 8-bit.

Alle interrupts request signals er synlige i  Timer Interrupt Flag Register (TIFR0)
og er individuelt masked med Timer Interrupt Mask Register (TIMSK0).

The clock source is selected by theClock Select logic which is controlled by
the Clock Select (CS02:0) bits located in the Timer/Counter Control Reg-ister (TCCR0B).

*/
