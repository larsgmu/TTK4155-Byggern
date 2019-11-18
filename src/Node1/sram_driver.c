/*!@file
* This file contains functions to use the IDT_7164SL chip as external SRAM for the atmega162.
*/
#include "sram_driver.h"
#include <avr/io.h>


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

void sram_test(){
  volatile char *ext_ram = (char *) 0x1800;
  // Start address for the SRAM
  uint16_t ext_ram_size= 0x800;
  uint16_t write_errors= 0;
  uint16_t retrieval_errors= 0;
  printf("Starting SRAM test...\n");
  // rand() stores some internal state, so calling this function in a loop will
  // yield different seeds each time (unless srand() is called before this function)
  uint16_t seed = rand();// Write phase: Immediately check that the correct value was stored
  srand(seed);
  for (uint16_t i = 0; i < ext_ram_size; i++) {
    uint8_t some_value = rand();
    ext_ram[i] = some_value;
    uint8_t retreived_value = ext_ram[i];
    if (retreived_value != some_value) {
      printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
      write_errors++;
    }
  }
  // Retrieval phase: Check that no values were changed during or after the write phase
  srand(seed);// reset the PRNG to the stateit had before the write phase
  for (uint16_t i = 0; i < ext_ram_size; i++) {
    uint8_t some_value = rand();
    uint8_t retreived_value = ext_ram[i];
    if (retreived_value != some_value) {
      printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
      retrieval_errors++;
    }
  }
  printf("SRAM test completed with \r\n%4d errors in write phase and \r\n%4d errors in retrieval phase\r\n\n", write_errors, retrieval_errors);
}
