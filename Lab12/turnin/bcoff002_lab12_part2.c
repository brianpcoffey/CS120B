/* Author: Brian Coffey
 * Partner(s) Name (if applicable):
 * Assignment: Lab #12  Exercise #2
* Exercise Description:
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: Youtube URL> https://www.youtube.com/watch?v=Bq-0ArdxYY8
 */

 #include <avr/io.h>
 #include <avr/interrupt.h>
 #ifdef _SIMULATE_
 #include "simAVRHeader.h"
 #endif

 #include "timer.h"

unsigned char pattern = 0x80;
unsigned char row = 0x00;
unsigned char tempA = 0x00;

enum states {init, wait, left, left_pressed, right, right_pressed} state;

void LED_Tick() {
    tempA = ~PINA & 0x03;
    switch (state){
        case init:
            state = wait;
            break;
        case wait:
            if (tempA == 0x01) {
                state = left;
            }
            else if (tempA == 0x02) {
                state = right;
            }
            else {
                state = wait;
            }
            break;
        case left:
            state = left_pressed;
            break;
        case left_pressed:
            if (tempA == 0x00) {
                state = wait;
            }
            else if (tempA == 0x01) {
                state = left_pressed;
            }
            break;
        case right:
            state = right_pressed;
            break;
        case right_pressed:
            if (tempA == 0x00) {
                state = wait;
            }
            else if (tempA == 0x02) {
                state = right_pressed;
            }
            break;
        default:
            break;
    }

    switch (state){
        case init:
            pattern = 0x80;
            break;
        case wait:
            break;
        case left:
            if (pattern < 0x80) {
                    pattern = (pattern << 1);
            }
            break;
        case left_pressed:
            break;
        case right:
            if (pattern > 0x01) {
                pattern = (pattern >> 1);
            }
            break;
        case right_pressed:
            break;
        default:
            break;
    }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    state = init;
    row = 0x00;

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
