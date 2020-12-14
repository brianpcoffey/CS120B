/* Author: Brian Coffey
* Partner(s) Name (if applicable):
* Lab Section: 24
* Assignment: Lab #14  Exercise #1
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*
*  Demo Link: Youtube URL> https://www.youtube.com/watch?v=yJ44acYI-kg
*/
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "usart_ATmega1284.h"
#include "timer.h"

unsigned char temp = 0x00;

void sendTick() {
   if(USART_IsSendReady(0)) {
       PORTA = temp;
       USART_Send(temp, 0);
       while(!USART_HasTransmitted(0)) {
           continue;
       }
       temp ^= 1;
   }
}


int main(void) {
   DDRA = 0xFF; PORTA = 0x00;
   initUSART(0);
   TimerSet(500);
   TimerOn();

   while (1) {
     while(!TimerFlag);
     TimerFlag = 0;
     sendTick();
   }
   return 1;
}

////////////////////////////////////////////////////////////////////////////////////
//********************************************************************************//
////////////////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "usart_ATmega1284.h"
#include "timer.h"

unsigned char temp = 0x00;

void receiveTick() {
  if(USART_HasReceived(0)) {
      temp = USART_Receive(0);
      PORTA = temp;
      USART_Flush(0);
  }
}

int main(void) {
  DDRA = 0xFF; PORTA = 0x00;
  initUSART(0);
  TimerSet(5);
  TimerOn();
  PORTA = temp;
  USART_Flush(0);
  while (1) {
    while(!TimerFlag);
    TimerFlag = 0;
    receiveTick();
  }
  return 1;
}
