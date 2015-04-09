/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #2 Exercise #1
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
    
    DDRB = 0x00;
    PORTB = 0xFF;

    DDRC = 0xFF;
    PORTC = 0x00;
        
    unsigned char count = 0;
    unsigned char tempA = 0;
    unsigned char tempB = 0;

    while(1)
    {
                
        for(unsigned char i = 0; i < 8; i++)
        {
            tempA = GetBit(PINA, i);
            tempB = GetBit(PINB, i);
            if(tempA)
                count++;
            if(tempB)
                count++;
        }
    
        PORTC = count;   
        count = 0;      
    }
}