/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #3 Part #1 Exercise #1
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 



#include <avr/io.h>

enum BTN_STATES {
    BTN_START,
    BTN_PRESS,
    BTN_WAIT1,
    BTN_PRESS2
    } BTN_State;

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}

void tick_btn() {
    switch (BTN_State) {
        case BTN_START:
            if (!GetBit(PINA, 0))
                BTN_State = BTN_PRESS;
            else
                BTN_State = BTN_START;
            break;
        case BTN_PRESS:
            if(!GetBit(PINA, 0))
                BTN_State = BTN_PRESS;
            else
                BTN_State = BTN_WAIT1;
            break;
        case BTN_WAIT1:
            if(!GetBit(PINA, 0))
                BTN_State = BTN_PRESS2;
            else
                BTN_State = BTN_WAIT1;
            break;
        case BTN_PRESS2:
            if(GetBit(PINA, 0))
                BTN_State = BTN_START;
            else
                BTN_State = BTN_PRESS2;
            break;         
        default: BTN_State = BTN_START;
    }
    
    switch (BTN_State) {
        case BTN_START:
            PORTB = SetBit(PORTB, 0, 1);
            PORTB = SetBit(PORTB, 1, 0);
            break;
        case BTN_PRESS:
            PORTB = SetBit(PORTB, 0, 0);
            PORTB = SetBit(PORTB, 1, 1);
            break;
        case BTN_WAIT1:
            PORTB = SetBit(PORTB, 0, 0);
            PORTB = SetBit(PORTB, 1, 1);
            break;
        case BTN_PRESS2:
            PORTB = SetBit(PORTB, 0, 1);
            PORTB = SetBit(PORTB, 1, 0);
            break;
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

