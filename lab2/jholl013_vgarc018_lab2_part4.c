/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #2 Exercise #4
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
    DDRA = 0x00;
    PORTA = 0xFF;

    DDRB = 0xFF;
    PORTB = 0x00;
    
    DDRC = 0xFF;
    PORTC = 0x00;

    unsigned char tempA = 0;
    unsigned char tempC = 0;
    unsigned char tempB = 0;
    while(1)
    {
       tempA = PINA;
       tempB = (tempA & 0xF0) >> 4;
       tempC = (tempA & 0xF) << 4;

       PORTB = tempB;
       PORTC = tempC;
       
    }
}
