/*!@file
* This file cointains functions to controll the motor.
*/
#define F_CPU 16000000
#define MOTOR_ADDRESS_WRITE 0x50 // 0101 000 0
#define COMMAND_BYTE 0x00

#include <avr/io.h>
#include <util/delay.h>

#include "motor_driver.h"
#include "TWI_Master.h"

/*-------------------------------------------------------*/
/********Function declarations*********/

/*!
*@brief Sets the direction and speed of the motor.
*@param[in] @c uint8_t val -> Value sent from joystick x position to control speed and direction of motor.
*/
void motor_run_joy(uint8_t val);

/*!
*@brief Reads the encoder value.
*@return @c int16_t -> Encoder value.
*/
int16_t encoder_read();

/*!
*@brief Resets the encoder.
*/
void encoder_reset();

/*-------------------------------------------------------*/
/********Function implementations*********/

void motor_init() {
  TWI_Master_Initialise();


  /* Set direction pin as output*/
  DDRH |= (1 << PH1);
  /*Set SEL pin as output*/
  DDRH |= (1 << PH3);
  /*Enable motor pin as output*/
  DDRH |= (1 << PH4);

  /*Enable motor*/
  PINH |= (1 << PH4);

  /*Encoder output enable pin as output*/
  DDRH |= (1 << PH5);
  /*Encoder reset pin as output*/
  DDRH |= (1 << PH6);

  /*Set PIN-K as input*/
  DDRK &= ~(1 << PK7);
  DDRK &= ~(1 << PK6);
  DDRK &= ~(1 << PK5);
  DDRK &= ~(1 << PK4);
  DDRK &= ~(1 << PK3);
  DDRK &= ~(1 << PK2);
  DDRK &= ~(1 << PK1);
  DDRK &= ~(1 << PK0);
  int8_t message[3];
  message[0] = MOTOR_ADDRESS_WRITE;
  message[1] = COMMAND_BYTE;
  message[2] = 0;  // Stop
  TWI_Start_Transceiver_With_Data(message,3);

  encoder_reset();
}

void motor_run_joy(uint8_t val){
  /* Set direction*/
  int8_t data = val - 100;
  if (data < 0) {         //left
    data = (-1)*data;
    PORTH &= ~(1 << PH1);
  }
  else if (data > 15) {   //right
    PORTH |= (1 << PH1);
  }
  else {
    data = 0;
  }

  /* Set speed */
  int8_t message[3];
  message[0] = MOTOR_ADDRESS_WRITE;
  message[1] = COMMAND_BYTE;
  message[2] = (int)data * 2.55;

  TWI_Start_Transceiver_With_Data(message,3);
}

void motor_run_slider(int16_t val) {
    if (val > 0) {  //Go left
      PORTH &= ~(1 << PH1);
    }
    else {        //Go right
      val = 0 - val;
      PORTH |= (1 << PH1);
    }
    if (val > 255) {
      val = 255;
    }
    //Start TWI message transmission
    int8_t message[3];
    message[0] = MOTOR_ADDRESS_WRITE;
    message[1] = COMMAND_BYTE;
    message[2] = (uint8_t)val;
    TWI_Start_Transceiver_With_Data(message, 3);
}


uint8_t motor_get_position(){
  /*Max and min values of the encoder output */
  uint8_t  encoder_min = 0; //Far right
  uint16_t encoder_max = ENCODER_MAX; //Far left
  int16_t  val;

  val = encoder_read();
  if(val < encoder_min){
    val = encoder_min;
  }
  if(val > encoder_max){
    val = encoder_max;
  }

  uint16_t pos = (uint16_t)(val/35.18);

  if (pos > 255) {
    return 255;
  }
  else if (pos < 0) {
    return 0;
  }
  else {
    return pos;
  }
}


void motor_calibrate(){

  /*Drive motor to right side of gameboard*/
  PORTH |= (1 << PH1); // set direction to right
  int8_t message[3];
  message[0] = MOTOR_ADDRESS_WRITE;
  message[1] = COMMAND_BYTE;
  message[2] = 100; // Drive slowly for 1.5 sec
  TWI_Start_Transceiver_With_Data(message,3);
  _delay_ms(1500);
  message[2] = 0;  // Stop
  TWI_Start_Transceiver_With_Data(message,3);
  _delay_ms(200);
  encoder_reset();
}

int16_t encoder_read(){
  //PINH |= (1 << PH6);

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

void encoder_reset() {
  PORTH &= ~(1 << PH6);
  _delay_us(200);
  PORTH |= (1 << PH6);
}
