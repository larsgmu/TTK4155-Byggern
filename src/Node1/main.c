/*!@file
* Node 1 main file
*/
#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include "adc_driver.h"
#include "can_driver.h"
#include "joystick_driver.h"
#include "mcp2515_driver.h"
#include "MCP2515.h"
#include "menu.h"
#include "oled_driver.h"
#include "slider_driver.h"
//#include "space_runner.h"
#include "spi_driver.h"
#include "sram_driver.h"
#include "usart_driver.h"

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
    Joystick joy;
    joystick_init(&joy);
    Slider slider;
    //sr_play(&joy);

    printf("everyone was not there \n\r");
    while(1){
      menu_run(&joy);
      _delay_ms(40);

      printf("everyone is right here \n\r");
      oled_draw();
    }
}
