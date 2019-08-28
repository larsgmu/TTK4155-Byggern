#define F_CPU 1000000UL
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>




void USART_Init( unsigned int ubrr ){
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit( unsigned char data ){
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

unsigned char USART_Receive( void ){
    /* Wait for data to be received */
    while ( !(UCSR0A & (1<<RXC0)) );
    /* Get and return received data from buffer */
    return UDR0;
}

//Excercise 6 day one
void squareSignalTest() {

    DDRB   |= (1 << PB0);           // LED on pin PB2

    while(1)
    {
        _delay_ms(50);              // 50ms delay
        PORTB &= ~(1 << PB2);       // LED off
        _delay_ms(50);              // 50ms delay
        PORTB |=  (1 << PB2);       // LED on
    }

}

void main( void ){

    USART_Init ( MYUBRR );
    while(1)USART_Transmit('a');

    //fdevopen(USART_Transmit, USART_Receive);
}
