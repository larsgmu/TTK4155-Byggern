#ifndef UART__H__
#define UART__H__
#include <avr/io.h>
#include <stdio.h>


extern FILE* uart;

void USART_Init( unsigned int ubrr );

void USART_Transmit( unsigned char data );

unsigned char USART_Receive( void );


#endif
