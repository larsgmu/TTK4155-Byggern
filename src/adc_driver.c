#include "adc_driver.h"

volatile bool interrupt_flag = false;

void adc_init(void) {
  //Interrupt on falling edge: PE0
  EMCUCR |= (0 << ISC2);

  //Clear interrupt flag 2
  GIFR |= (1 << INTF2);

  //Enable interrupt  on PE0
  GICR |= (1 << INT2);
}

uint8_t adc_read(enum channel_type channel){
  volatile char *address = (char*)0x1400;
  address[0] = channel;
  printf("Reading from mux %d, ", channel);
  // while(!(interrupt_flag));
  // interrupt_flag = false;
  _delay_ms(80);

  uint8_t ADC_read = address[0];

  printf("ADC read: %d \n\r", ADC_read);
  return address[0];
}

ISR(INT2_vec){
  interrupt_flag = true;
}
