/*!@file
* This file include functions to communicate with USART between the atmega162and a computer.
* The communication is done via a RS232 chip.
*/
#ifndef UART__H__
#define UART__H__

/*!
*@brief Sets the baudrate, enables receiver and transmitter
@param[in] @c unsigned int ubrr -> Value to set baudrate correct.
*/
void usart_init(unsigned int ubrr);

/*!
*@brief Enables the use of printf().
*/
void string_init();

#endif
