/*	Author: Brian Coffey
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 5  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding tmplate or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char tmpA = 0x00;
    unsigned char tmpC = 0x00;

    while (1) {
        tmpA = ~PINA & 0x0F;
        tmpC = 0x00;

        if (tmpA <= 2 && tmpA >= 1) {
            tmpC = tmpC | 0x20;
        }
        else if (tmpA <= 4 && tmpA >= 3) {
            tmpC = tmpC | 0x30;
        }
        else if (tmpA <= 6 && tmpA >= 5) {
            tmpC = tmpC | 0x38;
        }
        else if (tmpA <= 9 && tmpA >= 7) {
            tmpC = tmpC | 0x3C;
        }
        else if (tmpA <= 12 && tmpA >= 10) {
            tmpC = tmpC | 0x3E;
        }
        else if (tmpA <= 15 && tmpA >= 13) {
            tmpC = tmpC | 0x3F;
        }

        if (tmpA <= 4) {
            tmpC = tmpC | 0x40;
        }
        
        PORTC = tmpC;
    }
    return 1;
}
