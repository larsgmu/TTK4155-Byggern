/*!@file
* Node 1 main file
*/
#define F_CPU 4915200

#include <util/delay.h>
#include <avr/interrupt.h>

#include "adc_driver.h"
#include "can_driver.h"
#include "joystick_driver.h"
#include "menu.h"
#include "oled_driver.h"
#include "slider_driver.h"
#include "sram_driver.h"
#include "usart_driver.h"


void main( void ){

    cli();
    string_init();
        _delay_ms(20);
    sram_init();
        _delay_ms(20);
    adc_init();
        _delay_ms(20);
    oled_init();
        _delay_ms(20);
    oled_sram_reset();
        _delay_ms(20);
    menu_init();
        _delay_ms(20);
    can_init();
        _delay_ms(20);
    sei();
    joystick_init();

    while(1){
      menu_run();
    }
}
