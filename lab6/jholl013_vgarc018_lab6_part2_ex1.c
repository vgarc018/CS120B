/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #6 Part #2 Exercise #1
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

enum SM_SOUND {
    SILENT,
    PLAY_C,
    PLAY_D,
    PLAY_E,
} sound_state;

void tick_sound(void) {
    switch (sound_state) { // Transitions
        case SILENT:
            if (!GetBit(PINA, 0) && GetBit(PINA, 1) && GetBit(PINA, 2)) {
                sound_state = PLAY_C;
            }
            else if (GetBit(PINA, 0) && !GetBit(PINA, 1) && GetBit(PINA, 2)) {
                sound_state = PLAY_D;
            }
            else if (GetBit(PINA, 0) && GetBit(PINA, 1) && !GetBit(PINA, 2)) {
                sound_state = PLAY_E;
            }
            else {
                sound_state = SILENT;
            }
            break;
        case PLAY_C:
            if (!GetBit(PINA, 0) && GetBit(PINA, 1) && GetBit(PINA, 2)) {
                sound_state = PLAY_C;
            }
            else {
                sound_state = SILENT;
            }
            break;
        case PLAY_D:
            if (GetBit(PINA, 0) && !GetBit(PINA, 1) && GetBit(PINA, 2)) {
                sound_state = PLAY_D;
            }
            else {
                sound_state = SILENT;
            }
            break;
        case PLAY_E:
            if (GetBit(PINA, 0) && GetBit(PINA, 1) && !GetBit(PINA, 2)) {
                sound_state = PLAY_E;
            }
            else {
                sound_state = SILENT;
            }
            break;
        default:
            sound_state = SILENT;
            break;
    }

    switch (sound_state) { // actions
        case SILENT:
            PORTB = SetBit(PORTB, 0, 0);
            set_PWM(0);
            break;
        case PLAY_C:
            PORTB = SetBit(PORTB, 0, 1);
            set_PWM(261.63);
            break;
        case PLAY_D:
            set_PWM(293.66);
            break;
        case PLAY_E:
            set_PWM(329.63);
            break;
        default:
            break;
    }
}
        




int main(void) {
    // Inputs
    DDRA = 0x00; PORTA = 0xFF;

    // Outputs
    DDRB = 0xFF; PORTB = 0x00;

    // Timer setup
    TimerSet(100);
    TimerOn();

    // PWM Setup
    PWM_on();
    set_PWM(0);

    sound_state = SILENT;

    while(1) {
        tick_sound();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}
