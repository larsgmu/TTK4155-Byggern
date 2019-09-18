#include "oled_driver.h"
#include <string.h>
#include <math.h>
#include "fonts.h"
#include <avr/pgmspace.h>
#include "adc_driver.h"

volatile char* oled_command_address = (volatile char*)0x1000;
volatile char* oled_data_address = (volatile char*)0x1200;    //   sjekk h-fil
static uint8_t PAGE, COLUMN; // Keep track of cursor position

//const char* const font[] PROGMEM = {font8, font5, font4};

void oled_init()   {
  oled_write_c(0xae);        //  display  off
  oled_write_c(0xa1);        //segment  remap
  oled_write_c(0xda);        //common  pads  hardware:  alternative
  oled_write_c(0x12);
  oled_write_c(0xc8);        //common  output scan direction:com63~com0
  oled_write_c(0xa8);        //multiplex  ration  mode:63
  oled_write_c(0x3f);
  oled_write_c(0xd5);        //display  divide ratio/osc. freq. mode
  oled_write_c(0x80);
  oled_write_c(0x81);        //contrast  control
  oled_write_c(0x50);
  oled_write_c(0xd9);        //set  pre-charge  period
  oled_write_c(0x21);
  oled_write_c(0x20);        //Set  Memory  Addressing  Mode
  oled_write_c(0x02);
  oled_write_c(0xdb);        //VCOM  deselect  level  mode
  oled_write_c(0x30);
  oled_write_c(0xad);        //master  configuration
  oled_write_c(0x00);
  oled_write_c(0xa4);        //out  follows  RAM  content
  oled_write_c(0xa6);        //set  normal  display
  oled_write_c(0xaf);        //display  on
}

void oled_write_c(uint8_t command){
  *oled_command_address = command;
}

void oled_write_d(uint8_t data){
  oled_data_address[0] = data;
}

void oled_clear_display() {
  for (int page = 0; page < 8; page++) {
    oled_go_to_page(page);
    for (int col = 0; col < 128; col++) {
      oled_data_address[col] = 0;
    }
  }
}

void oled_go_to_page(uint8_t page){
  PAGE = page;
  page = page + 0xB0;

  *oled_command_address = page;
  *oled_command_address = 0x00;
  *oled_command_address = 0x10;
}

void oled_top_left(void){
  PAGE = 0;
  COLUMN = 0;

  *oled_command_address = 0xB0; //Set GDDRAM Page Start Address
  *oled_command_address = 0x00; //Set the lower nibble of the column start address register for Page Addressing Mode
  *oled_command_address = 0x10; // Horizontal Addressing Mode

}

// void oled_print_char(unsigned char c) {
//   unsigned char pc = c - 32;
//
//   for(int page = 0; page < 8; page++){
//     for(int col = 0; col < 128; col++){
//
//       oled_data_address[page*128 + col] = (pgm_read_word(&(font8[pc][page])));
//       //oled_write_d(pgm_read_byte(&font5[pc][i]))
//     }
//   }
// }

void OLED_print(unsigned char* c)
{
    int length = strlen(c);//FORSTESS HER FREDAG, SE BILDE PÅ MOBILEN
		for(int i = 0; i < 8; i++)
		{
      oled_go_to_page(i);
		  oled_data_address[0] = (pgm_read_byte(&(font8[c - ASCII_OFFSET][i])));
		}
}

void oled_reset(void){
  oled_write_c(0xb0); //Choose page 1
  oled_write_c(0x21); //Set size of col
  oled_write_c(0x00); //low = 0
  oled_write_c(0x7f); //to high = width = 128
  oled_write_c(0x22); //Select size of row
  oled_write_c(0x00);
  oled_write_c(0x07);
  for(int rows =0; rows < 8; rows++){
    for(int cols =0; cols<128; cols++){
      oled_write_d(0x00);
    }
    oled_write_c(0xb0 + rows);
  }
  oled_write_c(0xb0);

}
