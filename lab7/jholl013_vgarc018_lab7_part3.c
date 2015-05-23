/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #7 Part #3
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>
#include "bit.h"
#include "io.h"
#include "timer.h"
#include <stdio.h>


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

typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;


volatile unsigned char TimerFlag = 0;
unsigned char SM2_output = 0x00;
unsigned char SM3_output = 0x00;
unsigned char pause = 0;

unsigned char out = 0x00;
unsigned char i = 0;
unsigned char fin = 0x00;
enum key_State {one,one2, two,two2, three,three2, four,four2, time} keystate;
unsigned char message[] = "Congradulations!"; //16;
void tick_KeypadKey() {

	switch (keystate){
		case -1:
			keystate = one;
			i = 0;
			out = ' ';
			break;
		case one:
			keystate = one2;
			PORTC = 0xEF;
			break;
		case one2:
			keystate = two;
			break;
		case two:
			keystate = two2;
			PORTC = 0xDF;
			break;
		case two2:
			keystate = three;
			break;
		case three:
			keystate = three2;
			PORTC = 0xBF;
			break;
		case three2:
			keystate = four;
			break;
		case four:
			PORTC = 0x7F;
			keystate = four2;
			break;
		case four2:
			keystate = time;
			break;
		case time:
			keystate = one;
			break;
		default:
			keystate = one;
			break;
	}
		asm("nop");

		unsigned char b0 = GetBit(~PINA,0);
		unsigned char b1 = GetBit(~PINA,1);
		unsigned char b2 = GetBit(~PINA,2);
		unsigned char b3 = GetBit(~PINA,3);
		switch(keystate){
			case one2:
				//++i;
				if(b0){
					out = '1';
				}
				else if (b1){
					out = '4';
				}
				else if (b2){
					out = '7';
				}
				else if (b3){
					out = '*';
				}

				break;
			case two2:
				//++i;
				if(b0){
					out = '2';
				}
				else if (b1){
					out = '5';
				}
				else if (b2){
					out = '8';
				}
				else if (b3){
					out = '0';
				}

				break;
			case three2:
				//++i;
				if(b0){
					out = '3';
				}
				else if (b1){
					out = '6';
				}
				else if (b2){
					out = '9';
				}
				else if (b3){
					out = '#';
				}
				break;
			case four2:
				if(b0){
					out = 'A';
				}
				else if (b1){
					out = 'B';
				}
				else if (b2){
					out = 'C';
				}
				else if (b3){
					out = 'D';
				}
				break;
			case time:
				LCD_Cursor(1);
				LCD_WriteData(out);
				break;
			default:
				break;
		}
}


int main(void)
{
DDRB = 0xFF; PORTB = 0x00;
DDRA = 0xF0; PORTA = 0x0F; 
DDRC = 0xFF; PORTC = 0x00;
DDRD = 0xFF; PORTD = 0x00;
	LCD_init();
	unsigned long int SMTick1_calc = 10;
	unsigned long int tmpGCD = 1;
	unsigned long int GCD = tmpGCD;
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	// Task 1
	task1.state = -1;
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &tick_KeypadKey;
	TimerSet(GCD);
	TimerOn();

	unsigned short i;
	while(1) {
		for ( i = 0; i < numTasks; i++ ) {
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
}
