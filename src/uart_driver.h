#ifndef UART__H__
#define UART__H__
#include <avr/io.h>
#include <stdio.h>


void USART_Init( unsigned int ubrr );

void USART_Transmit( unsigned char data );

unsigned char USART_Receive( void );

void string_init();


#endif
