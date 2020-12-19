/* Author: Brian Coffey
 * Partner(s) Name (if applicable):
 * Assignment: Lab #12  Exercise #3
* Exercise Description:
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: Youtube URL> https://www.youtube.com/watch?v=ztsKg60ufQw
 */

 #include <avr/io.h>
 #include <avr/interrupt.h>
 #ifdef _SIMULATE_
 #include "simAVRHeader.h"
 #endif

 #include "timer.h"

unsigned char pattern = 0x00;
unsigned char row = 0x00;
unsigned char i = 0x00;

enum states {rectangle} state;

void LED_Tick() {
    switch (state){
        case rectangle:
            state = rectangle;
            break;
        default:
            break;
    }

    switch (state){
        case rectangle:
            if (i == 3) {
                i = 0;
                row = 0xFE;
            }

            if (i == 0 || i == 2) {
                pattern = 0x3C;
            }
            else if (i == 1) {
                pattern = 0x24;
            }
            i++;
            row = (row << 1) | 0x01;
            break;
        default:
            break;
    }
}
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    state = rectangle;
    row = 0xFE;

    TimerSet(1);
    TimerOn();

    while (1) {
        LED_Tick();
        PORTC = pattern;
        PORTD = row;

        while (!TimerFlag) {}
        TimerFlag = 0;
    }
    return 1;
}
