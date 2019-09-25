#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include "uart_driver.h"

void USART_Init( unsigned int ubrr ){
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

}

void USART_Transmit( unsigned char data){
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

void string_init(){

    USART_Init ( MYUBRR );
    fdevopen(USART_Transmit, USART_Receive); //Make a stream between terminal and t/r functions
    //while(1)printf("Knjut"); //Prints a string
    //while(1) USART_Transmit(USART_Receive()); //Transmits what it recieves from terminal back to the terminal
    //while(1) USART_Transmit('a');
}
