/* Author: Brian Coffey
 * Partner(s) Name (if applicable):  
 * Lab Section: 24
 * Assignment: Lab #14  Exercise #3
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: Youtube URL> 
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "usart_ATmega1284.h"
#include "timer.h"

unsigned char temp = 0x00;
unsigned char tmpB = 0x00;
unsigned short counter = 0x00;
enum States {follower, leader} state;

void Tick() {
    switch(state) {
        case follower:
            state = (counter >= 3000) ? leader : follower;
            break;
        case leader:
            state = (USART_HasReceived(0)) ? follower : leader;
            break;
        default:
            break;
    }

    switch(state) {
        case follower:
            if (USART_HasReceived(0)) {
                PORTA = USART_Receive(0);
                USART_Flush(0);
                counter = 0;
            }
            else {
                counter++;
            }
            PORTC = 0x00;
            break;
        case leader:
            if (counter >= 500) {
                if (USART_IsSendReady(1)) {
                    PORTA = temp;
                    USART_Send(temp, 1);
                    while(!USART_HasTransmitted(1)) {
                        continue;
                    }
                    temp = ~temp;
                }
                counter = 0;
            }
            PORTC = 0x01;
            counter++;
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
    initUSART(1);
    TimerSet(1);
    TimerOn();
    state = follower;
    PORTA = temp;
    USART_Flush(0);


    while (1) {
        Tick();
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
