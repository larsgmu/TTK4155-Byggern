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

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

//sudo picocom /dev/ttyACM0

void main () {

  cli();
  string_init();
  can_init();
  timer_init();
  sei();

  // DDRB |= (1 << PB6);
  // PORTB |= (1 << PB6);


  CANmsg latest_msg;
  while (1){
    // for (int cycle = 1000; cycle <= 5000; cycle += 200){
    //   set_duty_cycle(cycle);
    //   _delay_ms(500);
    // }
    // printf("-------------------------\n\r");

    //printf("bajs\n\r");
    latest_msg = can_receive_msg();
    if (latest_msg.id == 1){
      printf("Message ID: %d      Length: %d \n\r", latest_msg.id, latest_msg.length);
      printf("Data: ");
      for (int i = 0; i < latest_msg.length; i ++) {
        printf("%d \n\r", latest_msg.data[i]);
      }
      _delay_ms(200);
    }
  }

}
