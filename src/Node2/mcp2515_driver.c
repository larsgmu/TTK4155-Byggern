/*!@file
* This file contains functions to use the MCP2515 chip as a stand-alone CAN controller on Node2.
* The communication between atmega2560 and MCP2515 is done with SPI.
*/

#include <avr/io.h>

#include "MCP2515.h"
#include "mcp2515_driver.h"
#include "spi_driver.h"


void mcp2515_init() {
  spi_master_init();
  mcp2515_reset();
  mcp2515_write(MCP_CANCTRL, MODE_NORMAL);
}

uint8_t mcp2515_read(uint8_t address) {
  /*Set slave select low*/
  PORTB &= ~(1 << PB7);

  /*Send read instruction*/
  spi_master_transceive(MCP_READ);

  /*Send address*/
  spi_master_transceive(address);

  /*receive data*/
  uint8_t data;
  data = spi_master_transceive(0xFF);

  /*CS High*/
  PORTB |= (1 << PB7);
  return data;
}

void mcp2515_write(uint8_t address, uint8_t data) {
  /*Slave select low*/
  PORTB &= ~(1 << PB7);

  /*Send write instr */
  spi_master_transceive(MCP_WRITE);
  spi_master_transceive(address);
  spi_master_transceive(data);

  /*Slave Select High*/
  PORTB |= (1 << PB7);
}

void mcp2515_request_send(uint8_t pin) {
  /* Slave select low*/
  PORTB &= ~(1 << PB7);

  switch(pin) {
    case (0):
      spi_master_transceive(MCP_RTS_TX0);
      break;
    case (1):
      spi_master_transceive(MCP_RTS_TX1);
      break;
    case (2):
      spi_master_transceive(MCP_RTS_TX2);
      break;
    case (3):
      spi_master_transceive(MCP_RTS_ALL);
      break;
    default:
      break;
  }
  /* Slave select high */
  PORTB |= (1 << PB7);
}

uint8_t mcp2515_read_status() {
  return mcp2515_read(MCP_READ_STATUS);
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
  /* Slave select low */
  PORTB &= ~(1 << PB7);

  spi_master_transceive(MCP_BITMOD);
  spi_master_transceive(address);
  spi_master_transceive(mask);
  spi_master_transceive(data);

  /*Slave select high*/
  PORTB |= (1 << PB7);
}

void mcp2515_reset() {
  /*Set slave select low*/
  PORTB &= ~(1 << PB7);
  /*Sends reset-command*/
  spi_master_transceive(MCP_RESET);
  /*Set slave select high*/
  PORTB |= (1 << PB7);
}
