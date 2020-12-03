
/*	Author: bcoff002
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 3  Exercise 5
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
 DDRA = 0xFF; PORTA = 0x00;
 DDRB = 0xFE; PORTB = 0x00;
 DDRD = 0x00; PORTC = 0xFF;

 unsigned short weight = 0;

 while (1) {
   if (!(PINB)) {
       weight = PIND + PINB;
   }
   else {
       weight = PIND * 2;
   }
   PORTA = weight;
   if (weight >= 70) {
       PORTB = 0x02;
   }
   else if (weight > 5 && weight < 70) {
       PORTB = 0x04;
   }
   else if (weight <= 5) {
       PORTB = 0x00;
   }

   weight = 0;
   }
   return 1;
}
