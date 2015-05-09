/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #5 Exercise #5
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include "timer.h"

volatile unsigned char TimerFlag = 0;

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}

/* State Machine gloabal variables */
unsigned char led_count;
unsigned char waitTime;

/* State Machine Enums */

enum DISP_LED {
    DISP_LED
} display_state;

enum BTN_HELD {
    BTN_WAIT,
    BTN_PRESS,
    BTN_WAITINC
} btn_state;

enum BTN_PRESS {
    PRESS_INIT,
    PRESS_WAIT_INC,
    PRESS_INC,
    PRESS_WAIT,
    PRESS_WAIT_DEC,
    PRESS_DEC
} press_state;


void tick_disp(void) {
    switch (display_state) {
        case DISP_LED:
            PORTB = led_count;
            display_state = DISP_LED;
            break;
    }
}

void tick_btn(void) {
    static unsigned char count;
    switch (btn_state) { // Transitions
        case BTN_WAIT:
            waitTime = 10;
            count = 0;
            if (!GetBit(PINA, 0) || !GetBit(PINA, 1)) {
                btn_state = BTN_PRESS;
            }
            else {
                btn_state = BTN_WAIT;
            }
            break;
        case BTN_PRESS:
            if (count < 30 && (!GetBit(PINA, 0) || !GetBit(PINA, 1))) {
                btn_state = BTN_PRESS;
            }
            else {
               btn_state = BTN_WAITINC;
            }
            break;
        case BTN_WAITINC:
            if (!GetBit(PINA, 0) || !GetBit(PINA, 1)) {
                btn_state = BTN_WAITINC;
            }
            else {
                btn_state = BTN_WAIT;
            }
            break;
        default:
            btn_state = BTN_WAIT;
    }

    switch (btn_state) { // Actions
        case BTN_WAIT:
            waitTime = 10;
            break;
        case BTN_PRESS:
            count++;
            break;
        case BTN_WAITINC:
            waitTime = 4;
            break;
    }
}

void tick_press(void) {
    static unsigned char count;
    switch (press_state) { // Transitions
        case PRESS_INIT:
            led_count = 0;
            count = 0;
            press_state = PRESS_WAIT;
            break;
        case PRESS_WAIT:
            if (!GetBit(PINA, 0) && GetBit(PINA, 1)) {
                if (led_count < 9) led_count++;
                press_state = PRESS_INC;
            }
            else if (GetBit(PINA, 0) && !GetBit(PINA, 1)) {
                if (led_count > 0) led_count--;
                press_state = PRESS_DEC;
            }
            else if (!GetBit(PINA, 0) && !GetBit(PINA, 1)) {
                press_state = PRESS_INIT;
            }
            else {
                press_state = PRESS_WAIT;
            }
            break;
        case PRESS_INC:
            if (count < waitTime && (!GetBit(PINA, 0) && GetBit(PINA, 1))) {
                count++;
                press_state = PRESS_INC;
            }
            else if (count >= waitTime && (!GetBit(PINA, 0) && GetBit(PINA, 1))) {
                count = 0;
                if (led_count < 9) led_count++;
                press_state = PRESS_INC;
            }
            else if (!GetBit(PINA, 0) && !GetBit(PINA, 1)) {
                press_state = PRESS_INIT;
            }
            else {
                count = 0;
                press_state = PRESS_WAIT;
            }
            break;
        case PRESS_DEC:
            if (count < waitTime && (GetBit(PINA, 0) && !GetBit(PINA, 1))) {
                count++;
                press_state = PRESS_DEC;
            }
            else if (count >= waitTime && (GetBit(PINA, 0) && !GetBit(PINA, 1))) {
                count = 0;
                if (led_count > 0) led_count--;
                press_state = PRESS_DEC;
            }
            else if (!GetBit(PINA, 0) && !GetBit(PINA, 1)) {
                press_state = PRESS_INIT;
            }
            else {
                count = 0;
                press_state = PRESS_WAIT;
            }
            break;
        default:
            press_state = PRESS_INIT;
    }
}

int main(void) {
    DDRA = 0x00;
    PORTA = 0xFF;

    DDRB = 0xFF;
    PORTB = 0x00;

    TimerSet(100);
    TimerOn();

    led_count = 0;
    waitTime = 0;
    display_state = DISP_LED;
    btn_state = BTN_WAIT;
    press_state = PRESS_INIT;
    while(1) {
        tick_btn();
        tick_press();
        tick_disp();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}



