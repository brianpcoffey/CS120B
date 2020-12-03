*/
	Author: Brian Coffey
*  Partner(s) Name:
*	Lab Section:
*	Assignment: Lab 10  Exercise 1
*	Exercise Description:
* Modify the keypad code to be in an SM task and add an LED on PB7 then will light up while any button is pressed,
* consider this immediate feedback to the user. Then, modify the keypad SM to utilize the simple task scheduler format.
* All code from here on out should use the task scheduler.
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "bit.h"
#include "keypad.h"
#include "scheduler.h"
#include "timer.h"

unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char keypad_output = 0x00;
unsigned char pause = 0;

enum pauseButtonSM_States {pauseButton_wait, pauseButton_press, pauseButton_release};

int pauseButtonSMTick(int state) {
   unsigned char press = ~PINA & 0x01;

   switch(state) {
       case pauseButton_wait:
           state = press == 0x01 ? pauseButton_press : pauseButton_wait; break;
       case pauseButton_press:
           state = pauseButton_release; break;
       case pauseButton_release:
           state = press == 0x00 ? pauseButton_wait : pauseButton_press; break;
       default: state = pauseButton_wait; break;
   }
   switch(state) {
       case pauseButton_wait: break;
       case pauseButton_press:
           pause = (pause == 0) ? 1 : 0; //toggle pause
           break;
       case pauseButton_release: break;
   }
   return state;
}

enum toggleLED0_State {toggleLED0_wait, toggleLED0_blink};

int toggleLED0SMTick(int state) {
   switch(state) {
       case toggleLED0_wait: state = !pause ? toggleLED0_blink: toggleLED0_wait; break;
       case toggleLED0_blink: state = pause ? toggleLED0_wait: toggleLED0_blink; break;
       default: state = toggleLED0_wait; break;
   }
   switch(state) {
       case toggleLED0_wait: break;
       case toggleLED0_blink:
           led0_output = (led0_output == 0x00) ? 0x01 : 0x00;
           break;
   }
   return state;
}

enum toggleLED1_State {toggleLED1_wait, toggleLED1_blink};

int toggleLED1SMTick(int state) {
   switch(state) {
       case toggleLED1_wait: state = !pause ? toggleLED1_blink: toggleLED1_wait; break;
       case toggleLED1_blink: state = pause ? toggleLED1_wait: toggleLED1_blink; break;
       default: state = toggleLED1_wait; break;
   }
   switch(state) {
       case toggleLED1_wait: break;
       case toggleLED1_blink:
           led1_output = (led1_output == 0x00) ? 0x01: 0x00;
           break;
   }
   return state;
}

enum display_States {display_display};

int displaySMTick(int state) {
   switch(state) {
       case display_display: state = display_display; break;
       default: state = display_display; break;
   }
   switch (state) {
       case display_display:
           if (keypad_output != 0x1F) {
               PORTB = 0x80;
           }
           else {
               PORTB = 0x00;
           }
           break;
   }
   return state;
}

enum keypad_States {keypad_keypad};

int keypadSMTick(int state) {
   unsigned char x;
   switch (state) {
       case keypad_keypad:
           x = GetKeypadKey();
           switch (x) {
               case '\0': keypad_output = 0x1F; break;
               case '1': keypad_output = 0x01; break;
               case '2': keypad_output = 0x02; break;
               case '3': keypad_output = 0x03; break;
               case '4': keypad_output = 0x04; break;
               case '5': keypad_output = 0x05; break;
               case '6': keypad_output = 0x06; break;
               case '7': keypad_output = 0x07; break;
               case '8': keypad_output = 0x08; break;
               case '9': keypad_output = 0x09; break;
               case 'A': keypad_output = 0x0A; break;
               case 'B': keypad_output = 0x0B; break;
               case 'C': keypad_output = 0x0C; break;
               case 'D': keypad_output = 0x0D; break;
               case '*': keypad_output = 0x0E; break;
               case '0': keypad_output = 0x00; break;
               case '#': keypad_output = 0x0F; break;
               default: keypad_output = 0x1B; break;
           }
           state = keypad_keypad;
           break;
       default: state = keypad_keypad;
       break;
   }
   switch (state) {
       default: break;
   }
   return state;
}

int main(void) {
   DDRA = 0x00; PORTA = 0xFF;
   DDRB = 0xFF; PORTB = 0x00;
   DDRC = 0xF0; PORTC = 0x0F;

   static task task1, task2, task3, task4, task5;
   task *tasks[] = {&task1, &task2, &task3, &task4, &task5};
   const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

   const char start = -1;

   //Task1
   task1.state = start;
   task1.period = 50;
   task1.elapsedTime = task1.period;
   task1.TickFct = &pauseButtonSMTick;
   //Task2
   task2.state = start;
   task2.period = 500;
   task2.elapsedTime = task2.period;
   task2.TickFct = &toggleLED0SMTick;
   //Task3
   task3.state = start;
   task3.period = 1000;
   task3.elapsedTime = task3.period;
   task3.TickFct = &toggleLED1SMTick;
   //Task4
   task4.state = start;
   task4.period = 10;
   task4.elapsedTime = task4.period;
   task4.TickFct = &displaySMTick;
   //Task5
   task5.state = start;
   task5.period = 100;
   task5.elapsedTime = task5.period;
   task5.TickFct = &keypadSMTick;

   unsigned long GCD = tasks[0]->period;
   unsigned short i;
   for (i = 1; i < numTasks; i++) {
       GCD = findGCD(GCD, tasks[i]->period);
   }

   TimerSet(GCD);
   TimerOn();

   while (1) {
       for (i = 0; i < numTasks; i++) {
           if (tasks[i]->elapsedTime == tasks[i]->period) {
               tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
               tasks[i]->elapsedTime = 0;
           }
           tasks[i]->elapsedTime += GCD;
       }
       while(!TimerFlag);
       TimerFlag = 0;
   }

   return 0;
}
