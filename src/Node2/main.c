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
  printf("FUCK YOU\n\r");
}

void main () {

  cli();
  string_init();
  can_init();
  timer_init();
  motor_init();
  ir_adc_init();
  solenoid_init();
  sei();

  printf("LATEST MSG: %d\n\r",get_CAN_msg().id);
  //play_pingpong();

  // typedef enum {IDLE = 0, PINGPONG = 1}mode;
  // mode MODE = IDLE;
  // while (1){
  //     printf("ID:               %d\n\r",get_CAN_msg().id);
  //    switch (MODE) {
  //      case IDLE:
  //          //printf("MODE:  NORMAL\n\r");
  //          if (get_CAN_msg().id == 0 && get_CAN_msg().data[0] == 1){
  //              MODE = PINGPONG;
  //          }
  //          if (get_CAN_msg().id == 3){
  //              //Implementer detta
  //              //set_difficulty(get_CAN_msg().data[0]);
  //          }
  //          break;
  //      case PINGPONG:
  //           printf("MODE:             PINGPONG\n\r");
  //           play_pingpong();
  //           MODE = IDLE;
  //           break;
  //   }
  // }
}
