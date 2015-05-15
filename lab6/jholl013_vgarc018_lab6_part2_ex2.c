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

enum SM_SCALE {
    WAIT,
    SOUND_TOGGLE,
    SOUND_INC,
    SOUND_DEC
} scale_state;

double sound_array[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

void tick_scale(void) {
    static unsigned char index = 0;
    static unsigned char on = 0;
    switch (scale_state) { // Transitions
        case WAIT:
            if (!GetBit(PINA, 0) && GetBit(PINA, 1) && GetBit(PINA, 2)) {
                if (on) {
                    on = 0;
                    PWM_off();
                }
                else {
                    on = 1;
                    PWM_on();
                    set_PWM(sound_array[index]);
                }
                scale_state = SOUND_TOGGLE;
            }
            else if (GetBit(PINA, 0) && !GetBit(PINA, 1) && GetBit(PINA, 2) && on) {
                if (index > 0) {
                    index--;
                    set_PWM(sound_array[index]);
                }
                scale_state = SOUND_DEC;
            }
            else if (GetBit(PINA, 0) && GetBit(PINA, 1) && !GetBit(PINA, 2) && on) {
                if (index < 7) {
                    index++;
                    set_PWM(sound_array[index]);
                }
                scale_state = SOUND_INC;
            }
            else {
                scale_state = WAIT;
            }
            break;
        case SOUND_TOGGLE:
            if (!GetBit(PINA, 0) && GetBit(PINA, 1) && GetBit(PINA, 2)) {
                scale_state = SOUND_TOGGLE;
            }
            else {
                scale_state = WAIT;
            }
            break;
        case SOUND_INC:
            if (GetBit(PINA, 0) && GetBit(PINA, 1) && !GetBit(PINA, 2)) {
                scale_state = SOUND_INC;
            }
            else {
                scale_state = WAIT;
            }
            break;
        case SOUND_DEC:
            if (GetBit(PINA, 0) && !GetBit(PINA, 1) && GetBit(PINA, 2)) {
                scale_state = SOUND_DEC;
            }
            else {
                scale_state = WAIT;
            }
            break;
        default:
            scale_state = WAIT;
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

    scale_state = WAIT;

    while(1) {
        tick_scale();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}
