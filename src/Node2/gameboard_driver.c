#include "gameboard_driver.h"
#include "timer_driver.h"
#include <stdint.h>
#include <avr/io.h>

#define IR_SAMPLE_NO 4
volatile static int ir_adc_interrupt_flag = 0;

void servo_joystick_control(CANmsg* pos_msg){
  /* position between 0 and 200 */
  uint32_t cycle = -12*pos_msg->data[0] + 4200;
  set_duty_cycle(cycle);
}

void ir_adc_init() {

  /*Choose input pin*/
  DDRF &=  ~(1 << PF0);

  /*Enable interrupts | clear int flag */
  ADCSRA |= (1 << ADIE) | (1 << ADIF);

  /*Set prescaler to 128*/
  ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

  /*Enable ADC*/
  ADCSRA |= (1 << ADEN);
}


uint8_t ir_adc_read() {
  /*Sample 4 times to reduce errors*/
  int adc_value = 0;
  for (int i = 0; i < IR_SAMPLE_NO; i++) {

    /*Start converting*/
    ADCSRA |= (1 << ADSC);
    while(!ir_adc_interrupt_flag);
    ir_adc_interrupt_flag = 0;

    adc_value += ADC; /* ADC = (V_IN * 1024) / V_REF */
  }
  return adc_value/IR_SAMPLE_NO;
}

//    0.5 - 3V  = Invalid
// => 102 - 615 = Invalid

ISR(ADC_vect) {
  ir_adc_interrupt_flag = 1;
}
