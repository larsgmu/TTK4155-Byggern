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
#include "joystick_driver.h"
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
    Joystick joy;
    joystick_init(&joy);
    Slider slider;



    while(1){
      menu_run(&joy);
      _delay_ms(40);
      oled_draw();
    }
}
