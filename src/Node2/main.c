/*!@file
* NODE 2 MAIN FILE
*/
#define F_CPU 16000000

#include "usart_driver.h"
#include "spi_driver.h"
#include "mcp2515_driver.h"
#include "MCP2515.h"
#include "can_driver.h"
#include "timer_driver.h"
#include "gameboard_driver.h"
#include "motor_driver.h"


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

ISR(__vector_default){
  printf("FUCK ME\n\r");
}

void main () {

  cli();
  string_init();
  can_init();
  timer_init();
  //motor_init();
  ir_adc_init();
  solenoid_init();
  sei();

  CANmsg run_menu;
  run_menu.id = 0;
  run_menu.length = 1;
  run_menu.data[0] = 8;
  printf("########### Reboot ##############\n\r");

  typedef enum {IDLE = 0, PINGPONG = 1}mode;
  mode MODE = IDLE;
  while (1){
      switch (MODE) {
        case IDLE:
           if (get_CAN_msg().id == 0 && get_CAN_msg().data[0] == 1){
               MODE = PINGPONG;
           }
           if (get_CAN_msg().id == 3){
               //Implementer detta
               //set_difficulty(get_CAN_msg().data[0]);
           }
           break;
        case PINGPONG:
            printf("MODE: PINGPONG\n\r");
            play_pingpong();
            MODE = IDLE;
            _delay_ms(100);
            can_send_msg(&run_menu);
            break;
    }
  }
}
