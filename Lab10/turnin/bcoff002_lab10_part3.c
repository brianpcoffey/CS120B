/*	Author: Brian Coffey
*  Partner(s) Name:
*	Lab Section:
*	Assignment: Lab 10  Exercise 3
*	Exercise Description:
* You will now add a doorbell SM to this system. On PA7 add another button
* that will represent the doorbell button on the outside of the house.
* On PB6 you will attach the speaker from the previous lab and add that
* SM to your ever growing system. The functionality of this SM should be:
* Once the button is pressed (check every 200 ms)
* The speaker goes through a short (roughly 3 second) melody
* During the melody, pressing the button will do nothing
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

unsigned char tempB = 0x00;
enum lock_states {Lock, hash_press, hash_release, one_press, one_release, two_press, two_release, three_press, three_release, four_press, four_release, Unlock} lock_state;

void lockSMTick() {
    switch (lock_state) {
        case Lock:
            if (keypad_output == 0x0F) {
              lock_state = hash_press;
            }
            else {
              lock_state = Lock;
            }
            break;
        case hash_press:
            if (keypad_output == 0x1F) {
              lock_state = hash_release;
            }
            else if (keypad_output == 0x0F) {
              lock_state = hash_press;
            }
            else {
              lock_state = Lock;
            }
            break;
        case hash_release:
            if (keypad_output == 0x01) {
              lock_state = one_press;
            }
            else if (keypad_output == 0x1F) {
              lock_state = hash_release;
            }
            else {
              lock_state = Lock;
            }
            break;
        case one_press:
            if (keypad_output == 0x1F) {
              lock_state = one_release;
            }
            else if (keypad_output == 0x01) {
              lock_state = one_press;
            }
            else {
              lock_state = Lock;
            }
            break;
        case one_release:
            if (keypad_output == 0x02) {
              lock_state = two_press;
            }
            else if (keypad_output == 0x1F) {
              lock_state = one_release;
            }
            else {
              lock_state = Lock;
            }
            break;
        case two_press:
            if (keypad_output == 0x1F) {
              lock_state = two_release;
            }
            else if (keypad_output == 0x02) {
              lock_state = two_press;
            }
            else {
              lock_state = Lock;
            }
            break;
        case two_release:
            if (keypad_output == 0x03) {
              lock_state = three_press;
            }
            else if (keypad_output == 0x1F) {
              lock_state = two_release;
            }
            else {
              lock_state = Lock;
            }
            break;
        case three_press:
            if (keypad_output == 0x1F) {
              lock_state = three_release;
            }
            else if (keypad_output == 0x03) {
              lock_state = three_press;
            }
            else {
              lock_state = Lock;
            }
            break;
        case three_release:
            if (keypad_output == 0x04) {
              lock_state = four_press;
            }
            else if (keypad_output == 0x1F) {
              lock_state = three_release;
            }
            else {
              lock_state = Lock;
            }
            break;
        case four_press:
            if (keypad_output == 0x1F) {
              lock_state = four_release;
            }
            else if (keypad_output == 0x04) {
              lock_state = four_press;
            }
            else {
              lock_state = Lock;
            }
            break;
        case four_release:
            if (keypad_output == 0x05) {
              lock_state = Unlock;
            }
            else if (keypad_output == 0x1F) {
              lock_state = four_release;
            }
            else {
              lock_state = Lock;
            }
            break;
        case Unlock:
            if (tempB) {
              lock_state = Lock;
            }
            else {
              lock_state = Unlock;
            }
            break;
        default:
            break;
    }

    switch (lock_state) {
        case Lock:
            PORTB &= 0x80;
            break;
        case hash_press:
            break;
        case hash_release:
            break;
        case one_press:
            break;
        case one_release:
            break;
        case two_press:
            break;
        case two_release:
            break;
        case three_press:
            break;
        case three_release:
            break;
        case four_press:
            break;
        case four_release:
            break;
        case Unlock:
            PORTB |= 0x01;
            break;

        default:
            break;
    }
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

        default: state = keypad_keypad; break;
    }

    switch (state) {
        default: break;
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
unsigned char z = 0x00;
double tune [] = {261.63, 432.43, 345.32, 700.34, 219.4};

void doorbellSMTick() {
  switch(state) {
    case Start:
        state = Wait;
        break;
    case Wait:
          if (tempA == 0x80) {
              state = Play;
              z = 0x00;
          }
          else {
              state = Wait;
          }
        break;
    case Play:
          if (z < 4) {
              state = Play;
          }
          else {
              state = Pressed;
          }
          break;
    case Pressed:
        if (tempA == 0x80) {
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
        set_PWM(tune[z]);
        z++;
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
   DDRB = 0xFF; PORTB = 0x00;
   DDRC = 0xF0; PORTC = 0x0F;

   static task task1, task2, task3, task4, task5, task6;
   task *tasks[] = {&task1, &task2, &task3, &task4, &task5, &task6};
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
   task4.TickFct = &lockSMTick;
   //Task5
   task5.state = start;
   task5.period = 100;
   task5.elapsedTime = task5.period;
   task5.TickFct = &keypadSMTick;
   //Task6
   task6.state = Start;
   z = 0x00;
   task6.period = 200;
   task6.elapsedTime = task6.period;
   task6.TickFct = &doorbellSMTick;

   unsigned long GCD = tasks[0]->period;
   unsigned short i;
   for (i = 1; i < numTasks; i++) {
       GCD = findGCD(GCD, tasks[i]->period);
   }

   TimerSet(GCD);
   TimerOn();
   PWM_on();

   while (1) {
       for (i = 0; i < numTasks; i++) {
           tempA = ~PINA;
           tempB = ~PINB & 0x80;
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
