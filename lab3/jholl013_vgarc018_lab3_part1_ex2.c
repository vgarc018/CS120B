/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #2 Part #1 Exercise #2
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 



#include <avr/io.h>

enum BTN_STATES {
    BTN_INIT,
    BTN_WAIT,
    BTN_INC,
    BTN_DEC,
    BTN_WAIT_PRESS,
    BTN_RESET
    } BTN_State;

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}

void tick_btn() {
    switch (BTN_State) {
        case BTN_INIT:
            BTN_State = BTN_WAIT;
            break;
        case BTN_WAIT:
            if (GetBit(PINA, 0) && GetBit(PINA, 1))
                BTN_State = BTN_WAIT;
            else if (!GetBit(PINA, 0) && GetBit(PINA, 1) && PORTC < 9)
                BTN_State = BTN_INC;
            else if (!GetBit(PINA, 1) && GetBit(PINA, 0) && PORTC > 0)
                BTN_State = BTN_DEC;
            else if (!GetBit(PINA, 0) && !GetBit(PINA, 1))
                BTN_State = BTN_RESET;
            break;
        case BTN_INC:
            BTN_State = BTN_WAIT_PRESS;
            break;
        case BTN_WAIT_PRESS:
            if (!GetBit(PINA, 0) && !GetBit(PINA, 1))
                BTN_State = BTN_RESET;
            else if (!GetBit(PINA, 0) || !GetBit(PINA, 1))
                BTN_State = BTN_WAIT_PRESS;
            else
                BTN_State = BTN_WAIT;
            break;
        case BTN_DEC:
            BTN_State = BTN_WAIT_PRESS;
            break;
        case BTN_RESET:
            if (!GetBit(PINA, 0) && !GetBit(PINA, 1))
                BTN_State = BTN_RESET;
            else if (!GetBit(PINA, 0) || !GetBit(PINA, 1))
                BTN_State = BTN_WAIT_PRESS;
            else
                BTN_State = BTN_WAIT;
            break;
        default: BTN_State = BTN_INIT;
    }
    
    switch (BTN_State) {
        case BTN_START:
            PORTB = SetBit(PORTB, 0, 1);
            PORTB = SetBit(PORTB, 1, 0);
            break;
        case BTN_WAIT1:
            PORTB = SetBit(PORTB, 0, 0);
            PORTB = SetBit(PORTB, 1, 1);
        default: break;
    }   
}
int main(void)
{
    DDRA = 0x00;
    PORTA = 0xFF;
    
    DDRB = 0xFF;
    PORTB = 0x00;
    
    BTN_State = BTN_START;
    
    while(1)
    {
        tick_btn();
    }
}

