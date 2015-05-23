/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #7 Part #2
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 


#include <avr/io.h>
//#include <bit.h>
#include "timer.h"
#include <stdio.h>
#include <string.h>
#include "io.h"

unsigned char SetBit(unsigned char pin, unsigned char number, unsigned char bin_value)
{
	return (bin_value ? pin | (0x01 << number) : pin & ~(0x01 << number));
}

unsigned char GetBit(unsigned char port, unsigned char number)
{
	return ( port & (0x01 << number) );
}

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0) return b;
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



unsigned char SM2_output = 0x00;
unsigned char SM3_output = 0x00;
unsigned char pause = 0;
volatile unsigned char TimerFlag = 0;


enum key_State {wait, one, two, three, four} keystate;
enum LCD_State {next, shift} lcdstate;
int start_index = 0;
int end_index = 1;
int cursor = 16;
unsigned char message[] = "CS120B is Legend... wait for it DARY!";
unsigned char sub_message[100];


tick_LCD (){

	switch(lcdstate){
		case -1 :
			start_index = 1;
			end_index = 1;
			cursor = 16;
			lcdstate = next;
			break;
		case next:
			if(cursor < 1)
			{
				lcdstate = shift;
			}
			break;
		case shift:
			if(start_index > 36)
			{
				start_index = 1;
				end_index = 1;
				cursor = 16;
				lcdstate = next;
			}
			break;
		default:
			break;
	}
	switch(lcdstate){
		case next:
			strncpy(sub_message, message, end_index);
			LCD_DisplayString(cursor, sub_message);
			++end_index;
			--cursor;
			break;
		case shift:
			strncpy(sub_message, message + start_index, end_index);
			LCD_DisplayString(cursor, sub_message);
			++start_index;
			//++end_index;
			break;
		default:
			break;
	}

}


int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	LCD_init();

	unsigned long int SMTick1_calc = 150;

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
	task1.TickFct = &tick_LCD;
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
