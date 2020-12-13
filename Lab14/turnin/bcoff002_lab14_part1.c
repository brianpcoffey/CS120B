/*	Author: briancoffey
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
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



 void sendTick(){
     if(USART_IsSendReady(0)){
         PORTA = temp;
         USART_Send(temp, 0);
         while(!USART_HasTransmitted(0)){
             continue;
            }

         if(temp == 0x01){
             temp = 0x00;
         }
         else{
             temp = 0x01;
         }

     }
 }


 int main(void) {
     /* Insert DDR and PORT initializations */
     DDRA = 0xFF; PORTA = 0x00;
     initUSART(0);
     TimerSet(500);
     TimerOn();

     /* Insert your solution below */
     while (1) {
     while(!TimerFlag);
     TimerFlag = 0;
     sendTick();
     }
     return 1;
 }

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "usart_ATmega1284.h"
#include "timer.h"
unsigned char temp = 0x00;



void receiveTick(){

    if(USART_HasReceived(0)){
        temp = USART_Receive(0);
        PORTA = temp;
        USART_Flush(0);

    }

}





int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFF; PORTA = 0x00;

    initUSART(0);
    TimerSet(5);
    TimerOn();
    PORTA = temp;
    USART_Flush(0);
    /* Insert your solution below */
    while (1) {
    while(!TimerFlag);
    TimerFlag = 0;
    receiveTick();
    }
    return 1;
}
