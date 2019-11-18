#define F_CPU 16000000
#include "music_driver.h"
#include "can_driver.h"
#include "pitches.h"
#include "songs.h"
#include <stdio.h>
#include <util/delay.h>
#include <float.h>
#include <avr/io.h>
#include <avr/interrupt.h>


void music_init(){

    /*Set PE3 as pwm output*/
    DDRE |= (1 << PE3);

    /*Mode 12 CTC*/
    TCCR3B |= (1 << WGM33) | (1 << WGM32);

    /*For generating a waveform output in CTC
    mode*/
    TCCR3A = (1 << COM3A0);

    /*64 bit Prescaler*/
    TCCR3B |= (1 << CS31) | ( 1 << CS30);


}


void music_play(song title){
  int size = 0;
  switch (title) {
    case MARIO:
        size = sizeof(mario_melody)/sizeof(int);
        //size = 49;
        //Iterate through all notes of the song
        for (int current_note = 0; current_note < size; current_note++){
          if (get_CAN_msg().id == 7 && get_CAN_msg().data[0] == 0){
             music_buzzer(0,0);
             return;
          }
            //Calculating the note duration - 1sec/Notetype eg. 1000/8
            int note_duration = 1000 / mario_tempo[current_note];

            music_buzzer(mario_melody[current_note], note_duration);

            //Small delay between notes
            int note_delay = note_duration * 2;
            for(int i = 1; i < note_delay; i++){
            _delay_ms(1);
          }
        }
        break;
    case SOVJET:
      size = sizeof(sovjet_melody)/sizeof(int);
      //size = 49;
      //Iterate through all notes of the song
      for (int current_note = 0; current_note < size; current_note++){
        if (get_CAN_msg().id == 7 && get_CAN_msg().data[0] == 0){
           music_buzzer(0,0);
           return;
        }
          //Calculating the note duration - 1sec/Notetype eg. 1000/8
          int note_duration = 1000 / sovjet_tempo[current_note];

          music_buzzer(sovjet_melody[current_note], note_duration);

          //Small delay between notes
          int note_delay = note_duration * 2.2;
          for(int i = 1; i < note_delay; i++){
          _delay_ms(1);
        }
      }
      break;
  }
}


void music_buzzer(float freq, int length){

  float clock = F_CPU;
  int pwm_signal = (clock/(2*64*freq)) - 1;
  /*IC3 defines top value of counter, hence our freq*/
  ICR3 = pwm_signal;
  for (int i = 1; i < length; i++){
        _delay_us(50);
  }

}

void set_tone(float freq){
  float clock = F_CPU;
  int pwm_signal = (clock/(2*64*freq)) - 1;
  /*IC3 defines top value of counter, hence our freq*/
  ICR3 = pwm_signal;


}
