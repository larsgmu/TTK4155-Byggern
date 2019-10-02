#ifndef MCP2515_DRIVER_H
#define MCP2515_DRIVER_H

#include <avr/io.h>
#include <stdint.h>
void mcp2515_init();
uint8_t mcp2515_read(uint8_t address);
uint8_t mcp2515_read_status();
void mcp2515_write(uint8_t address, uint8_t data);
void mcp2515_request_send(uint8_t pin);
void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void mcp2515_reset();

#endif
