/*	Author: Brian Coffey
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 5  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding tmplate or example
 *	code, is my own original work.
 <DEMO> https://www.youtube.com/watch?v=WXaKQG6LVpE
 */
 #include <avr/io.h>
 #ifdef _SIMULATE_
 #include "simAVRHeader.h"
 #endif

 enum States {start, init, center, left, right, reset} state;

 void Tick() {

     unsigned char tmpA = ~PINA;
     unsigned char tmpC = PORTC;

     switch(state) {
         case start:
             state = init;
             break;

         case init:
             if (tmpA == 0x01) {
                 state = center;
             }
             else {
                 state = init;
             }
             break;

         case center:
             if (tmpA == 0x01) {
                 state = left;
             }
             else {
                 state = center;
             }
             break;

         case left:
             if (tmpA == 0x01) {
                 state = right;
             }
             else {
                 state = left;
             }
             break;

         case right:
             if (tmpA == 0x01) {
                 state = init;
             }
             else {
                 state = left;
             }
             break;

         case reset:
             state = init;
             break;

         default:
             state = init;
             break;
     }

     switch(state) {
         case init:
             tmpC = 0x3F;
             break;

         case center:
             tmpC = 0x0C;
             break;

         case left:
             tmpC = 0x03;
             break;

         case right:
             tmpC = 0x30;
             break;

         case reset:
             tmpC = 0x00;
             break;

         default:
             break;
     }

     PORTC = tmpC;

 }

 int main(void) {
     DDRA = 0x00; PORTA = 0xFF;
     DDRC = 0xFF; PORTC = 0x00;

     while (1) {
         Tick();
     }
     return 1;
 }
