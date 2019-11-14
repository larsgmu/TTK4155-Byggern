/*!@file
* This file contains functions to display data on the game controller OLED screen.
*/
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "fonts.h"
#include "oled_driver.h"
#include "sram_driver.h"

static volatile char* oled_command_address = (char*)0x1000;
static volatile char* oled_data_address = (char*)0x1200;    //   sjekk h-fil
static volatile char* oled_sram_adress = (char*)0x1C00;

static struct oled_data_marker_struct oled_state;

//const char* const font[] PROGMEM = {font8, font5, font4};
//

void oled_init()   {
  oled_write_c(0xae);        // display  off
  oled_write_c(0xa1);        // segment  remap
  oled_write_c(0xda);        // common  pads  hardware:  alternative
  oled_write_c(0x12);
  oled_write_c(0xc8);        // common  output scan direction:com63~com0
  oled_write_c(0xa8);        // multiplex  ration  mode:63
  oled_write_c(0x3f);
  oled_write_c(0xd5);        // display  divide ratio/osc. freq. mode
  oled_write_c(0x80);
  oled_write_c(0x81);        // contrast  control
  oled_write_c(0x50);
  oled_write_c(0xd9);        // set  pre-charge  period
  oled_write_c(0x21);

  oled_write_c(0x20);        //Set  Memory  Addressing  Mode
  oled_write_c(0x00);							// 0x00: Horizontal Adressing Mode
  oled_write_c(0xdb);        //VCOM  deselect  level  mode
  oled_write_c(0x30);
  oled_write_c(0xad);        // master  configuration
  oled_write_c(0x00);
  oled_write_c(0xa4);        // out  follows  RAM  content
  oled_write_c(0xa6);        // set  normal  display
  oled_write_c(0xaf);        // display  on

  // /*  Want the timer to overflow every
  //60Hz = 0.01667s= 16.67ms                // CPU = 4915200 Hz

  /*Ensuring clear on compare mode*/
  TCCR0 &= ~(1 << WGM00);
  TCCR0 |= (1 << WGM01);

  /*Normal compare operation*/
  TCCR0 &= ~(1 << COM00);
  TCCR0 &= ~(1 << COM01);

  /*Setting Prescaler to 256 -> Gives TOP = 159 */
  TCCR0 |= (1 << CS00);
  TCCR0 &= ~(1 << CS01);
  TCCR0 |= (1 << CS02);

  OCR0 = 255;

  /*Enable interrupt on Timer Compare Match*/
  TIMSK |= (1 << OCIE0);
  /*Clear interrupt flag*/
  TIFR |= (1 << OCF0);

  oled_state.CHANGED = 1; //Draws main menu;
  oled_state.LINE = 0;
  oled_state.COL = 0;
  oled_state.COLOR = 1;


}

// void oled_set_brightness(char* c){
//   return;
// }
//
// void oled_flip_colors(char* c){
//   if (oled_state.COLOR) {
//     oled_write_c(0xa7);
//     oled_state.COLOR = 0;
//     return;
//   }
//   oled_write_c(0xa6);
//   oled_state.COLOR = 1;
// }

ISR(TIMER0_COMP_vect){

  if (!(oled_state.CHANGED)) {
    TCNT0 = 0;
    return;
  }
  oled_draw();
  oled_state.CHANGED = 0;
  TCNT0 = 0;
}


void oled_write_c(uint8_t command){
  *oled_command_address = command;
}

void oled_write_d(uint8_t data){
  *oled_data_address = data;
}

void oled_goto_line(int line){

    oled_state.LINE = line;

    oled_write_c(0x22); //Choosing page/line (synonyms)
    oled_write_c(line);
    oled_write_c(line+1);

    oled_write_c(0x21); //Moving cursor to left
    oled_write_c(0);
    oled_write_c(127);
}

void oled_goto_column(int column){

    oled_state.COL = column;

    oled_write_c(0x21);
    oled_write_c(column);
    oled_write_c(127);
}

void oled_home(void){

    oled_state.LINE = 0;
    oled_state.COL = 0;

    oled_write_c(0x22); //Choosing page/line 1
    oled_write_c(0);
    oled_write_c(1);

    oled_write_c(0x21); //Moving cursor to left
    oled_write_c(0);
    oled_write_c(127);
}

void oled_pos(int row,int column){
    oled_state.LINE = row;
    oled_state.COL = column;

    oled_write_c(0x22); //Choosing page/line (synonyms)
    oled_write_c(row);
    oled_write_c(row+1);

    oled_write_c(0x21); //Moving cursor to column
    oled_write_c(column);
    oled_write_c(127);
}


void oled_sram_write_d(uint8_t adr, uint8_t data){
  oled_state.CHANGED = 1;
  oled_sram_adress[adr] = data;
}

void oled_sram_write_char(unsigned char c){
    oled_state.CHANGED = 1;
    int output = c - 32;
    for(int i = 0; i<OLED_PAGE_HEIGHT; i++){
      oled_sram_adress[oled_state.LINE*OLED_COLS + oled_state.COL + i] = pgm_read_byte(&font8[output][i]);
    }
}

void oled_sram_write_string(char* str){
  int length = strlen(str);
  for(int i = 0; i < length; i++){
      oled_sram_write_char(str[i]);
      oled_state.COL += OLED_PAGE_HEIGHT;
  }
}



void oled_sram_reset(void){
    for(int row =0; row < 8; row++){
      //oled_goto_line(rows);
      for(int col = 0; col < OLED_COLS; col++){
        //oled_data_address[cols] = 0x00;
        oled_sram_adress[row*OLED_COLS + col] = 0x00;
      }
    }
    oled_home();
}


void oled_sram_clear_line(int line){
  oled_goto_line(line);
  //for(int i=0; i<127; i++){
    //oled_write_d(0x00);
  //}
  for(int col = 0; col < OLED_COLS; col++){
    oled_sram_adress[oled_state.LINE*OLED_COLS + col] = 0x00;
  }
}


void oled_sram_menu(Menu* menu) {
  oled_sram_reset();
  oled_home();
  if(menu->header != ""){
    oled_sram_write_string(menu->header);
  }else{
    oled_sram_write_string(menu->name); //print menu name on top
  }
  for (int i = 0; i < menu->num_sub_menu; i++ ) {
    oled_goto_line(i+1);
    oled_goto_column(15);
    oled_sram_write_string(menu->sub_menu[i]->name);
  }
  if (menu->info != "") {
    oled_goto_column(0);
    oled_goto_line(7);
    oled_sram_write_string(menu->info);
  }
}

void oled_sram_arrow(uint8_t line){
  oled_goto_line(line);
  oled_goto_column(0);
  oled_sram_write_string("~");
}

void oled_draw(){
  oled_home();
  for (int line = 0; line < OLED_PAGES; line++ ) {
    for (int col = 0; col < OLED_COLS; col++){
      oled_pos(line,col);
      oled_write_d(oled_sram_adress[line*OLED_COLS + col]);
    }
  }
}
