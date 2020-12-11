/* Author: Brian Coffey
 * Partner(s) Name (if applicable):
 * Lab Section: 24
 * Assignment: Lab #13  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: Youtube URL>
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "timer.h"

const unsigned char row = 0xFE;
unsigned char pattern = 0x80;
unsigned short x = 0x00;
const unsigned short max = 0x3E7;
const unsigned short min = 0x1F;

enum displayStates {display_display} display_state;
enum shiftStates {shift_init, shift_wait, shift_left, shift_right} shift_state;

void ADC_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

void displayTick() {
    switch (display_state) {
        case display_display:
            display_state = display_display;
            break;
        default:
            display_state = display_display;
            break;
    }

    switch (display_state) {
        case display_display:
            PORTD = row;
            PORTC = pattern;
            break;
        default:
            PORTD = row;
            PORTC = pattern;
            break;
    }
}

void shiftTick() {
    switch (shift_state) {
        case shift_init:
            shift_state = shift_wait;
            break;
        case shift_wait:
            if (x <= (min + 10)) {
                shift_state = shift_left;
            }
            else if (x >= (max - 10)) {
                shift_state = shift_right;
            }
            else {
                shift_state = shift_wait;
            }
            break;
        case shift_left:
            if (x > (min + 10)) {
              shift_state = shift_wait;
            }
            else {
              shift_state = shift_left;
            }
            break;
        case shift_right:
            if (x < (min - 10)) {
              shift_state = shift_wait;
            }
            else {
              shift_state = shift_right;
            }
            break;
        default:
            break;
    }

    switch (shift_state) {
        case shift_init:
            pattern = 0x80;
            break;
        case shift_wait:
            break;
        case shift_left:
            if (pattern < 0x80) {
                    pattern = (pattern << 1);
            }
            else {
                pattern = 0x01;
            }
            break;
        case shift_right:
            if (pattern > 0x01) {
                pattern = (pattern >> 1);
            }
            else {
                pattern = 0x80;
            }
            break;
        default:
            break;
    }
}

int main(void) {
		DDRA = 0x00; PORTA = 0xFF;
		DDRC = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    display_state = display_display;
    shift_state = shift_init;

    ADC_init();

    TimerSet(100);
    TimerOn();

  	while (1) {
  	  x = ADC;
      displayTick();
      shiftTick();

      while (!TimerFlag)
      TimerFlag = 0;
	  }
    return 1;
}
