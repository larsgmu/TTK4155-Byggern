#include "can_driver.h"
#include "mcp2515_driver.h"
#include "MCP2515.h"
#include <stdio.h>

//int can_interrupt_flag = 0;

void can_init() {
  mcp2515_init();

  /*loopback mode*/
  mcp2515_write(MCP_CANCTRL, MODE_LOOPBACK);

  /*Turn mask/filters off*/
  mcp2515_write(MCP_RXB0CTRL, 0b01100000);

  /*Generate interrupt when received message kissmypuiss*/
  mcp2515_bit_modify(MCP_CANINTE, 0b00000001, 1);

  /* clear interrupt flag*/
  mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0);

  /*Set interrupt on PD2 to falling edge */
  MCUCR |= (1 << ISC01);

  /*Enable interrupt  on PD2*/
  GICR |= (1 << INT0);

  /*Clear interrupt flag on PD2*/
  GIFR |= (1 << INTF0);
}

void can_send_msg(CANmsg* can_msg) {

  mcp2515_write(MCP_TXB0SIDH, 0);
  mcp2515_write(MCP_TXB0SIDL, can_msg->id << 5);
  mcp2515_write(MCP_TXB0DLC, can_msg->length);

  for (int length = 0; length < can_msg->length; length++){
    mcp2515_write(MCP_TXB0D0 + length, can_msg->data[length]);
  }
  mcp2515_request_send(0);

}

CANmsg can_receive_msg() {
  CANmsg msg;
  msg.length = mcp2515_read(MCP_RXB0DLC);
  msg.id = ( (mcp2515_read(MCP_RXB0SIDH) >> 3) + (mcp2515_read(MCP_RXB0SIDL) >> 5) );
  for (int i = 0; i < msg.length; i++){
    msg.data[i] = mcp2515_read(MCP_RXB0D0 + i);
  }
  mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0);
  return msg;
}

ISR(INT0_vect){
  cli();
  can_receive_msg();
  sei();
}
