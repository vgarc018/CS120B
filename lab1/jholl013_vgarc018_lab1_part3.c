/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #1 Exercise #3
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 


#include <avr/io.h>

int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	
	DDRC = 0xFF;
	PORTC = 0x00;
	
	unsigned char avail = 4;
    while(1)
    {
		if (PINA & 0x01) {
			avail--;
		}
		if (PINA & (0x01<<1)) {
			avail--;
		}
		if (PINA & (0x01<<2)) {
			avail--;
		}
		if (PINA & (0x01<<3)) {
			avail--;
		}
		PORTC = avail;
		if (avail == 0) {
			PORTC = PORTC | (0x01 << 7);
		}
		else {
			PORTC = PORTC & ~(0x01 << 7);
		}
		avail = 4;
    }
}
