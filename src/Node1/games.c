#include "games.h"
#include "oled_driver.h"
#include "can_driver.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 4915200

CANmsg latest_msg;
Game_info game;

void pingpong_timer_init() {
  /*Normal mode*/
  TCCR3A = 0;

  /*Prescaler 64*/
  TCCR3B &= ~(1 << CS32);
  TCCR3B |= (1 << CS31);
  TCCR3B |= (1 << CS30);

  /*Enable timer overflow interrupt*/
  ETIMSK |= (1 << TOIE3);

  /*Clear interrupt flag*/
  ETIFR |= (1 << TOV3);
}

void pingpong_score(){
  game.score += 1;
}

void play_pingpong(char* player, Joystick* joy) {

  game.player_name = player;
  game.score = 0;

  CANmsg start_pingpong;
  start_pingpong.id = 0;
  start_pingpong.length = 1;
  start_pingpong.data[0] = 1;

  oled_sram_reset();
  oled_goto_line(3);
  oled_sram_write_string("GET READY!");
  oled_draw();
  can_send_msg(&start_pingpong);
  _delay_ms(1000);
  pingpong_timer_init();
  oled_sram_reset();
  oled_goto_line(3);
  oled_sram_write_string("PLAYING");
  oled_goto_line(4);
  oled_sram_write_string("PINGPONG");
  oled_draw();

  while(1) {

    joystick_run(joy);
    send_joystick_pos(joy);
    latest_msg = get_CAN_msg();
    if (get_CAN_msg().id == 0){
      if (get_CAN_msg().data[0] == 0) {
        oled_sram_reset();
        oled_goto_line(3);
        oled_sram_write_string("YOU DIED!");
        oled_draw();
        _delay_ms(2000);
        break;
      }
    }
  }
  /*Disable timer overflow interrupt*/
  ETIMSK &= ~(1 << TOIE3);
}

ISR(TIMER3_OVF_vect){
  pingpong_score();
}
