#include "gameboard_driver.h"
#include "timer_driver.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define IR_SAMPLE_NO 4
volatile static int ir_adc_interrupt_flag = 0;
volatile static int playing = 0;
volatile static int adc_value;


void servo_joystick_control(CANmsg* pos_msg){
  /* position between 0 and 200 */
  uint32_t cycle = 12*pos_msg->data[0] + 1800;
  set_duty_cycle(cycle);
  //printf("Duty cycle: %d\n\r", cycle );
}

void ir_adc_init() {

  /*Enable ADC*/
  ADCSRA |= (1 << ADEN);

  /*Set prescaler to 128*/
  ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

  /*Choose input pin*/
  DDRF &=  ~(1 << PF0);

  /*Finn ut hva denne gjør*/
  ADMUX  |= (1 << REFS0);

  /*Enable interrupts | clear int flag */
  ADCSRA |= (1 << ADIE); //| (1 << ADIF);
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

ISR(ADC_vect) {
  ir_adc_interrupt_flag = 1;
}

void player_init(Player* player, char* name){
  player->score = 0;
  player->lives = 3;
  player->name = name;
}

/*Har skrevet denne nå kun for å teste logikken og det funker
Vi må finne en ny måte å sørge for at den ikke teller ned antall liv
mens ballen fortsatt bever seg nedover */
void ir_game_score(Player* player) {
  adc_value = ir_adc_read();
  if (adc_value < 70 && player->lives != 0 && playing == 1){
    player->lives -= 1;
    playing = 0;
    if (player->lives == 0) {
      printf("You dead now\n\r");
    }
    else{
    printf("Lives: %d\n\r", player->lives);
    }
  }
  else if (adc_value > 150 && player->lives != 0 && playing == 0){
    playing = 1;
  }
}
