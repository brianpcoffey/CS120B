
/*	Author: bcoff002
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 4  Exercise 5
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
 enum States {start1, Locked, Combo_Press, Combo_Release, NotNext_Press, Unlocked,
     NotCombo_Press, NotCombo_Release, Next_Press} state;

 void tickfunc() {
     unsigned char tempA = PINA;
     unsigned char A0 = PINA & 0x01;
     unsigned char A1 = PINA & 0x02;
     unsigned char A2 = PINA & 0x04;
     unsigned char A7 = PINA & 0x80;
     unsigned char tempB = 0x00;

     switch(state) {
         case start1:
             state = Locked;
             break;
         case Locked:
             state = A2 ? Combo_Press : Locked;
             break;
         case Combo_Press:
             if (A0) {
                 state = Combo_Release;
             }
             else if (A2 || tempA == 0) {
                 state = Combo_Press;
             }
             else if (A1) {
                 state = Locked;
             }
             break;
         case Combo_Release:
             if (A0 || tempA == 0) {
                 state = Combo_Release;
             }
             else if (A1) {
                 state = NotNext_Press;
             }
             else if (A2) {
                 state = Locked;
             }
             break;
         case NotNext_Press:
             if (A1 || tempA == 0) {
                 state = NotNext_Press;
             }
             else if (A0 || tempA) {
                 state = Unlocked;
             }
             else if (A2) {
                 state = Locked;
             }
             break;
         case Unlocked:
             if (A7) {
                 state = Locked;
             }
             else if (A2) {
                 state = NotCombo_Press;
             }
             else {
                 state = Unlocked;
             }
             break;
         case NotCombo_Press:
             if (A0) {
                 state = NotCombo_Release;
             }
             else if (A2 || tempA == 0) {
                 state = NotCombo_Press;
             }
             else if (A1) {
                 state = Unlocked;
             }
             break;
         case NotCombo_Release:
             if (A0 || tempA == 0) {
                 state = NotCombo_Release;
             }
             else if (A1) {
                 state = Next_Press;
             }
             else if (A2) {
                 state = Unlocked;
             }
             break;
         case Next_Press:
             if (A1 || tempA == 0) {
                 state = Next_Press;
             }
             else if (A0) {
                 state = Locked;
             }
             else if (A2) {
                 state = Unlocked;
             }
             break;
         default:
             state = Locked;
             break;
     }
     switch(state) {
         case Locked:
             tempB = 0;
             break;
         case Unlocked:
             if (tempB == 0) {
                 tempB = 1;
             }
             else {
                 tempB = 0;
             }
             break;
         default:
             tempB = 0;
             break;
     }
     PORTB = tempB;
 }

 int main(void) {
     DDRA = 0x00; PORTA = 0xFF;
     DDRB = 0xFF; PORTB = 0x00;
     while (1) {
         tickfunc();
     }
     return 1;
 }
