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
#include "spi_driver.h"
#include "mcp2515_driver.h"
#include "MCP2515.h"
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

    mcp2515_init();
    printf("Read from MCP_CANSTAT %d\n\r", mcp2515_read(MCP_READ_STATUS));

    //oled_sram_write_char('~');
    struct joystick joy;
    joystick_init(&joy);
    struct slider slider;

    while(1){
      mcp2515_write(0x00, 0b10001000);

      analog_position(&joy);
      analog_direction(&joy);

      //printf("X: %d     Y: %d     DIR: %d       NX: %d \n\r",joy.x,joy.y,joy.dir, joy.neutralx);
      //get_slider_pos(&slider);

      menu_run(&joy);
      _delay_ms(100);
      oled_draw();
    }
}
