/*	Author: Brian Coffey
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 6  Exercise 2
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

 enum States {start, zero, one, two, cycle, press, release} state;

 void Tick() {
     unsigned char A0 = ~PINA & 0x01;
     unsigned char tempB = 0x00;
     switch (state) {
         case start:
             state = zero;
             break;

         case zero:
             if (A0) {
                 tempB = PORTB;
                 state = press;
             }
             else
                 state = one;
             break;

         case one:
             if (A0) {
                 tempB = PORTB;
                 state = press;
             }
             else
                 state = two;
             break;

         case two:
             if (A0) {
                 tempB = PORTB;
                 state = press;
             }
             else
                 state = cycle;
             break;

         case cycle:
             if (A0) {
                 tempB = PORTB;
                 state = press;
             }
             else
                 state = zero;
             break;

         case press:
             if (A0) {
                 tempB = PORTB;
                 state = press;
             }
             else if (!A0){
                 tempB = PORTB;
                 state = release;
             }
             break;

         case release:
             if (A0) {
                 state = zero;
             }
             else if (!A0) {
                 tempB = PORTB;
                 state = release;
             }
             break;

         default:
             state = start;
             break;
     }

     switch (state) {

         case zero:
             PORTB = 0x01;
             break;

         case one:
             PORTB = 0x02;
             break;

         case two:
             PORTB = 0x04;
             break;

         case cycle:
             PORTB = 0x02;
             break;

         case press:
             PORTB = tempB;
             break;

         case release:
             PORTB = tempB;
             break;

         default:
             PORTB = 0x00;
             break;
     }
 }

 int main(void) {
     DDRA = 0x00; PORTA = 0xFF;
     DDRB = 0xFF; PORTB = 0x00;
     TimerSet(300);
     TimerOn();
     state = start;
     while (1) {
         Tick();
         while (!TimerFlag);
         TimerFlag = 0;
     }
     return 1;
 }
