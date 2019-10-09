#ifndef UART__H__
#define UART__H__

void usart_init( unsigned int ubrr );

void usart_transmit( unsigned char data );

unsigned char usart_receive( void );

void string_init();


#endif
