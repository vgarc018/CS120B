/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #6 Part #2 Exercise #2
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "PWM.h"

volatile unsigned char TimerFlag = 0;

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}

/*State machine enums*/

enum SM_MELODY {
    WAIT,
    PLAY_NOTE,
    WAIT_NOTE,
    WAIT_PRESS
} melody_state;

#define C4 261.63
#define D4 293.66
#define E4 329.63
#define F4 349.23
#define G4 392.00
#define A4 440.00
#define B4 493.88
#define C5 523.25

#define MAX_NOTES 40
double sound_array[MAX_NOTES] = {A4, A4, A4, G4, A4, A4, A4, B4,
                                 E4, E4, E4, D4, E4, E4, E4, G4,
                                 E4, E4, E4, G4, A4, A4, A4, G4,
                                 A4, A4, A4, A4, C4, C4, C4, C4,
                                 E4, E4, E4, D4, E4, E4, E4, D4};

unsigned char time_array[MAX_NOTES] = {2, 2, 2, 4, 2, 2, 2, 4,
                                       2, 2, 2, 4, 2, 2, 2, 4,
                                       2, 2, 2, 4, 2, 2, 2, 4,
                                       2, 2, 2, 4, 2, 2, 2, 4,
                                       2, 2, 2, 4, 2, 2, 2, 4};

unsigned char wait_array[MAX_NOTES] = {1, 1, 1, 4, 1, 1, 1, 4,
                                       1, 1, 1, 4, 1, 1, 1, 4,
                                       1, 1, 1, 4, 1, 1, 1, 4,
                                       1, 1, 1, 4, 1, 1, 1, 4,
                                       1, 1, 1, 4, 1, 1, 1, 4};

void tick_melody(void) {
    static unsigned char index = 0;
    static unsigned char count = 0;
    switch (melody_state) { // Transitions
        case WAIT:
            index = 0;
            count = 0;
            if (!GetBit(PINA, 0)) {
                melody_state = PLAY_NOTE;
            }
            else {
                melody_state = WAIT;
            }
            break;
        case PLAY_NOTE:
            if (count < time_array[index]) {
                set_PWM(sound_array[index]);
                count++;
                melody_state = PLAY_NOTE;
            }
            else {
                count = 0;
                melody_state = WAIT_NOTE;
            }
            break;
        case WAIT_NOTE:
            if (count < wait_array[index]) {
                set_PWM(0);
                count++;
                melody_state = WAIT_NOTE;
            }
            else {
                count = 0;
                if (index < MAX_NOTES - 1) {
                    index++;
                    melody_state = PLAY_NOTE;
                }
                else {
                    if (!GetBit(PINA, 0))
                        melody_state = WAIT_PRESS;
                    else
                        melody_state = WAIT;
                }
            }
            break;
        case WAIT_PRESS:
            if (!GetBit(PINA, 0)) {
                melody_state = WAIT_PRESS;
            }
            else {
                melody_state = WAIT;
            }
            break;
        default:
            melody_state = WAIT;
            break;
    }
}
        




int main(void) {
    // Inputs
    DDRA = 0x00; PORTA = 0xFF;

    // Outputs
    DDRB = 0xFF; PORTB = 0x00;

    // Timer setup
    TimerSet(50);
    TimerOn();

    // PWM Setup
    PWM_on();
    set_PWM(0);

    melody_state = WAIT;

    while(1) {
        tick_melody();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}
