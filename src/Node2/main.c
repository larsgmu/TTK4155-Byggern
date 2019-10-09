/*!@file
* NODE 2 MAIN FILE
*/
#define F_CPU 16000000

#include "usart_driver.h"
#include "spi_driver.h"
#include "mcp2515_driver.h"
#include "MCP2515.h"
#include "can_driver.h"

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

//sudo picocom /dev/ttyACM0

void main () {

  cli();
  string_init();
  can_init();
  sei();

  CANmsg latest_msg;
  while (1){
    //printf("horer er greit\n\r");
    latest_msg = can_receive_msg();
    printf("Message ID: %d      Length: %d \n\r", latest_msg.id, latest_msg.length);
    printf("Data: ");
    for (int i = latest_msg.length - 1; i >= 0; i --) {
      printf("%d", latest_msg.data[i]);
    }
    _delay_ms(200);
    printf("\n\r");
  }

}
