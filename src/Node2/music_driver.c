#define F_CPU 16000000
#include "music_driver.h"
#include "pitches.h"
#include "songs.h"
#include <stdio.h>
#include <util/delay.h>
#include <float.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>

int size = 0;

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

  switch (title) {
    case MARIO:

        size = sizeof(mario_melody)/sizeof(int);
        //Iterate through all notes of the song
        for (int current_note = 0; current_note < size; current_note++){

            //Calculating the note duration - 1sec/Notetype eg. 1000/8
            float note_duration = 1000/mario_tempo[current_note];

            music_buzzer(mario_melody[current_note], note_duration);
            for (int i = 1; i < note_duration; i++){
                  _delay_ms(1);
              }
            //Small delay between notes
            int note_delay = note_duration * 1.30;
            for(int i = 1; i < note_delay; i++){
            _delay_ms(1);
          }

            //Stop current note
            music_buzzer(0, note_duration);
        }

  }



}


void music_buzzer(uint16_t freq, uint16_t length){

/*

pwm = clock / 2 * Prescaler

*/
  float pwm_signal = (F_CPU/(2*64*freq)) - 1;

  /*ICR3 defines top value of counter, hence our freq*/
  ICR3 = pwm_signal;


}
