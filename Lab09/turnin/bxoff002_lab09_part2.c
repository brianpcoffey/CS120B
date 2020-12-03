/* Author: Brian Coffey
 * Partner(s) Name (if applicable):
 * Assignment: Lab #9  Exercise #2
 * Exercise Description:
 * Modify the above example so the threeLEDs light for 300 ms, while blinkingLED’s LED still blinks 1 second on and 1 second off.
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

enum threeLEDs_States {threeLEDs_zero, threeLEDs_one, threeLEDs_two} threeLEDs_state;

unsigned char threeLEDs = 0x00;
unsigned char threeLEDs_led = 0x00;

void ThreeLEDsSM() {
  switch(threeLEDs_state) {
    case threeLEDs_zero:
        threeLEDs_state = threeLEDs_one;
        break;
    case threeLEDs_one:
        threeLEDs_state = threeLEDs_two;
        break;
	case threeLEDs_two:
		    threeLEDs_state = threeLEDs_zero;

    default:
        threeLEDs_state = threeLEDs_zero;
        break;
  }

  switch(threeLEDs_state) {
    case threeLEDs_zero:
        threeLEDs_led = 0x01;
        break;
    case threeLEDs_one:
        threeLEDs_led = 0x02;
        break;
    case threeLEDs_two:
        threeLEDs_led = 0x04;
        break;

    default:
        threeLEDs_led |= 0x01;
        break;
  }
}

enum blinkingLEDs_States {blinkingLEDs_three, blinkingLEDs_zero} blinkingLEDs_state;

void BlinkingLEDSM() {
  switch(blinkingLEDs_state) {
    case blinkingLEDs_three:
        blinkingLEDs_state = blinkingLEDs_zero;
        break;
    case blinkingLEDs_zero:
        blinkingLEDs_state = blinkingLEDs_three;
        break;

    default:
        break;
  }

  switch(blinkingLEDs_state) {
    case blinkingLEDs_three:
        blinkingLEDs_led = 0x08;
        break;
    case blinkingLEDs_zero:
        blinkingLEDs_led = 0x01;
        break;

    default:
        blinkingLEDs_led = 0x08;
        break;
  }
}

enum combineLEDs_States {combineLEDs_combine} combineLEDs_state;

unsigned char blinkingLEDs_led = 0x00;

void CombineLEDsSM() {
  switch(combineLEDs_state) {
    case combineLEDs_combine:
        combineLEDs_state = combineLEDs_combine;
        break;

    default:
        combineLEDs_state = combineLEDs_combine;
        break;
  }

  switch(combineLEDs_state) {
    case combineLEDs_combine:
        threeLEDs = threeLEDs_led | blinkingLEDs_led;
        PORTB = threeLEDs;
        break;

    default:
        threeLEDs = threeLEDs_led | blinkingLEDs_led;
        PORTB = threeLEDs;
        break;
  }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
		unsigned long threeLEDs_elapsedTime = 0;
		unsigned long blinkingLEDs_elapsedTime = 0;
		unsigned long combineLEDs_elapsedTime = 0;
		const unsigned long timer_period = 100;

    threeLEDs_state = threeLEDs_zero;
    blinkingLEDs_state = blinkingLEDs_three;
    combineLEDs_state = combineLEDs_combine;

    TimerSet(timer_period);
    TimerOn();

    while (1) {
  		if (threeLEDs_elapsedTime >= 300) {
  			ThreeLEDsSM();
  			threeLEDs_elapsedTime = 0;
  		}
  		if (blinkingLEDs_elapsedTime >= 1000) {
  			BlinkingLEDSM();
  			blinkingLEDs_elapsedTime = 0;
  		}
  		CombineLEDsSM();

      while (!TimerFlag);
      TimerFlag = 0;

  		threeLEDs_elapsedTime += timer_period;
  		blinkingLEDs_elapsedTime += timer_period;
  		combineLEDs_elapsedTime += timer_period;
    }
    return 1;
}
