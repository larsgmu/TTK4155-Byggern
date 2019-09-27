#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include "uart_driver.h"
#include "adc_driver.h"
#include "sram_driver.h"
#include "joystick_driver.h"
#include "slider_driver.h"
#include "oled_driver.h"
#include "menu.h"

void main( void ){
    cli();
    string_init();
    SRAM_init();
    adc_init();
    sei();
    oled_init();
    oled_sram_reset();
    menu_init();
    //oled_sram_write_char('~');
    struct joystick joy;
    joystick_init(&joy);
    struct slider slider;

    while(1){
      analog_position(&joy);
      analog_direction(&joy);
      //printf("X: %d     Y: %d     DIR: %d       NX: %d \n\r",joy.x,joy.y,joy.dir, joy.neutralx);
      //get_slider_pos(&slider);

      //oled_sram_test("~");
      menu_run(&joy);
          }
    SRAM_test();

    // SRAM_write(0,'A');
    // oled_home();
    // int output = SRAM_read(0) - 32;
    // static volatile char* oled_data_address = (char*)0x1200;
    // for(int i = 0; i<8; i++){
    //
    //     *oled_data_address = pgm_read_byte(&font8[output][i]);
    // }

}
