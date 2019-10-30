#include "solenoid_driver.h"
#include <util/delay.h>
#include <avr/io.h>

void solenoid_init(){

  /*Set output pin to enable solenoid relay */
  DDRB |= (1 << PB4);

  /* "Active high"  */
  PORTB |= (1 << PB4);

}
void solenoid_extend(){

  PORTB |= (1 << PB4);
  _delay_ms(100);
  PORTB &= ~(1 << PB4);


}
