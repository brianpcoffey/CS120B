/* Author: Brian Coffey
 * Partner(s) Name (if applicable):
 * Lab Section: 24
 * Assignment: Lab #13  Exercise #3
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
unsigned char time = 0;
unsigned char i = 0x00;
unsigned char speed = 0;
unsigned short level[] = {200, 300, 400, 500, 600, 700, 800, 900};

enum displayStates {display_display} display_state;
enum shiftStates {shift_init, shift_wait, shift_left, shift_right} shift_state;
enum speedStates {speed_getPeriod} speed_state;

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
          	pattern = 0x80;
          	row = 0xEF;
          	time = 0;
            break;
        case shift_wait:
            if (x <= level[3]) {
                shift_state = shift_left;
                time = 0;
            }
            else if (x >= level[4]) {
                shift_state = shift_right;
                time = 0;
            }
            else {
                shift_state = shift_wait;
            }
            break;
        case shift_left:
            if (x > level[3]) {
              shift_state = shift_wait;
            }
            else {
              shift_state = shift_left;
            }
            break;
        case shift_right:
            if (x < level[4]) {
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
            if (time % speed == 0) {
              if (pattern < 0x80) {
                      pattern = (pattern << 1);
              }
              else {
                  pattern = 0x01;
              }
            }
            time++;
            break;
        case shift_right:
          if (time % speed == 0) {
            if (pattern > 0x01) {
                pattern = (pattern >> 1);
            }
            else {
                pattern = 0x80;
            }
          }
          time++;
          break;
        default:
            break;
    }
}

void speedTick() {
    switch (speed_state) {
        case speed_getPeriod:
            speed_state = speed_getPeriod;
            break;
        default:
            break;
    }

    switch (speed_state) {
        case speed_getPeriod:
            if (((x > 600) && (x <= 700)) || ((x < 500) && (x >= 400))) {
                speed = 20;
            }
            else if (((x > 700) && (x <= 800)) || ((x < 400) && (x >= 300))) {
                speed = 10;
            }
            else if (((x > 800) && (x <= 900)) || ((x < 300) && (x >= 200))) {
                speed = 5;
            }
            else if ((x > 900) || (x < 200)) {
                speed = 2;
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
    speed_state = speed_getPeriod;

    ADC_init();

    TimerSet(50);
    TimerOn();

  	while (1) {
      x = ADC;
      speedTick();
      shiftTick();
      displayTick();

      while (!TimerFlag)
      TimerFlag = 0;

  	}
    return 1;
}
