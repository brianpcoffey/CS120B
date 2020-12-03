/*	Author: Brian Coffey
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 5  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding tmplate or example
 *	code, is my own original work.
 */
 #include <avr/io.h>
 #ifdef _SIMULATE_
 #include "simAVRHeader.h"
 #endif

 enum States {start, init, add_p, add_r, sub_p, sub_r, reset} state;

 void Tick() {

     unsigned char tmpA = ~PINA;
     unsigned char tmpC = PORTC;

     switch(state) {
         case start:
             state = init;
             break;

         case init:
             if (tmpA == 1) {
                 state = add_p;
             }
             else if (tmpA == 2) {
                 state = sub_p;
             }
             else if (tmpA == 3) {
                 state = reset;
             }
             break;

         case add_p:
             state = add_r;
             break;

         case add_r:
             if (tmpA == 0) {
                 state = init;
             }
             else if (tmpA == 1) {
                 state = add_r;
             }
             else if (tmpA == 3) {
                 state = reset;
             }
             break;

         case sub_p:
             state = sub_r;
             break;

         case sub_r:
             if (tmpA == 0) {
                 state = init;
             }
             else if (tmpA == 2) {
                 state = sub_r;
             }
             else if (tmpA == 3) {
                 state = reset;
             }
             break;

         case reset:
             if (!tmpA) {
                 state = init;
             }
             else {
                 state = reset;
             }
             break;

         default:
             state = init;
             break;
     }

     switch(state) {
         case add_p:
             if (tmpC < 9) {
                 tmpC = tmpC + 1;
             }
             break;

         case sub_p:
             if (tmpC > 0) {
                 tmpC = tmpC - 1;
             }
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
