//#define F_CPU 1000000UL
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
//#include <util/delay.h>
#include "uart.h"




//Excercise 6 day one
/*void squareSignalTest() {

    DDRB   |= (1 << PB0);           // LED on pin PB2

    while(1)
    {
        _delay_ms(50);              // 50ms delay
        PORTB &= ~(1 << PB2);       // LED off
        _delay_ms(50);              // 50ms delay
        PORTB |=  (1 << PB2);       // LED on
    }

}*/

void main( void ){

    USART_Init ( MYUBRR );
    fdevopen(USART_Transmit, USART_Receive); //Make a stream between terminal and t/r functions
    while(1) USART_Transmit(USART_Receive());

}
