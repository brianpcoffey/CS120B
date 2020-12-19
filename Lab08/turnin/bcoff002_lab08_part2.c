/*	Author: Brian Coffey
 *  Partner(s) Name:
 *	Lab Section: 24
 *	Assignment: Lab #8  Exercise #2
 *	Exercise Description:
 *  Using the ATmega1284’s PWM functionality, design a system where the notes: C4, D, E, F, G, A, B, and C5,
 *  from the table at the top of the lab, can be generated on the speaker by scaling Up or Down the eight note scale.
 *  Three buttons are used to control the system. One button toggles sound on/off.
 *  The other two buttons scale Up, or Down, the eight note scale. Criteria:
 *  The system should scale Up/Down one note per button press.
 *  When scaling Down, the system should not scale below a C.
 *  When scaling Up, the system should not scale above a C.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://www.youtube.com/watch?v=xAr6JNd9yqc
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
    static double current_frequency;

    if (frequency != current_frequency) {
        if (!frequency) {
            TCCR3B &= 0x08;
        }
        else {
            TCCR3B |= 0x03;
        }
        if (frequency < 0.954){
            OCR3A = 0xFFFF;
        }
        else if (frequency > 31250) {
            OCR3A = 0x0000;
        }
        else {
            OCR3A = (short)(8000000 / (128 * frequency)) - 1;
        }

        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

enum States {Start, Wait, Power, Up, Down, Release} state;

unsigned char tempA = 0x00;
unsigned char status = 0;
unsigned char freq = 0x00;
unsigned char i = 0x00;
double frequencyArray [] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493, 523.25};

void Tick() {
    switch(state) {
        case Start:
            state = Wait;
            break;
        case Wait:
            if (tempA == 0x01) {
                state = Power;
            }
            else if (tempA == 0x02) {
                state = Up;
            }
            else if (tempA == 0x04) {
                state = Down;
            }
            break;
        case Power:
            if (tempA == 0x01) {
                state = Power;
            }
            else {
                state = Release;
            }
            break;
        case Up:
            if (tempA == 0x02) {
                state = Up;
            }
            else {
                state = Release;
            }
            break;
        case Down:
            if (tempA == 0x04) {
                state = Down;
            }
            else {
                state = Release;
            }
            break;
        case Release:
            state = Wait;
            break;
        default:
            state = Start;
            break;
    }

    switch(state) {
        case Start:
            i = 0x00;
            status = 0;
            break;
        case Wait:
            if (status) {
                set_PWM(frequencyArray[i]);
            }
            else {
                set_PWM(0);
            }
            break;
        case Power:
            freq = tempA;
            break;
        case Up:
            freq = tempA;
            break;
        case Down:
            freq = tempA;
            break;
        case Release:
            if (freq == 0x01) { // on/off
                status ^= 1;
            }
            else if (freq == 0x02 && i < 0x07) { // upper bound
                i++;
            }
            else if (freq == 0x04 && i > 0x00) { // lower bound
                i--;
            }
            break;

      	 default:
            i = 0x00;
            status = 0;
    	      break;
    }
}


int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x40;

    PWM_on();

    state = Start;
    i = 0;
    status = 0;

    while (1) {
        tempA = ~PINA;
        Tick();
    }

    PWM_off();
    return 1;
}
