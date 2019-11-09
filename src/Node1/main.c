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
#include "space_runner.h"
#include "spi_driver.h"
#include "sram_driver.h"
#include "usart_driver.h"

// typedef struct Bit_struct {
//     unsigned b:1;
// } sr_Bit;
//
// static sr_Bit MAP[64][128];
// for (int y = 0; y < 64; y++) {
//   for (int x = 0; x < 128; x++) {
//     MAP[y][x].b = 0;
//   }
// }
// uint8_t map[8][128];     //hvorfor funker dette men ikke det over?
// for (int i = 0; i < 8; i++) {
//   for(int j = 0; j < 128; j++) {
//     map[i][j] = 1;
//   }
// }


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
    sr_play(&joy);


    while(1) {};


    printf("########### Reboot ##############\n\r");
    while(1){
      menu_run(&joy);
      _delay_ms(40);
      oled_draw();
    }
}
