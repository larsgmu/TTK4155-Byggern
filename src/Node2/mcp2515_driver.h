/*!@file
* This file contains functions to use the MCP2515 chip as a stand-alone CAN controller on Node2.
* The communication between atmega2560 and MCP2515 is done with SPI.
*/
#ifndef MCP2515_H
#define MCP2515_H

#include <avr/io.h>
#include <stdint.h>

/*!
*@brief Resets the MCP2515 and configures it to use normal mode.
*/
void mcp2515_init();

/*!
*@brief Reads the data sent from the MCP2515 at the specified address.
*@param[in] @c uint8_t address -> Address to read from.
*@return @c uint8_t -> The data read from the MCP2515.
*/
uint8_t mcp2515_read(uint8_t address);

/*!
*@brief Reads the current status of the MCP2515.
*@return @c uint8_t -> Current status.
*/
uint8_t mcp2515_read_status();

/*!
*@brief Writes the given data to the specified address.
*@param[in] @c uint8_t address -> Address to write to.
*@param[in] @c uint8_t data -> Data to write to address.
*/
void mcp2515_write(uint8_t address, uint8_t data);

/*!
*@brief Sends a request to send on specified MCP2515 pin.
*@param[in] @c uint8_t pin -> MCP2515 pin to send from.
*/
void mcp2515_request_send(uint8_t pin);

/*!
*@brief Modifies specified bits at specified adress with specified data.
*@param[in] @c uint8_t address -> Adress with data to alter.
*@param[in] @c uint8_t mask -> Chooses which of the 8 bits at adress to alter.
*@param[in] @c uint8_t data -> Data to alter specified bits with.
*/
void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data);

/*!
*@brief Resets the MCP2515.
*/
void mcp2515_reset();

#endif
