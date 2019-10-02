#include "can_driver.h"
#include "mcp2515_driver.h"

void can_init() {
  mcp2515_init();

  //loopback mode
  mcp2515_write(MCP_CANCTRL, MODE_LOOPBACK);

  //Turn mask/filters off
  mcp2515_write(MCP_RXB0CTRL, 0b01100000);

  //Generate interrupt when recieved message kissmypuiss
  mcp2515_bit_modify(MCP_CANINTE, 0b00000001, 1);
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

CANmsg can_recieve_msg() {
  //kiss mp
}
