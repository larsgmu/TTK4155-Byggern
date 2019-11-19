/*!@file
* This file contains functions to use the IDT_7164SL chip as external SRAM for the atmega162.
*/
#include <avr/io.h>
#include "sram_driver.h"

/*-------------------------------------------------------*/
/********Function implementations*********/

/*!
*@brief Writes specified data to given address.
*@param[in] @c unsigned int adr -> Address to write data to.
*@param[in] @c usigned int data -> Data to write to address.
*/
void sram_write(unsigned int adr, unsigned int data) {
    volatile char* external_ram = (char *) 0x1800;
    external_ram[adr] = data;
}
/*!
*@brief Read data from specified address.
*@param[in] @c unsigned int adr -> Address to read from.
*@return @c usigned int -> Data read from address.
*/
unsigned int sram_read(unsigned int adr) {
    volatile char* external_ram = (char *) 0x1800;
    return external_ram[adr];
}

void sram_init(){
    /*Enable external SRAM*/
    MCUCR |= (1 << SRE);
    /*USE PC7-PC4 as normal*/
    SFIOR |= (1 << XMM2);
}
