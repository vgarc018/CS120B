/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #1 Exercise #1
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 


#include <avr/io.h>

int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	
	DDRB = 0xFF;
	PORTB = 0x00;
	
	
    while(1)
    {
		if ( (PINA & 0x01) && !(PINA & (0x01<<1)) ) {
			PORTB |= 0x01;
		}
		else {
			PORTB &= ~(0x01);
		}
    }
	return 0;
}