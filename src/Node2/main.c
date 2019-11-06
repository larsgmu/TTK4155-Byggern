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
#include "solenoid_driver.h"


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>


void main () {

  cli();
  string_init();
  can_init();
  timer_init();
  ir_adc_init();
  motor_init();
  sei();
  solenoid_init();
  printf("Velkommen til Mujafas BMW!\n\r");

  typedef enum {IDLE = 0, PINGPONG = 1}mode;
  mode MODE = IDLE;

  while (1){
     switch (MODE) {
       case IDLE:
           /*Disable timer overflow interrupt*/
           ETIMSK &= ~(1 << TOIE3);
           if (get_CAN_msg().id == 0 && get_CAN_msg().data[0] == 1){
               MODE = PINGPONG;
           }
           if (get_CAN_msg().id == 3){
               //Implementer detta
               //set_difficulty(get_CAN_msg().data[0]);
           }
           break;
       case PINGPONG:
            play_pingpong();
            MODE = IDLE;
            break;
     }
  }
}
