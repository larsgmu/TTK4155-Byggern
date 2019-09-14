#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define SRAM_START_ADR 0x1800
#include "uart_driver.h"
#include "adc_driver.h"
#include "sram_driver.h"
#include "joystick_driver.h"
#include "slider_driver.h"



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
    cli();
    string_init();
    SRAM_init();
    adc_init();
    sei();
    // DDRB &= ~((1 << PB0) | (1 << PB1) ); //Pins set as input
    // while(1){
    //   int btn0 = PINB & (1 << PB0);
    //   int btn1 = PINB & (1 << PB1);
    //   printf("Button 0: %d ", btn0);
    //   printf("Button 1: %d \n\r", btn1);
    //   _delay_ms(40);
    // }
    struct joystick joy;
    joystick_init(&joy);
    
    struct slider slider;

    while(1){
      analog_position(&joy);
      analog_direction(&joy);
      printf("X: %d     Y: %d     DIR: %d       NX: %d \n\r",joy.x,joy.y,joy.dir, joy.neutralx);
      //printf( "X: %d \n\r", adc_read(X_axis));
      get_slider_pos(&slider);
      //printf("right: %d         left: %d \n\r", slider.right_pos, slider.left_pos);


    }

//SRAM_test();
}
