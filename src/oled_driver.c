#include "oled_driver.h"
#include <string.h>
#include <math.h>
#include "fonts.h"
#include <avr/pgmspace.h>
#include "adc_driver.h"
#include "menu.h"

volatile char* oled_command_address = (char*)0x1000;
volatile char* oled_data_address = (char*)0x1200;    //   sjekk h-fil
static uint8_t PAGE, COLUMN; // Keep track of cursor position

//const char* const font[] PROGMEM = {font8, font5, font4};

void oled_write_c(uint8_t command){
  *oled_command_address = command;
}

void oled_write_d(uint8_t data){
  *oled_data_address = data;
}



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
  oled_write_c(0x00);
  oled_write_c(0xdb);        //VCOM  deselect  level  mode
  oled_write_c(0x30);
  oled_write_c(0xad);        //master  configuration
  oled_write_c(0x00);
  oled_write_c(0xa4);        //out  follows  RAM  content
  oled_write_c(0xa6);        //set  normal  display
  oled_write_c(0xaf);        //display  on


  oled_write_c(0x21);   // Addressing mode
  oled_write_c(0);      // Horizontal Mode
  oled_write_c(127);
  oled_write_c(0x22);
  oled_write_c(0);
  oled_write_c(7);

  // oled_write_c(0x20);   // Addressing mode setup Horizontal
  // oled_write_c(2);      // Page addressing mode
  // oled_write_c(127);
  // oled_write_c(0x22);
  // oled_write_c(0);
  // oled_write_c(7);

}

void oled_print_char(unsigned char c){

    int output = c - 32;

    for(int i = 0; i<8; i++){

        *oled_data_address = pgm_read_byte(&font8[output][i]);
    }

}

void oled_print_string(char* str){
    int length = strlen(str);
    for(int i = 0; i < length; i++){
        oled_print_char(str[i]);
    }
}


void oled_reset(void){
  for(int rows =0; rows < 8; rows++){
    for(int columns = 0; columns < 127; columns++)
        *oled_data_address=0x00;
    }
}

void oled_goto_line(int line){

    oled_write_c(0x22); //Choosing page/line (synonyms)
    oled_write_c(line);
    oled_write_c(line+1);

    oled_write_c(0x21); //Moving cursor to left
    oled_write_c(0);
    oled_write_c(127);
}

void oled_goto_column(int column){
    oled_write_c(0x21);
    oled_write_c(column);
    oled_write_c(127);
}

void oled_home(void){
  oled_write_c(0x22); //Choosing page/line 1
    oled_write_c(0);
    oled_write_c(1);

    oled_write_c(0x21); //Moving cursor to left
    oled_write_c(0);
    oled_write_c(127);
}

void oled_clear_line(int line){
  oled_goto_line(line);
  for(int i=0; i<127; i++){
    oled_write_d(0x00);
  }
}

void oled_pos(int row,int column){
  oled_write_c(0x22); //Choosing page/line (synonyms)
    oled_write_c(row);
    oled_write_c(row+1);

    oled_write_c(0x21); //Moving cursor to column
    oled_write_c(column);
    oled_write_c(127);
}

void oled_print_menu(Menu* menu, uint8_t line) {
  oled_reset();
  _delay_ms(200);
  oled_home();
  oled_goto_column(15);
  oled_print_string(menu->name); //print menu name on top

  for (int i = 0; i < menu->num_sub_menu; i++ ) {
    oled_goto_line(i+1);
    oled_goto_column(15);
    oled_print_string(menu->sub_menu[i]->name);
  }
  oled_print_arrow(line);
}

void oled_print_arrow(uint8_t line){
  oled_goto_line(line);
  oled_goto_column(0);
  oled_print_string("->");
}
