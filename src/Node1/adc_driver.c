/*!@file
* This file contains functions to convert analog signals from the game controller to digital values for the atmega162.
* The conversion is done by an ADC0844CCN chip. Node1.
*/
#define F_CPU 4915200

#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc_driver.h"

volatile static uint8_t adc_FLAG = 0;

/*!
  * Initializes ADC interface with interrupts in PE0 (INT2)
*/
void adc_init(void) {

  /*Disable interrupt on PE0*/
  GICR |= (0 << INT2);

  /*Interrupt on falling edge*/
  EMCUCR |= (0 << ISC2);

  /*Clear interrupt flag 2*/
  GIFR |= (1 << INTF2);

  /*Enable interrupt on PE0*/
  GICR |= (1 << INT2);


/*Flytte disse til en annen fil kanskje?*/
  //Setup PB2 as input for right button on game controller
  DDRB &= ~(1 << PB2);

  /*Setup PB3 as input for left button on game controller*/
  DDRB &= ~(1 << PB3);
}

uint8_t adc_read(enum channel_type channel){
  volatile char *address = (char*)0x1400;
  address[0] = channel;
  while(!(adc_FLAG));
  adc_FLAG = 0;
  return address[0];
}

ISR(INT2_vect){
  adc_FLAG = 1;
}
