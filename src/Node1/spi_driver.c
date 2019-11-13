/*!@file
* This file contains functions to enable SPI communication between the atmega162 and MCP2515 chip.
*/
#define F_CPU 4915200

#include "spi_driver.h"
#include <avr/io.h>

void spi_master_init(){
  /* Sets MCU-pins to output mode */
  DDRB |= (1<<PB4) | (1<<PB5) | (1<<PB7) ;

  /* Enable SPI */
  SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

char spi_master_transceive(char c) {
  SPDR = c;
  while(!(SPSR & (1 << SPIF))){
  }
  return SPDR;
}
