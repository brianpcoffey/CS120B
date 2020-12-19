/* Author: Brian Coffey
 * Partner(s) Name (if applicable):
 * Assignment: Lab #12  Exercise #1
 * Exercise Description:
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: Youtube URL> https://www.youtube.com/watch?v=QnZ6sAO-QW0
 */

 #include <avr/io.h>
 #include <avr/interrupt.h>
 #ifdef _SIMULATE_
 #include "simAVRHeader.h"
 #endif

 #include "timer.h"

const unsigned char pattern = 0xFF;
unsigned char row = 0xFE;
unsigned char tempA = 0x00;

enum states {init, wait, down, down_pressed, up, up_pressed} state;

void LED_Tick() {
    tempA = ~PINA & 0x03;
    switch (state){
        case init:
            state = wait;
            break;
        case wait:
            if (tempA == 0x01) {
                state = down;
            }
            else if (tempA == 0x02) {
                state = up;
            }
            else {
                state = wait;
            }
            break;
        case down:
            state = down_pressed;
            break;
        case down_pressed:
            if (tempA == 0x00) {
                state = wait;
            }
            else if (tempA == 0x01) {
                state = down_pressed;
            }
            break;
        case up:
            state = up_pressed;
            break;
        case up_pressed:
            if (tempA == 0x00) {
                state = wait;
            }
            else if (tempA == 0x02) {
                state = up_pressed;
            }
            break;
        default:
            break;
    }

    switch (state){
        case init:
            row = 0xFE;
            break;
        case wait:
            break;
        case down:
            if (row > 0xEF) {
                row = (row << 1) | 0x01;
            }
            break;
        case down_pressed:
            break;
        case up:
            if (row < 0xFE) {
                row = (row >> 1) | 0x80;
            }
            break;
        case up_pressed:
            break;
        default:
            row = 0xFE;
            break;
    }
}
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    state = init;
    row = 0xFE;

    TimerSet(100);
    TimerOn();

    while (1) {
        LED_Tick();
        PORTC = pattern;
        PORTD = row;

        while (!TimerFlag)
          TimerFlag = 0;
    }
    return 1;
}
