/*	Author: briancoffey
 *  Partner(s) Name: Brian Coffey
 *	Lab Section: 24
 *	Assignment: Lab 14  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "usart_ATmega1284.h"
#include "timer.h"

unsigned char temp = 0x00;
unsigned char tempB = 0x00;
enum states {init, leader, follower} state;
unsigned char count = 0x00;

void Tick() {
    switch(state){
        case init:
            if (tempB == 1) {
                state = leader;
            }
            else if (tempB == 0) {
                state = follower;
            }
            else {
                state = init;
            }
            break;
        case leader:
            if (tempB == 1) {
                state = leader;
            }
            else if (tempB == 0) {
                state = follower;
            }
            else {
                state = init;
            }
            break;
        case follower:
            if (tempB == 1) {
                state = leader;
            }
            else if (tempB == 0) {
                state = follower;
            }
            else {
                state = init;
            }
            break;
        default:
            break;
    }

    switch(state){
        case init:
            count = 0;
            break;
        case leader:
            if (count >= 100) {
                if (USART_IsSendReady(0)) {
                    PORTA = temp;
                    USART_Send(temp, 0);
                    while (!USART_HasTransmitted(0)) {
                        continue;
                    }
                    temp ^= 1;
                }
                count = 0;
            }
            PORTC = 0x01;
            count++;
            break;
        case follower:
            if (USART_HasReceived(0)) {
                temp = USART_Receive(0);
                PORTA = temp;
                USART_Flush(0); 
            }
            PORTC = 0x00;
            break;
        default:
            break;
    }
}

int main(void) {
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    initUSART(0);
    TimerSet(5);
    TimerOn();
    PORTA = temp;
    USART_Flush(0);
    while (1) {
        tempB = ~PINB;
        Tick();
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
