/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #2 Part #1 Exercise #5
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 


#include <avr/io.h>

enum LOCK_STATES {
	LOCK_INIT,
	LOCK_WAIT,
	LOCK_PRESS1,
	LOCK_PRESS2x,
	LOCK_PRESS3y,
	LOCK_PRESS4x,
	LOCK_INSIDE,
	LOCK_INSIDEWAIT
	} LOCK_State;

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

unsigned char lock = 0;
unsigned char unlock = 1;
unsigned char p1 = 0;
unsigned char px1 = 0;
unsigned char py = 0;

void tick_lock()
{
	switch(LOCK_State){
		case LOCK_INIT:
				PORTB = 0x00;
				LOCK_State = LOCK_WAIT;
			break;
		case LOCK_WAIT:
			//PORTC = 0x01;
			if(!GetBit(PINA, 2) && GetBit(PINA,0) && GetBit(PINA, 1) && GetBit(PINA, 7))
				LOCK_State = LOCK_PRESS1;
			else if(GetBit(PINA,0) && GetBit(PINA, 1) && GetBit(PINA, 2) && !GetBit(PINA,7))
				LOCK_State = LOCK_INSIDE;
			else if(!GetBit(PINA,0) && GetBit(PINA, 1) && GetBit(PINA, 2) && GetBit(PINA,7) && lock == 1 && p1 == 1)
				LOCK_State = LOCK_PRESS2x;
			else if(GetBit(PINA,0) && !GetBit(PINA, 1) && GetBit(PINA, 2) && GetBit(PINA,7) && lock == 1 && p1 == 1 && px1 == 1)
				LOCK_State = LOCK_PRESS3y;
			else if(!GetBit(PINA,0) && GetBit(PINA, 1) && GetBit(PINA, 2) && GetBit(PINA,7) && lock == 1 && p1 == 1 && px1 == 1 && py == 1)
				LOCK_State = LOCK_PRESS4x;
			else
				LOCK_State = LOCK_WAIT;
			break;
		case LOCK_PRESS1:
			PORTC = 0x02;
				if(lock == 1)
				{
					PORTC = 0x0E;
				}
				lock = 1;
				p1 = 1;
				LOCK_State = LOCK_WAIT;
			break;
			
		case LOCK_PRESS2x:
			PORTC = 0x02;
			if(lock == 1)
			{
				PORTC = 0x0E;
			}
			px1 = 1;
			LOCK_State = LOCK_WAIT;
		break;
		case LOCK_PRESS3y:
			PORTC = 0x02;
			if(lock == 1)
			{
				PORTC = 0x0E;
			}
			py = 1;
		LOCK_State = LOCK_WAIT;
		break;			
		case LOCK_PRESS4x:
			LOCK_State = LOCK_WAIT;
			if(unlock == 0){
				 PORTB = 0x00;
				 unlock = 1;
				 lock = 0;
				 p1 = 0;
				 px1 = 0;
				 py = 0;
				 PORTC = 0x09;
			}
			else {
				PORTB = 0x01;
				PORTC = 0x0F;
				unlock = 0;
				lock = 0;
				 p1 = 0;
				 px1 = 0;
				 py = 0;
			}
			break;
		case LOCK_INSIDE:
			PORTC = 0x04;
			LOCK_State = LOCK_INSIDEWAIT;
			lock = 0;
			unlock = 1;
			p1 = 0;
	    	px1 = 0;
			py = 0;
			PORTB = 0x00;
			break;
		case LOCK_INSIDEWAIT:
			PORTC = 0x05;
			if((GetBit(PINA,0) && GetBit(PINA, 1) && GetBit(PINA, 2) && !GetBit(PINA,7)))
				LOCK_State = LOCK_INSIDEWAIT;
			else
				LOCK_State = LOCK_WAIT;
			break;
		default: LOCK_State = LOCK_WAIT;
	}
	
	/*switch(LOCK_State) {
		case LOCK_WAIT:
		break;
		case LOCK_PRESS1:
		break;
		case LOCK_PRESS2:
			PORTB = 0x01;
			break;
		case LOCK_PRESS2WAIT:
			PORTB = 0x01;
		break;
		case LOCK_INSIDE:
			PORTB = 0x00;
		break;
		case LOCK_INSIDEWAIT:
			PORTB = 0x00;
		break;
		default: break;
	}*/	
}



int main(void)
{
	 DDRA = 0x00;
	 PORTA = 0xFF;
	 
	 DDRB = 0xFF;
	 PORTB = 0x00;
	 
	 DDRC = 0xFF;
	 PORTC = 0x00;
	 
	 LOCK_State = LOCK_INIT;
	 
	 while(1)
	 {
		 tick_lock();
	 }
}