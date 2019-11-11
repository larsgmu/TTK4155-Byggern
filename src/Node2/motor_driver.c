/*!@file
* This file cointains functions to controll the motor.
*/

#include "motor_driver.h"
#include "TWI_Master.h"
#include <avr/io.h>

#define MOTOR_ADDRESS_WRITE 0x50 // 0101 000 0
#define COMMAND_BYTE 0x00


#define F_CPU 16000000
#include <util/delay.h>


void motor_init() {
  TWI_Master_Initialise();
  DDRH |= (1 << PH4);
  PINH |= (1 << PH4);

  /* Set direction pin as output*/
  DDRH |= (1 << PH1);
  /*Set SEL pin as output*/
  DDRH |= (1 << PH3);
  /*Enable motor pin as output*/


  /*Encoder output enable pin as output*/
  DDRH |= (1 << PH5);
  /*Encoder reset pin as output*/
  //DDRH |= (1 << PH6);

  /*Set PIN-K as input*/
  DDRK &= ~(1 << PK7);
  DDRK &= ~(1 << PK6);
  DDRK &= ~(1 << PK5);
  DDRK &= ~(1 << PK4);
  DDRK &= ~(1 << PK3);
  DDRK &= ~(1 << PK2);
  DDRK &= ~(1 << PK1);
  DDRK &= ~(1 << PK0);

  //reset_encoder();
  /*Enable motor*/

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

  printf("DATA:                        %d\n\r", data);

  /* Set speed */
  int8_t message[3];
  message[0] = MOTOR_ADDRESS_WRITE;
  message[1] = COMMAND_BYTE;
  message[2] = data;

  printf("ENCODER VALUE: %d        \n\r", encoder_read() );

  TWI_Start_Transceiver_With_Data(message,3);
}

void motor_get_position(){
  /*Max and min values of the encoder output */
  int encoder_min = 0;
  int encoder_max = 7805;

}

int16_t encoder_read(){
  PINH |= (1 << PH6);

  uint8_t high;
  uint8_t low;
  PORTH &= ~(1 << PH5);
  PORTH &= ~(1 << PH3);
  _delay_us(300);
  high = PINK;
  PORTH |= (1 << PH3);
  _delay_us(300);
  low = PINK;
  //reset_encoder();
  PORTH |= (1 << PH5);
  return (int16_t) ((high << 8) | low);
}

void reset_encoder() {
  PORTH &= ~(1 << PH6);
  _delay_us(200);
  PORTH |= (1 << PH6);
}
