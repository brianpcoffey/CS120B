/*	Author: Brian Coffey
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 7  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 <DEMO> https://www.youtube.com/watch?v=GV81tIvJg_k
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
         PORTB = (char) adc;
         PORTD = (char) (adc >> 8);
     }
     return 1;
 }
