/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #2 Exercise #5
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 


#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}

int main(void)
{
    DDRD = 0x00;
    PORTD = 0xFF;

    DDRB = 0xFE;
    PORTB = 0x01;

    unsigned short tempWeight = 0;
    unsigned char tempB = 0;
 
    while(1)
    {
        tempWeight = (PIND << 1) | (PINB & 0x01);
        tempB = (tempWeight >= 70) ? (SetBit(tempB,1,1)) : (SetBit(tempB, 1, 0));
        tempB = ((tempWeight < 70) && (tempWeight > 5)) ? (SetBit(tempB, 2, 1)) : (SetBit(tempB, 2, 0));
        
        PORTB = tempB;
    }
}