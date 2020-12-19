/* Author: Brian Coffey
 * Partner(s) Name:
 * Lab Section: 24
 * Assignment: Lab #8  Exercise #1
 * Exercise Description:
 * Use the tones C4, D4, and E4 from the table in the introduction section.
 * When a button is pressed and held, the tone mapped to it is generated on the speaker.
 * When more than one button is pressed simultaneously, the speaker remains silent.
 * When no buttons are pressed, the speaker remains silent.
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://www.youtube.com/watch?v=SBcOWJEAT9A
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

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

int main(void) {
		DDRA = 0x00; PORTA = 0xFF;
		DDRB = 0xFF; PORTB = 0x00;

    unsigned char tempA = 0x00;
    PWM_on();

  	while (1) {
        tempA = ~PINA & 0x07;

        if (tempA == 0x01) {
            set_PWM(261.63);
        }
        else if (tempA == 0x02) {
            set_PWM(293.66);
        }
        else if (tempA == 0x04) {
            set_PWM(329.63);
        }
        else {
            set_PWM(0);
        }
  	}

    PWM_off();
    return 1;
}
