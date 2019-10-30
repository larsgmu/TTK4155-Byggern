#include "motor_driver.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <util/delay.h>

#define MOTOR_ADDRESS_WRITE 0x50 // 0101 000 0
#define COMMAND_BYTE 0x00
#define F_CPU 16000000


void motor_init() {
  TWI_Master_Initialise();
  /* Set direction pin as output*/
  DDRH |= (1 << PH1);
  /*Set SEL pin as output*/
  DDRH |= (1 << PH3);
  /*Enable motor pin as output*/
  DDRH |= (1 << PH4);
  /*Encoder output enable pin as output*/
  DDRH |= (1 << PH5);

  /*Set PIN-K as input*/
  DDRK = 0;

}

void motor_run(uint8_t val){

  /* Set direction*/
  uint8_t data = val - 100;
  if (val < 0) {
    data = (-1)*val;
    PORTH &= ~(1 << PH1);
  }
  else {
    data = val;
    PORTH |= (1 << PH1);
  }

  /* Set speed */
  uint8_t message[3];
  message[0] = MOTOR_ADDRESS_WRITE;
  message[1] = COMMAND_BYTE;
  message[2] = data;

  TWI_Start_Transceiver_With_Data(message,3);
}

int16_t encoder_read(){
  uint8_t high;
  uint8_t low;
  PORTH &= ~(1 << PH5);
  PORTH &= ~(1 << PH3);
  _delay_us(50);
  high = PINK;
  PORTH |= (1 << PH3);
  _delay_us(50);
  low = PINK;
  PORTH |= (1 << PH5);
  return (int16_t) ((high << 8) | low);
}

void reset_encoder() {
  PORTH &= ~(1 << PH6);
  _delay_us(200);
  PORTH |= (1 << PH6);
}
