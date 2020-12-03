
/*	Author: bcoff002
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 2  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif	

int main(void) {
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0x00; PORTC = 0xFF;
    DDRD = 0xFF; PORTD = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
    
    unsigned short total_weight = 0x00; //Total weight of A, B, and C
    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char tmpC = 0x00;
    
    unsigned char over_cap = 0x01;
    unsigned char balanced = 0x01;

    unsigned char ret = 0x00;

    while(1) {
        // 1) Read input
        tmpA = PINA;
        tmpB = PINB;
        tmpC = PINC;

        // 2) Perform computation
        
        total_weight = tmpA + tmpB + tmpC;

        if (total_weight > 0x8C) {
            over_cap = 0x01;
        }
        
        if (tmpA > tmpC) {
            if (tmpA - tmpC > 0x50)
                balanced = 0x01;
        }
        else {
            if (tmpC - tmpA > 0x50)
                balanced = 0x01;
        }
        
        if (balanced == 0x01 && over_cap == 0x01)
            ret = (total_weight & 0xFC) | 0x03;
        if (balanced == 0x01 && over_cap == 0x00)
            ret = (total_weight & 0xFC) | 0x02;
        if (balanced == 0x00 && over_cap == 0x01)
            ret = (total_weight & 0xFC) | 0x01;
        else 
           ret = total_weight;

        // 3) Write output

        PORTD = ret;
    }
    return 0;
}
