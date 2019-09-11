#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define SRAM_START_ADR 0x1800
#include "uart.h"
#include "adc_driver.h"
#include "sram.h"




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
    //while(1) USART_Transmit('a');
}

void SRAM_init(){
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM2);
}

void SRAM_write(unsigned int adr, unsigned int data) {
    volatile char* external_ram = (char *) 0x1000;
    external_ram[adr] = data;
}

unsigned int SRAM_read(unsigned int adr) {
    volatile char* external_ram = (char *) 0x1000;
    printf(external_ram[adr]);
    return external_ram[adr];
}

void main( void ){
    string_init();
    SRAM_init();
    adc_init();
/*  while(1){
    SRAM_write(0,0); //Alle lyse
    _delay_ms(1000);
    SRAM_write(400,0); //P17, P16 lyse
    _delay_ms(1000);
    SRAM_write(200,0); //P16, P18 lyse
    _delay_ms(1000);
    SRAM_write(0,0); // P17, P18 lyse
    _delay_ms(1000);



  }
  SRAM_write(0,0);*/
  while(1){
     adc_read(X_axis);
  }
//SRAM_test();
}
