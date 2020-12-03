/*	Author: bcoff002
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 #include <avr/io.h>
 #ifdef _SIMULATE_
 #include "simAVRHeader.h"
 #endif

 //#include "RIMS.h"
 enum States {start, OFF_R, OFF_P, ON_R, ON_P} state;

 void tickfunc() {
     unsigned char A0 = PINA & 0x01;
     switch(state) {
         case start:
             state = OFF_R;
             break;
         case OFF_R:
             state = A0 ? ON_P : OFF_R;
             break;
         case ON_P:
             state = A0 ? ON_P : ON_R;
             break;
         case ON_R:
             state = A0 ? OFF_P : ON_R;
             break;
         case OFF_P:
             state = A0 ? OFF_P : OFF_R;
             break;
         default:
             state = OFF_R;
             break;
     }

     switch(state) {
         case OFF_R:
             PORTB = 0x01;
             break;
         case ON_P:
             PORTB = 0x02;
             break;
         case ON_R:
             PORTB = 0x02;
             break;
         case OFF_P:
             PORTB = 0x01;
             break;
         default:
             PORTB = 0x00;
             break;
     }
 }

 int main(void) {
     DDRA = 0x00; PORTA = 0xFF;
     DDRB = 0xFF; PORTB = 0x00;
     while (1) {
         tickfunc();
     }
     return 1;
 }
