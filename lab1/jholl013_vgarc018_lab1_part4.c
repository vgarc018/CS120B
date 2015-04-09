/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #1 Exercise #4
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 


#include <avr/io.h>

#define MAX_WEIGHT		140
#define MAX_BAL_WEIGHT	80

int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	
	DDRB = 0x00;
	PORTB = 0xFF;
	
	DDRC = 0xFF;
	PORTC = 0x00;
	
	DDRD = 0x00;
	PORTD = 0xFF;
	
	
	unsigned short total_weight;
	
    while(1)
    {
		total_weight = PINA + PINB + PIND;
		if (total_weight > MAX_WEIGHT) {
			PORTC |= 0x01;
		}
		else {
			PORTC &= ~(0x01);
		}
		if ( ((PINA - PIND) > MAX_BAL_WEIGHT) || ((PIND - PINA) > MAX_BAL_WEIGHT)) {
			PORTC |= 0x02;
		}
		else {
			PORTC &= ~(0x02);
		}

		// Approximate weight is stored in PC7-PC2
		// To use the value take the value stored in PC7-PC2 and multiply by 4
		PORTC |= total_weight & 0xFC;
    }
}