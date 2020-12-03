/* Author: Brian Coffey
 * Partner(s) Name (if applicable):
 * Assignment: Lab #9  Exercise #1
 * Exercise Description:
 * In one state machine (ThreeLEDsSM), output to a shared variable (threeLEDs)
 * the following behavior: set only bit 0 to 1, then only bit 1, then only bit 2 in sequence for 1 second each.
 * In a second state machine (BlinkingLEDSM), output to a shared variable (blinkingLED)
 * the following behavior: set bit 3 to 1 for 1 second, then 0 for 1 second.
 * In a third state machine (CombineLEDsSM), combine both shared variables and output to the PORTB. 
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

unsigned char threeLEDs = 0x00;
unsigned char threeLED_led = 0x00;

enum threeLED_States {threeLED_zero, threeLED_one, threeLED_two} threeLED_state;

void ThreeLEDsSM() {
  switch(threeLED_state) {
    case threeLED_zero:
        threeLED_state = threeLED_one;
        break;
    case threeLED_one:
        threeLED_state = threeLED_two;
        break;
	case threeLED_two:
		threeLED_state = threeLED_zero;

    default:
        threeLED_state = threeLED_zero;
        break;
  }

  switch(threeLED_state) {
    case threeLED_zero:
        threeLED_led = 0x01;
        break;
    case threeLED_one:
        threeLED_led = 0x02;
        break;
    case threeLED_two:
        threeLED_led = 0x04;
        break;

    default:
        threeLED_led |= 0x01;
        break;
  }
}

enum blinkingLED_States {blinkingLED_three, blinkingLED_zero} blinkingLED_state;

unsigned char blinkingLED_led = 0x00;

void BlinkingLEDsSM() {
  switch(blinkingLED_state) {
    case blinkingLED_three:
        blinkingLED_state = blinkingLED_zero;
        break;
    case blinkingLED_zero:
        blinkingLED_state = blinkingLED_three;
        break;

    default:
        break;
  }

  switch(blinkingLED_state) {
    case blinkingLED_three:
        blinkingLED_led = 0x08;
        break;
    case blinkingLED_zero:
        blinkingLED_led = 0x01;
        break;

    default:
        blinkingLED_led = 0x08;
        break;
  }
}

enum combineLED_States {combineLED_combine} combineLED_state;

void CombineLEDsSM() {
  switch(combineLED_state) {
    case combineLED_combine:
        combineLED_state = combineLED_combine;
        break;

    default:
        combineLED_state = combineLED_combine;
        break;
  }

  switch(combineLED_state) {
    case combineLED_combine:
        threeLEDs = threeLED_led | blinkingLED_led;
        PORTB = threeLEDs;
        break;

    default:
        threeLEDs = threeLED_led | blinkingLED_led;
        PORTB = threeLEDs;
        break;
  }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
		const unsigned long timer_period = 1000;

    TimerSet(timer_period);
    TimerOn();

    while (1) {
    	ThreeLEDsSM();
    	BlinkingLEDsSM();
    	CombineLEDsSM();
      while (!TimerFlag);
      TimerFlag = 0;
    }
    return 1;
}
