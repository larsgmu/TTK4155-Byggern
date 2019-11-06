/*!@file
* Node 1 main file
*/
#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include "usart_driver.h"
#include "adc_driver.h"
#include "sram_driver.h"
#include "slider_driver.h"
#include "oled_driver.h"
#include "spi_driver.h"
#include "mcp2515_driver.h"
#include "MCP2515.h"
#include "can_driver.h"
#include "menu.h"

void main( void ){
    cli();
    string_init();
    SRAM_init();
    adc_init();
    oled_init();
    oled_sram_reset();
    menu_init();
    can_init();
    sei();

    Slider slider;

    char line[8] = {1,0,0,0,0,0,1,0};
    char temp[1]= {0b00000000};
    for(int i= 0; i < 8 ; i++){
      temp[0] |= (line[i]<<(7-i));
      printf("Temp: %d \n\r",(uint8_t)temp[0]);
    }
    //printf("Temp: %s\r\n", temp[0]);


    while(1){
      menu_run();
      _delay_ms(40);
      oled_draw();
    }
}
