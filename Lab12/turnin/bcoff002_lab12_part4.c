/* Author: Brian Coffey
 * Partner(s) Name (if applicable):
 * Assignment: Lab #12  Exercise #4
* Exercise Description:
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: Youtube URL>
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "timer.h"

unsigned char rowArray[3] = {0xFD, 0xFB, 0xF7};
unsigned char patternArray[3] = {0x3C, 0x24, 0x3C};
unsigned char tempA = 0x00;

enum states {start, wait, up, up_pressed, down, down_pressed, left, left_pressed, right, right_pressed} state;
enum display_states {display_row1, display_row2, display_row3} display_state;

void LED_Tick() {
  switch (state) {
    case wait:
      if (tempA == 0x01) {
        state = up;
      }
      else if (tempA == 0x02) {
        state = down;
      }
      else if (tempA == 0x04) {
        state = left;
      }
      else if (tempA == 0x08) {
        state = right;
      }
      else {
        state = wait;
      }
      break;
    case up:
      state = up_pressed;
      break;
    case up_pressed:
      if (tempA != 0x00) {
        state = up_pressed;
      }
      else {
        state = wait;
      }
      break;
    case down:
      state = down_pressed;
      break;
    case down_pressed:
      if (tempA != 0x00) {
        state = down_pressed;
      }
      else {
        state = wait;
      }
      break;
    case left:
      state = left_pressed;
      break;
    case left_pressed:
      if (tempA != 0x00) {
        state = left_pressed;
      }
      else {
        state = wait;
      }
      break;
    case right:
      state = right_pressed;
      break;
    case right_pressed:
      if (tempA != 0x00) {
        state = right_pressed;
      }
      else {
        state = wait;
      }
      break;
    default:
      state = wait;
      break;
  }

  switch (state) {
    case wait:
      break;
    case up:
      if (rowArray[0] < 0xFE) {
        for (unsigned char i = 0; i < 3; i++) {
          rowArray[i] = rowArray[i] >> 1 | 0x80;
        }
      }
      break;
    case up_pressed:
      break;
    case down:
      if (rowArray[0] > 0xFB) {
        for (unsigned char i = 0; i < 3; i++) {
          rowArray[i] = rowArray[i] << 1 | 0x01;
        }
      }
      break;
    case down_pressed:
      break;
    case left:
      if (patternArray[0] < 0xF0) {
        for (unsigned char i = 0; i < 3; i++) {
          patternArray[i] = patternArray[i] << 1;
        }
      }
      break;
    case left_pressed:
      break;
    case right:
      if (patternArray[0] > 0x0F) {
        for (unsigned char i = 0; i < 3; i++) {
          patternArray[i] = patternArray[i] >> 1;
        }
      }
      break;
    case right_pressed:
      break;
    default:
      break;
  }
}

void LED_Display() {
    switch (display_state){
        case display_row1:
            display_state = display_row2;
            break;
        case display_row2:
            display_state = display_row3;
            break;
        case display_row3:
            display_state = display_row1;
            break;
        default:
            display_state = display_row1;
            break;
    }

    switch (display_state){
        case display_row1:
            PORTC = patternArray[0];
            PORTD = rowArray[0]
            break;
        case display_row2:
            PORTC = patternArray[1];
            PORTD = rowArray[1];
            break;
        case display_row3:
            PORTC = patternArray[2];
            PORTD = rowArray[2];
            break;
        default:
            break;
    }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    state = wait;
    display_state = display_row1;

    unsigned char time = 0;

    TimerSet(1);
    TimerOn();

    while (1) {
        tempA = ~PINA & 0x0F;
        if (time >= 100) {
          LED_Tick();
          time = 0;
        }
        LED_Display();

        while (!TimerFlag) {}
        TimerFlag = 0;
        time++;
    }
    return 1;
}
