/*	Author: Brian Coffey
 *	Lab Section: 24
 *	Assignment: Lab # 15 Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * <DEMO> https://www.youtube.com/watch?v=n7ePmmt-hEk
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <avr/interrupt.h>

#include "timer.h"

unsigned char cData = 0x00;

void SPI_MasterInit(void) {
	DDRB = 0xB0;
	SPCR |= 0x51;
	SREG |= 0x80;
}


void SPI_MasterTransmit(unsigned char cData) {
	SPDR = cData;
	PORTB = 0xA0;

	while(!(SPSR & (1<<SPIF))) {
		;
	}
	DDRB = 0xB0;
}

enum states {inc} state;
int Tick(int state){
	switch(state){
		case inc:
			break;
		default:
			state = inc;
			break;
	}
	switch(state){
		case inc:
			if(cData == 0xFF){
				cData = 0;
			}
			else{
				cData++;
			}
			SPI_MasterTransmit(cData);
			break;
		default:
			state = inc;
			break;
	}
	return state;
}


int main(void) {
	DDRA = 0xF0; PORTA = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xFF; PORTC = 0x00;

	SPI_MasterInit();
	TimerSet(1000);
	TimerOn();
	state = inc;
	while (1) {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//*********SLAVE*******SLAVE*******SLAVE*******SLAVE*******SLAVE**************//
////////////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <avr/interrupt.h>

#include "timer.h"

unsigned char receivedData = 0;

void SPI_SlaveInit(void) {
	DDRB = 0x40;
	SPCR |= 0xC0;
	SREG |= 0x80;
}

ISR(SPI_STC_vect) {
    receivedData = SPDR;
}

int main(void) {
	DDRA = 0xFF; PORTA = 0x00;
	SPI_SlaveInit();
	TimerSet(1);
	TimerOn();

	while (1) {
		PORTA = receivedData;
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}
