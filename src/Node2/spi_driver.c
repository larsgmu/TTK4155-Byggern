#include "spi_driver.h"
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

void spi_master_init(){
  /* Sets MCU-pins to output mode . Dette er andre pins enn i N1*/
  DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB7); //The arduino defines SS as PB7

  /* Enable SPI */
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

char spi_master_transceive(char c) {
  SPDR = c;
  while(!(SPSR & (1 << SPIF))){
    //_delay_ms(100);
  }
  return SPDR;
}
