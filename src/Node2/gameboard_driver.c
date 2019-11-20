/*!@file
* This file contains functions to play the pingpong game including the IR-sensor and solenoid.
*/
#define MOTOR_ADDRESS_WRITE 0x50 // 0101 000 0
#define COMMAND_BYTE 0x00
#define F_CPU 16000000
#define IR_SAMPLE_NO 4
#define DEAD 4

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pid.h"
#include "gameboard_driver.h"
#include "timer_driver.h"
#include "can_driver.h"
#include "motor_driver.h"

volatile static int       ir_adc_interrupt_flag = 0;
volatile static int       playing               = 0;
volatile static int       adc_value;
volatile static uint16_t  ir_value;

/*!
*@brief Struct containing the current game state.
*/
typedef struct Game_struct {
  uint8_t right_slider_pos;     /*!Position of right slider*/
  uint8_t right_button;         /*!1 if right button is pressed, 0 if not*/
  uint8_t solenoid_extend;      /*!1 if solenoid is extended, 0 if not.*/
  uint8_t servo;                /*!Direction of servo*/
} Game;

Game current_game;

/*-----------------------------------------------------*/
/*Function declarations*/

/*!
*@brief Reads the IR sensor with digital filtering.
*@return @c uint16_t -> Digital IR-sensor value.
*/
uint16_t ir_adc_read();

/*!
*@brief Sets the servo dutycycle from joystick input.
*@param[in] @c uint8_t pos_msg -> Joystick Y position.
*/
void servo_joystick_control(uint8_t pos_msg);

/*!
*@brief Extends the solenoid.
*/
void solenoid_extend(uint8_t right_button);

/*------------------------------------------------------*/
/*Function implementations*/

void game_board_init(){
  current_game.right_slider_pos = 127;
  current_game.right_button     = 0;
  current_game.servo            = 100;
}

void servo_joystick_control(uint8_t pos_msg){
  /* position between 0 and 200 */
  uint32_t cycle = 4200 - 12*pos_msg;
  set_duty_cycle(cycle);
}

void ir_adc_init() {

  /*Enable ADC*/
  ADCSRA  |=  (1 << ADEN);
  /*Set prescaler to 128*/
  ADCSRA  |=  (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
  /*Choose input pin*/
  DDRF    &= ~(1 << PF0);
  /*Choose reference voltage*/
  ADMUX   &= ~(1 << REFS1);
  ADMUX   |=  (1 << REFS0);
  /*Enable interrupts*/
  ADCSRA  |=  (1 << ADIE);
  /*Clear interrupt flag */
  ADCSRA  &= ~(1 << ADIF);
}

void solenoid_init(){
  /*Set output pin to enable solenoid relay */
  DDRB  |= (1 << PB5);// (1 << PB5);
  /* "Active high"  */
  PORTB &= ~(1 << PB5);//~(1 << PB5);
}

void solenoid_extend(uint8_t right_button){
  if (right_button) {
    PORTB |= (1 << PB5);
    _delay_ms(100);
    PORTB &= ~(1 << PB5);
  }
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
    _delay_ms(5);
  }
  return adc_value/IR_SAMPLE_NO;
}


void play_pingpong() {
  /*CTC Interrupt disable*/
    TIMSK4 &= ~(1 << OCIE4A);
    motor_calibrate();
    _delay_ms(200);
    pid_init();
    _delay_ms(1000);
    while(1) {
        /*Check if IR signal is cut*/
        if (ir_adc_read() < DEAD){
          TIMSK4 &= ~(1 << OCIE4A);
          int8_t message[3];
          message[0] = MOTOR_ADDRESS_WRITE;
          message[1] = COMMAND_BYTE;
          message[2] = 0;  // Stop
          TWI_Start_Transceiver_With_Data(message,3);
          _delay_ms(1000);
          break;
        }
        /*Slider input*/
        if(get_CAN_msg().id == 2) {
          current_game.right_slider_pos = get_CAN_msg().data[0];
          if (get_CAN_msg().data[2] && current_game.right_button == 0 ) {
             current_game.right_button    = 1;
             current_game.solenoid_extend = 1;
          }
          else if (get_CAN_msg().data[2] && current_game.right_button == 1) {
            current_game.solenoid_extend  = 0;
          }
          else if (get_CAN_msg().data[2] == 0) {
            current_game.right_button     = 0;
            current_game.solenoid_extend  = 0;
          }
        }
        /*JOYSTICK INPUT*/
        if (get_CAN_msg().id == 1) {
            current_game.servo = get_CAN_msg().data[0];
        }
        servo_joystick_control(current_game.servo);
        solenoid_extend(current_game.solenoid_extend);
    }
    /*Disable motor*/
    CANmsg stop_pingpong;
    stop_pingpong.id      = 0;
    stop_pingpong.length  = 1;
    stop_pingpong.data[0] = 0;
    can_send_msg(&stop_pingpong);
    servo_joystick_control(100); //Center servo
    _delay_ms(200);
}

ISR(ADC_vect) {
  ir_adc_interrupt_flag = 1;
}
