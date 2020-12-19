/*	Author: Brian Coffey
 *  Partner(s) Name:
 *	Lab Section: 24
 *	Assignment: Lab #8  Exercise #3
 *	Exercise Description:
 *  When the button is pressed, the melody should play until completion
 *  Pressing the button again in the middle of the melody should do nothing
 *  If the button is pressed and held, when the melody finishes, it should not repeat until the button is released and pressed again
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://www.youtube.com/watch?v=aEEl0D0wZ94
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

void set_PWM(double frequency) {
    static double current_frequency;

    if (frequency != current_frequency) {
        if (!frequency) {
            TCCR3B &= 0x08;
        }
        else {
            TCCR3B |= 0x03;
        }
        if (frequency < 0.954){
            OCR3A = 0xFFFF;
        }
        else if (frequency > 31250) {
            OCR3A = 0x0000;
        }
        else {
            OCR3A = (short)(8000000 / (128 * frequency)) - 1;
        }

        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

enum States {Start, Wait, Play, Pressed} state;

unsigned char tempA = 0x00;
unsigned char i = 0x00;
double tune [] = {261.63, 329.63, 640.32, 392.00, 523.25, 392.00, 432.53, 234.12, 329.63, 261.63, 329.63, 392.00, 523.25, 634.34};

void Tick() {
  switch(state) {
    case Start:
        state = Wait;
        break;
    case Wait:
          if (tempA) {
              state = Play;
              i = 0x00;
          }
          else {
              state = Wait;
          }
        break;
    case Play:
          if (i < 13) {
              state = Play;
          }
          else {
              state = Pressed;
          }
          break;
    case Pressed:
        if (tempA) {
            state = Pressed;
        }
        else {
            state = Wait;
        }
        break;
    default:
        state = Start;
        break;
  }

  switch(state) {
    case Start:
        break;
    case Wait:
        break;
    case Play:
        set_PWM(tune[i]);
        i++;
        break;
    case Pressed:
        set_PWM(0);
        break;
    default:
        break;
  }
}


int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x40;
    TimerSet(250);
	  TimerOn();

    PWM_on();
    state = Start;
    i = 0x00;

    while (1) {
        tempA = ~PINA;
        Tick();
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
