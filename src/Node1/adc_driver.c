/*!@file
* ADC interface
*/

#include "adc_driver.h"

volatile bool interrupt_flag = false;

/*!
  * Initialize ADC interface
  */
void adc_init(void) {

  //Disable interrupt on PE0
  GICR |= (0 << INT2);

  //Interrupt on falling edge: PE0
  EMCUCR |= (0 << ISC2);

  //Clear interrupt flag 2
  GIFR |= (1 << INTF2);

  //Enable interrupt  on PE0
  GICR |= (1 << INT2);
}

uint8_t adc_read(enum channel_type channel){		//er interrupts brukt fornuftig her ? 
  volatile char *address = (char*)0x1400;
  address[0] = channel;
  while(!(interrupt_flag));
  interrupt_flag = false;
  return address[0];
}

ISR(INT2_vect){
  interrupt_flag = true;
}
