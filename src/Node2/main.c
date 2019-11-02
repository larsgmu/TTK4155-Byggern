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
  uint8_t pos = 3000;
  sei();
  CANmsg latest_msg;
  Player ung_spiller;
  solenoid_init();
  player_init(&ung_spiller, "Ung Spiller");
  printf("Velkommen til Mujafas BMW!\n\r");


  while (1){
     latest_msg = can_receive_msg();
     // ir_game_score(&ung_spiller);
     //motor_run(30);
     //printf("Message ID: %d      Length: %d \n\r", latest_msg.id, latest_msg.length);
     // for (int i = 0; i < latest_msg.length; i ++){
     //   printf("Data: %d \n\r", latest_msg.data[i]);
     // }

     if (latest_msg.id == 1) {
      servo_joystick_control(&latest_msg);
      motor_run(latest_msg.data[0]);
    }
    // printf("ENCODER: %d\n\r", encoder_read());
    if(latest_msg.data[2] == 1){
        solenoid_extend();
    }

    printf("Right button: %d\n\r", latest_msg.data[2]);
     // _delay_ms(5000);

  }
}
