/*	Author: Brian Coffey
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 6  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 <DEMO> https://www.youtube.com/watch?v=rGkVwHoT8QM
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;

    _avr_timer_cntcurr = _avr_timer_M;

    SREG |= 0x80;
}

void TimerOff() {
    TCCR1B = 0x00;
}

void TimerISR() {
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
    _avr_timer_cntcurr--;
    if (_avr_timer_cntcurr == 0) {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

enum States {start, zero, one, two} state;

void Tick() {
    switch (state) {
        case start:
            state = zero;
            break;

        case zero:
            state = one;
            break;

        case one:
            state =  two;
            break;

        case two:
            state = zero;
            break;

        default:
            state = start;
            break;
    }

    switch (state) {
        case zero:
            PORTB = 0x01;
            break;

        case one:
            PORTB = 0x02;
            break;

        case two:
            PORTB = 0x04;
            break;

        default:
            PORTB = 0x01;
            break;
    }
}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(1000);
    TimerOn();

    state = start;
    while (1) {
        Tick();
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
