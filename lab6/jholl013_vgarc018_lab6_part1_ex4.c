/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 023
 * Assignment: Lab #6 Part #1 Excercise #4
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
	unsigned short  MAX = ADC;
	
    
    while(1) {
       inVal = ADC;
	   temp = (char)inVal;
       if( temp >= MAX )
	   {
		   PORTB = 0xFF;
	   }
       if(temp > (MAX-8) && temp < MAX)
	   {
		   PORTB = 0x7F;
	   }
	   if(temp > (MAX-16) && temp < (MAX-8))
	   {
		   PORTB = 0x3F;
	   }
	   if(temp > (MAX-24) && temp < (MAX-16))
	   {
		   PORTB = 0x1F;
	   }
	   if(temp > (MAX-32) && temp < (MAX-24))
	   {
		   PORTB = 0x0F;
	   }
	   if(temp > (MAX-40) && temp < (MAX-32))
	   {
		   PORTB = 0x07;
	   }
	   if(temp > (MAX-48) && temp < (MAX-40))
	   {
		   PORTB = 0x03;
	   }
	   if(temp > (MAX-56) && temp < (MAX-48))
	   {
		   PORTB = 0x01;
	   }
	   if(temp > (MAX-64) && temp < (MAX-56))
	   {
		   PORTB = 0x00;
	   }
     
     
	}
}
