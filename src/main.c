#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define SRAM_START_ADR 0x1800
#include <util/delay.h>
#include "uart.h"




//Excercise 6 day one
void squareSignalTest() {

    DDRB   |= (1 << PB2);           // LED on pin PB2

    while(1)
    {
        _delay_ms(50);              // 50ms delay
        PORTB &= ~(1 << PB2);       // LED off
        _delay_ms(50);              // 50ms delay
        PORTB |=  (1 << PB2);       // LED on
    }

}

//Exercise 11 day one, but used for debugging
void string_init(){

    USART_Init ( MYUBRR );
    fdevopen(USART_Transmit, USART_Receive); //Make a stream between terminal and t/r functions
    //while(1)printf("Knjut"); //Prints a string
    //while(1) USART_Transmit(USART_Receive()); //Transmits what it recieves from terminal back to the terminal
}

void SRAM_init(){
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM2);





}

void SRAM_write(unsigned int adr, unsigned int data) {
    volatile char* external_ram = (char *) 0x1800;
    external_ram[adr] = data;
}

unsigned int SRAM_read(unsigned int adr) {
    volatile char* external_ram = (char *) 0x1800;

    return external_ram[adr];

}

void main( void ){
    string_init();
    SRAM_init();
    while(1){
        SRAM_write(0,0);
        _delay_ms(1000);
        SRAM_write(2,0);
        _delay_ms(1000);

    }

}
