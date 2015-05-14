/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 023
 * Assignment: Lab #6 Part #1 Excercise #3
 *
 * I acknoledge all content containe herein, excluding template or example
 * code, is my original work.
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

void ADC_init() {

    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    DDRB = 0xFF;
    PORTB = 0x00;

    DDRA = 0x00;
    PORTA = 0xFF;

    DDRD = 0xFF;
    PORTD = 0x00;

    ADC_init();

    unsigned short inVal;
	unsigned char temp;
	unsigned char MAX = 56;
	
    
    while(1) {
       inVal = ADC;
	   temp = inVal;
       if( temp >= MAX )
	   {
		   PORTB = 0x01;
	   }
       if(temp < MAX)
	   {
		   PORTB = 0x00;
	   }
    }
}
