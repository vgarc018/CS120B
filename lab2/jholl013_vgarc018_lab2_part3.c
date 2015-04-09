/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #2 Exercise #3
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

    DDRC = 0xFF;
    PORTC = 0x00;

    unsigned char tempA = 0;
    unsigned char tempC = 0;
    unsigned char tempA2 = 0;
    while(1)
    {
        tempA = PINA & 0xF;
        tempA2 = PINA;
        
        tempC = ( (tempA >= 1) ? SetBit(tempC,5,1) : SetBit(tempC,5,0));
        tempC = ( (tempA >= 3) ? SetBit(tempC,4,1) : SetBit(tempC,4,0));
        tempC = ( (tempA >= 5) ? SetBit(tempC,3,1) : SetBit(tempC,3,0));
        tempC = ( (tempA >= 7) ? SetBit(tempC,2,1) : SetBit(tempC,2,0));
        tempC = ( (tempA >= 10) ? SetBit(tempC,1,1) : SetBit(tempC,1,0));
        tempC = ( (tempA >= 13) ? SetBit(tempC,0,1) : SetBit(tempC,0,0));
        tempC = ( (tempA <= 4) ? SetBit(tempC,6,1) : SetBit(tempC,6,0));
        
        unsigned char key = GetBit(tempA2, 4);
        unsigned char seated = GetBit(tempA2, 5);
        unsigned char belt = GetBit(tempA2, 6);
        tempC = (key && seated && !belt) ? SetBit(tempC, 7, 1) : SetBit(tempC, 7, 0);
        
        PORTC = tempC;
    }
}