#include "gameboard_driver.h"
#include "timer_driver.h"
#include "can_driver.h"
#include "motor_driver.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>

#define IR_SAMPLE_NO 4
#define DEAD 1

volatile static int ir_adc_interrupt_flag = 0;
volatile static int playing = 0;
volatile static int adc_value;
volatile static int ir_value= DEAD;


void servo_joystick_control(uint8_t pos_msg){
  /* position between 0 and 200 */
  uint32_t cycle = 12*pos_msg + 1800;
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

  //ADMUX |= (1 << MUX0);

  /*Enable interrupts*/
  ADCSRA |= (1 << ADIE);

  /*clear int flag */
  ADCSRA &= ~(1 << ADIF);
}


uint8_t ir_adc_read() {
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


/* ADC = (V_IN * 1024) / V_REF
    0.5 - 3V  = Invalid
 => 102 - 615 = Invalid
*/


void play_pingpong() {

    while(1) {
        ir_value = ir_adc_read();
        if (get_CAN_msg().id == 1) {
            //printf("MOTOR: %d     SERVO: %d     SOLENOID: % d\n\r",get_CAN_msg().data[0], get_CAN_msg().data[1], get_CAN_msg().data[2] );
            motor_run(get_CAN_msg().data[0]);
            servo_joystick_control(get_CAN_msg().data[1]);
            if (get_CAN_msg().data[2]){
                printf("FIRE!\n\r");
                solenoid_extend();
                printf("FIRE VOL 2\n\r");
            }
        }
        if (ir_value < DEAD){
            break;
        }
    }
    CANmsg stop_pingpong;
    stop_pingpong.id = 0;
    stop_pingpong.length = 1;
    stop_pingpong.data[0] = 0;
    can_send_msg(&stop_pingpong);
    printf("STOP PINGPONG SENT\n\r");
}


void solenoid_init(){

  /*Set output pin to enable solenoid relay */
  DDRB |= (1 << PB4);

  /* "Active high"  */
  PORTB &= ~(1 << PB4);
}

void solenoid_extend(){

  PORTB |= (1 << PB4);
  PORTB &= ~(1 << PB4);
}

ISR(ADC_vect) {
  ir_adc_interrupt_flag = 1;
}
