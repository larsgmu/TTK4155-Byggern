/*!@file
* This file conatins fucntions to write to the EEPROM of the atmega162
*/

#include "eeprom_driver.h"
#include <avr/io.h>

/*-------------------------------------------------------*/
/********Function implementations*********/

void eeprom_write(unsigned int ui_address, unsigned char uc_data){
  /* Wait for completion of previous write */
  while (EECR & (1<<EEWE));
  /* Set up address and data registers */
  EEAR = ui_address;
  EEDR = uc_data;
  /* Write logical one to EEMWE */
  EECR |= (1<<EEMWE);
  /* Start eeprom write by setting EEWE */
  EECR |= (1<<EEWE);
}

unsigned char eeprom_read(unsigned int ui_address){
  /* Wait for completion of previous write */
  while (EECR & (1<<EEWE));
  /* Set up address register */
  EEAR = ui_address;
  /* Start eeprom read by writing EERE */
  EECR |= (1<<EERE);
  /* Return data from data register */
  return EEDR;
}
