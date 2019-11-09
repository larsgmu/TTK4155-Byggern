/*!@file
* USART interface
*/
#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include "usart_driver.h"

void usart_init( unsigned int ubrr ){
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

}

void usart_transmit( unsigned char data){
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = data;

}

unsigned char usart_receive( void ){
    /* Wait for data to be received */
    while ( !(UCSR0A & (1<<RXC0)) );
    /* Get and return received data from buffer */
    return UDR0;
}

void string_init(){

    usart_init ( MYUBRR );
    fdevopen(usart_transmit, usart_receive); //Make a stream between terminal and t/r functions
}
