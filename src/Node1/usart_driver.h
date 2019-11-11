/*!@file
* This file include functions to communicate with USART between the atmega162and a computer.
* The communication is done via a RS232 chip.
*/
#ifndef UART__H__
#define UART__H__
#include <avr/io.h>
#include <stdio.h>

/*!
*@brief Sets the baudrate, enables receiver and transmitter
@param[in] @c unsigned int ubrr -> Value to set baudrate correct.
*/
void usart_init( unsigned int ubrr );

/*!
*@brief Sends specified data.
*@param[in] @c unsigned char data -> Data to send.
*/
void usart_transmit( unsigned char data );


/*!
*@brief Receives data.
*@return @c unsigned char -> Data received.
*/
unsigned char usart_receive( void );

/*!
*@brief Enables the use of printf().
*/
void string_init();


#endif
