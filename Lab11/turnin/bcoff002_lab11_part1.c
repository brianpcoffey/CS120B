/* Author: Brian Coffey
* Partner(s) Name (if applicable):
* Assignment: Lab #11  Exercise #1
* Exercise Description:
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*
*  Demo Link: Youtube URL>
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

void TimerISR () {
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

char data = 0x00;
char tmpA = 0x00;

enum shift_states {shift_wait, shift_increase, shift_decrease} shift_state;

void ShiftSM() {
     switch(shift_state) {
         case shift_wait:
             if (tmpA == 0x01) {
                 shift_state = shift_increase;
             }
             else if (tmpA == 0x02) {
                 shift_state = shift_decrease;
             }
             else {
                 shift_state = shift_wait;
             }
             break;
         case shift_increase:
 			       if (tmpA == 0x01) {
               shift_state = shift_increase;
             }
             else {
               shift_state = shift_wait;
             }
			       break;
         case shift_decrease:
         		 if (tmpA == 0x02) {
               shift_state = shift_decrease;
             }
             else {
               shift_state = shift_wait;
             }
        		 break;
         default:
             shift_state = shift_wait;
             break;
	 }

     switch(shift_state) {
         case shift_wait:
             break;
         case shift_increase:
      			if (data < 0xFF) {
      				data++;
              transmit_data(data);
      			}
      			 break;
         case shift_decrease:
             if (data > 0x00) {
               data--;
               transmit_data(data);
             }
            break;
         default:;
             break;
     }
 }

 void transmit_data(unsigned char data) {
     int i;
     for (i = 0; i < 8 ; ++i) {
    	 // Sets SRCLR to 1 allowing data to be set
    	 // Also clears SRCLK in preparation of sending data
    	 PORTC = 0x08;
    	 // set SER = next bit of data to be sent.
    	 PORTC |= ((data >> i) & 0x01);
    	 // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
    	 PORTC |= 0x02;
     }
     // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
     PORTC |= 0x04;
     // clears all lines in preparation of a new transmission
     PORTC = 0x00;
 }

int main() {
  DDRA = 0x00; PORTA = 0xFF;
  DDRC = 0xFF; PORTC = 0x00;

  const unsigned char timer_period = 300;

  shift_state = shift_wait;

  TimerSet(timer_period);
  TimerOn();

  while(1) {
      tmpA = ~PINA & 0x03;
      ShiftSM();
      
      while (!TimerFlag);
      TimerFlag = 0;
  }
  return 0;
}
