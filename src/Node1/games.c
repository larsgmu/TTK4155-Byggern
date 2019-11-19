/*!@file
* This file contains functions to play the pingpong game.
*/
#define F_CPU 4915200

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

#include "games.h"
#include "oled_driver.h"
#include "can_driver.h"
#include "joystick_driver.h"
#include "slider_driver.h"
#include "eeprom_driver.h"

/*!
*@brief Struct containing the name and score of the current player.
*/
typedef struct Game_info_struct{
  char*     player_name;
  uint8_t   score;
} Game_info;

static Game_info game;

/*-------------------------------------------------------*/
/********Function declarations*********/

/*!
*@brief Initializes TIMER3 to keep track of pingpong score.
*/
void pingpong_timer_init();

/*!
*@brief Counts the score of current game.
*/
void pingpong_score();

/*-------------------------------------------------------*/
/********Function implementations*********/

void pingpong_timer_init() {
  /*Normal mode*/
  TCCR3A = 0;

  /*Prescaler 64*/
  TCCR3B &= ~(1 << CS32);
  TCCR3B |=  (1 << CS31);
  TCCR3B |=  (1 << CS30);

  /*Enable timer overflow interrupt*/
  ETIMSK |= (1 << TOIE3);

  /*Clear interrupt flag*/
  ETIFR  |= (1 << TOV3);
}

void pingpong_score() {
  game.score ++;
}

void play_pingpong() {
  /*Stop Music from node 2*/
  CANmsg music_msg;
  music_msg.id      = 7;
  music_msg.length  = 1;
  music_msg.data[0] = 0;
  can_send_msg(&music_msg);
  /*Enough delay for the longest note in song to finish before stopping*/
  _delay_ms(600);

  /*Reset score*/
  game.score        = 0;

  /*Create start pingpong CAN message*/
  CANmsg start_pingpong;
  start_pingpong.id       = 0;
  start_pingpong.length   = 1;
  start_pingpong.data[0]  = 1;

  /*Update OLED and send start pingpong message*/
  oled_sram_reset();
  oled_goto_line(3);
  oled_sram_write_string("GET READY!");
  oled_draw();
  can_send_msg(&start_pingpong);
  _delay_ms(1000);
  pingpong_timer_init();
  _delay_ms(100);
  oled_sram_reset();
  oled_goto_line(2);
  oled_sram_write_string("PLAYING");
  oled_goto_line(4);
  oled_sram_write_string("PINGPONG");
  oled_draw();

  /*Play game until stop pingpong message is received from Node2*/
  while(1) {
    joystick_send_pos();
    slider_send_pos();
    if (get_CAN_msg().id == 0) {
      /*Stop ping pong recieved*/
      if (get_CAN_msg().data[0] == 0) {
          char score[5];
          itoa(game.score, score, 10);
          oled_sram_reset();
          oled_goto_line(1);
          oled_sram_write_string("YOU DIED!");
          oled_pos(3,0);
          oled_sram_write_string("Score: ");
          oled_sram_write_string(score);

          /*If a new Highscore is obtained, save it*/
          if (game.score > eeprom_read(HIGHSCORE_PINGPONG_ADDR)) {
            eeprom_write(HIGHSCORE_PINGPONG_ADDR, 0b0000000);
            eeprom_write(HIGHSCORE_PINGPONG_ADDR, game.score);
            oled_pos(6,0);
            oled_sram_write_string("NEW HIGHSCORE!");
          }
          oled_draw();
          _delay_ms(5000);
          break;
      }
    }
  }
  /*Disable timer overflow interrupt*/
  ETIMSK &= ~(1 << TOIE3);
  ETIFR  |=  (1 << TOV3);
}

ISR(TIMER3_OVF_vect) {
  pingpong_score();
}
