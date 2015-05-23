/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #7 Part #1
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>
//#include <bit.h>

#include "timer.h"

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
b = c;
	}
	return 0;
}


// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

unsigned char SM2_output = 0x00;
unsigned char SM3_output = 0x00;
unsigned char pause = 0;
unsigned char out = 0x00;
unsigned char i = 0;
unsigned char fin = 0x00;
enum key_State {one,onew, two,twow, three,threew, four,fourw, tim} keystate;	
void tick_KeypadKey() {

	switch (keystate){
		case -1:
			keystate = one;
			i = 0;
			break;
		case one:
			out = 0x1F;
			keystate = onew;
			PORTC = 0xEF;
			break;
		case onew:
			keystate = two;
			break;
		case two:
			keystate = twow;
			PORTC = 0xDF;
			break;
		case twow:
			
			keystate = three;
			break;
		case three:
			keystate = threew;
			PORTC = 0xBF;
			break;
		case threew:
			
			keystate = four;
			break;
		case four:
			PORTC = 0x7F;
			keystate = fourw;
			break;
		case fourw:
			keystate = tim;
			break;
		case tim:
			keystate = one;
			break;
		default:
			keystate = one;
			break;
	}			
		asm("nop");
		
		unsigned char b0 = GetBit(~PINC,0);
		unsigned char b1 = GetBit(~PINC,1);
		unsigned char b2 = GetBit(~PINC,2);
		unsigned char b3 = GetBit(~PINC,3);
		switch(keystate){
			case onew:
				++i;
				if(b0){
					out = 0x01;
				}					
				else if (b1){
					out = 0x04;
				}					
				else if (b2){
					out = 0x07;
				}					
				else if (b3){
					out = 0x0E;
				}
			
				break;
			case twow:
				++i;
				if(b0){
					out = 0x02;
				}
				else if (b1){
					out = 0x05;
				}
				else if (b2){
					out = 0x08;
				}
				else if (b3){
					out = 0x00;
				}
				
				break;	
			case threew:
				++i;
				if(b0){
					out = 0x03;
				}
				else if (b1){
					out = 0x06;
				}
				else if (b2){
					out = 0x09;
				}
				else if (b3){
					out = 0x0F;
				}
				
				
				break;
			case fourw:
				++i;
				if(b0){
					out = 0x0A;
				}
				else if (b1){
					out = 0x0B;
				}
				else if (b2){
					out = 0x0C;
				}
				else if (b3){
					out = 0x0D;
				}
			
				break;
			case tim:
			
				PORTB = out;
		
				break;
			default:
				break;
		}
}		


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	unsigned long int SMTick1_calc = 15;
	unsigned long int tmpGCD = 1;
	unsigned long int GCD = tmpGCD;
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	// Task 1
	task1.state = -1;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &tick_KeypadKey;//Function pointer for the tick.
	
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();

	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
}