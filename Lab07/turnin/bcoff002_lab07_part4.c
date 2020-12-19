/*	Author: Brian Coffey
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 7  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 <DEMO> https://www.youtube.com/watch?v=I9Meypn3ds8
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    PINA = ~PINA;

    ADC_init();

    while (1) {
        unsigned short adc = ADC;
        if (adc >= 23) {
            PORTB = 0x01;
        }
        if (adc > 46) {
            PORTB = 0x03;
        }
        if (adc > 69) {
            PORTB = 0x07;
        }
        if (adc > 92) {
            PORTB = 0x0F;
        }
        if (adc > 115) {
            PORTB = 0x10;
        }
        if (adc > 138) {
            PORTB = 0x30;
        }
        if (adc > 161) {
            PORTB = 0x70;
        }
        if (adc > 184) {
            PORTB = 0xF0;
        }
    }
    return 1;
}
