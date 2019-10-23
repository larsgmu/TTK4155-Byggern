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

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>



/*
###### STATUS DAG 7 #######
- Når timer_init() blir kalt, fungerer ikke å motta meldinger over canbus
*/



void main () {

  cli();
  string_init();
  can_init();
  timer_init();
  uint8_t pos = 3000;
  sei();




  while (1){
    // for (int i = 0; i < 50; i++) {
    //   set_duty_cycle(i*100);
    //   _delay_ms(10);
    // }
    // for (int i = 50; i > 0; i--) {
    //   set_duty_cycle(i*100);
    //   _delay_ms(10);
    // }
    printf("-------------------------\n\r");
    CANmsg latest_msg;
    latest_msg = can_receive_msg();

    printf("Message ID: %d      Length: %d \n\r", latest_msg.id, latest_msg.length);
    for (int i = 0; i < latest_msg.length; i ++) {
      printf("Data: %d \n\r", latest_msg.data[i]);
    }

    if (latest_msg.id = 1) {
      servo_joystick_control(&latest_msg);
    }
  }
}
