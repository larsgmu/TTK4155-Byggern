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
  DDRK &= ~(1 << PK7);
  DDRK &= ~(1 << PK6);
  DDRK &= ~(1 << PK5);
  DDRK &= ~(1 << PK4);
  DDRK &= ~(1 << PK3);
  DDRK &= ~(1 << PK2);
  DDRK &= ~(1 << PK1);
  DDRK &= ~(1 << PK0);

  /*Enable motor*/
  PINH |= (1 << PH4);

}

void motor_run(uint8_t val){

  /* Set direction*/
  int8_t data = val - 100;
  if (data < 0) {
    data = (-1)*data;
    PORTH &= ~(1 << PH1);
  }
  else if (data > 15) {
    PORTH |= (1 << PH1);
  }
  else {
    data = 0;
  }
  /* Set speed */
  int8_t message[3];
  message[0] = MOTOR_ADDRESS_WRITE;
  message[1] = COMMAND_BYTE;
  message[2] = data;

  TWI_Start_Transceiver_With_Data(message,3);
}

void motor_get_position(){
  /*Max and min values of the encoder output */
  int encoder_min = 0;
  int encoder_max = 7805;

}

int16_t encoder_read(){
  uint8_t high;
  uint8_t low;
  PORTH &= ~(1 << PH5);
  PORTH &= ~(1 << PH3);
  _delay_us(300);
  high = PINK;
  PORTH |= (1 << PH3);
  _delay_us(300);
  low = PINK;
  reset_encoder();
  PORTH |= (1 << PH5);
  return (int16_t) ((high << 8) | low);
}

void reset_encoder() {
  PORTH &= ~(1 << PH6);
  _delay_us(200);
  PORTH |= (1 << PH6);
}
