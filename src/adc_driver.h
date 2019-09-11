#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>


enum channel_type{
  Y_axis = 4,
  X_axis = 5,
};


void adc_init(void);
uint8_t adc_read(enum channel_type channel);

#endif
