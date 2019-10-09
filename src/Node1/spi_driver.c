/*!@file
* SPI interface
*/
#include "spi_driver.h"
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

void spi_master_init(){
  /* Sets MCU-pins to output mode */
  DDRB |= (1<<PB4) | (1<<PB5) | (1<<PB7) ;

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
