/*!@file
* This file contains functions to play the pingpong game including the IR-sensor.
*/
#define MOTOR_ADDRESS_WRITE 0x50 // 0101 000 0
#define COMMAND_BYTE 0x00
#define F_CPU 16000000
#define IR_SAMPLE_NO 4
#define DEAD 25

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pid.h"
#include "gameboard_driver.h"
#include "timer_driver.h"
#include "can_driver.h"
#include "motor_driver.h"



volatile static int ir_adc_interrupt_flag = 0;
volatile static int playing = 0;
volatile static int adc_value;
volatile static uint16_t ir_value;

Game current_game;

void game_board_init(){
  current_game.right_slider_pos = 127;
  current_game.right_button = 0;
  current_game.servo = 100;
}

void servo_joystick_control(uint8_t pos_msg){
  /* position between 0 and 200 */
  uint32_t cycle = 4200 - 12*pos_msg;
  set_duty_cycle(cycle);
}

void ir_adc_init() {

  /*Enable ADC*/
  ADCSRA |= (1 << ADEN);

  /*Set prescaler to 128*/
  ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

  /*Choose input pin*/
  DDRF &=  ~(1 << PF0);

  /*Choose reference voltage*/
  ADMUX &= ~(1 << REFS1);
  ADMUX |= (1 << REFS0);


  /*Enable interrupts*/
  ADCSRA |= (1 << ADIE);

  /*clear int flag */
  ADCSRA &= ~(1 << ADIF);


}

void solenoid_init(){
  /*Set output pin to enable solenoid relay */
  DDRB |= (1 << PB5);

  /* "Active high"  */
  PORTB &= ~(1 << PB5);
}

uint16_t ir_adc_read() {
  /*Sample 4 times to reduce errors*/
  int adc_value = 0;
  for (int i = 0; i < IR_SAMPLE_NO; i++) {
    /*Start converting*/
    ADCSRA |= (1 << ADSC);
    while(!ir_adc_interrupt_flag);
    ir_adc_interrupt_flag = 0;
    adc_value += ADC;
  }
  return adc_value/IR_SAMPLE_NO;
}

void solenoid_extend(uint8_t right_button){
  if (right_button) {
    PORTB |= (1 << PB5);
    _delay_ms(100);
    PORTB &= ~(1 << PB5);
  }
}

void play_pingpong() {
    motor_init();
    motor_calibrate();
    pid_init();
    while(1) {
        if (ir_adc_read() < DEAD){
          break;
        }
        /*SLIDER INPUT*/
        if(get_CAN_msg().id == 2) {
          current_game.right_slider_pos = get_CAN_msg().data[0];
          if (get_CAN_msg().data[2] && current_game.right_button == 0 ){
             current_game.right_button = 1;
             current_game.solenoid_extend = 1;
          }
          else if (get_CAN_msg().data[2] && current_game.right_button == 1){
            current_game.solenoid_extend = 0;
          }
          else if (get_CAN_msg().data[2] == 0){
            current_game.right_button = 0;
            current_game.solenoid_extend = 0;
          }
        }
        /*JOYSTICK INPUT*/
        if (get_CAN_msg().id == 1) {
            current_game.servo = get_CAN_msg().data[0];
        }
        _delay_ms(20); //60 Hz frequency
        pid_controller(current_game.right_slider_pos);
        servo_joystick_control(current_game.servo);
        solenoid_extend(current_game.solenoid_extend);
    }
    CANmsg stop_pingpong;
    stop_pingpong.id = 0;
    stop_pingpong.length = 1;
    stop_pingpong.data[0] = 0;
    can_send_msg(&stop_pingpong);
    printf("STOP PINGPONG SENT\n\r");
    servo_joystick_control(100); //Center servo
    _delay_ms(200);
}

ISR(ADC_vect) {
  ir_adc_interrupt_flag = 1;
}
