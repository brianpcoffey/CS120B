/*	Author: Brian Coffey
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 6  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #ifdef _SIMULATE_
 #include "simAVRHeader.h"
 #endif

 volatile unsigned char TimerFlag = 0;

 unsigned long _avr_timer_M = 1;
 unsigned long _avr_timer_cntcurr = 0;

 void TimerOn() {
     TCCR1B = 0x0B;
     OCR1A = 125;
     TIMSK1 = 0x02;
     TCNT1 = 0;

     _avr_timer_cntcurr = _avr_timer_M;

     SREG |= 0x80;
 }

 void TimerOff() {
     TCCR1B = 0x00;
 }

 void TimerISR() {
     TimerFlag = 1;
 }

 ISR(TIMER1_COMPA_vect) {
     _avr_timer_cntcurr--;
     if (_avr_timer_cntcurr == 0) {
         TimerISR();
         _avr_timer_cntcurr = _avr_timer_M;
     }
 }

 void TimerSet(unsigned long M) {
     _avr_timer_M = M;
     _avr_timer_cntcurr = _avr_timer_M;
 }

 enum States {start, wait, add, sub, reset} state;
 unsigned char x = 0;

 void Tick() {
     unsigned char tempA = ~PINA;
     unsigned char tempB = PORTB;

     switch(state) {
         case start:
             state = wait;
             break;

         case wait:
             if (tempA == 1) {
                 state = add;
             }
             else if (tempA == 2) {
                 state = sub;
             }
             else if (tempA == 3) {
                 state = reset;
             }
             else if (tempA == 0) {
                 state = wait;
             }
             break;

         case add:
             if (tempA == 1) {
                state = add;
             }
             else if (tempA == 3) {
                 state = reset;
             }
             else {
                 state = wait;
             }
             break;

         case sub:
             if (tempA == 2) {
                 state = sub;
             }
             else if (tempA == 3) {
                 state = reset;
             }
             else {
                 state = wait;
             }
             break;

         case reset:
             state = wait;
             break;

     }

     switch(state) {
         case start:
             tempB = 0x07;
             break;

         case add:
             if (tempB <= 8 && x == 10) {
                 tempB = tempB + 1;
                 x = 0;
             }
             x += 1;
             break;

         case sub:
             if (tempB >= 1 && x == 10) {
                 tempB = tempB - 1;
                 x = 0;
             }
             x++;
             break;

         case reset:
             x = 0;
             tempB = 0x00;
             break;

         case wait:
             x = 10;
             break;

         default:
             break;
     }

     PORTB = tempB;

 }

 int main(void) {
     DDRA = 0x00; PORTA = 0xFF;
     DDRB = 0xFF; PORTB = 0x07;
     
     TimerSet(100);
     TimerOn();

     state = start;

     while (1) {
         Tick();
         while (!TimerFlag);
         TimerFlag = 0;
     }
     return 1;
 }
