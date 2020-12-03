
/*	Author: bcoff002
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 3  Exercise 2
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
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char tempA = 0x00;
    unsigned char tempC = 0x00;

    while (1) {
        tempA = PINA;
        tempC = 0x00;

        if (tempA <= 2 && tempA >= 1) {
            tempC = tempC | 0x20;
        }
        else if (tempA <= 4 && tempA >= 3) {
            tempC = tempC | 0x30;
        }
        else if (tempA <= 6 && tempA >= 5) {
            tempC = tempC | 0x38;
        }
        else if (tempA <= 9 && tempA >= 7) {
            tempC = tempC | 0x3C;
        }
        else if (tempA <= 12 && tempA >= 10) {
            tempC = tempC | 0x3E;
        }
        else if (tempA <= 15 && tempA >= 13) {
            tempC = tempC | 0x3F;
        }
        else if (tempA > 15) {
            tempC = tempC | 0x70;
        }
        if (tempA <= 4) {
            tempC = tempC | 0x40;
        }

        PORTC = tempC;
    }
    return 1;
}
