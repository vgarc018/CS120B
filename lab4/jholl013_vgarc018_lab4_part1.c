
/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #4 Part #1 Exercise #1
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"


//TimerISR sets this to 1.
volatile unsigned char TimerFlag = 0;
unsigned char count = 0;
// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
void TimerOn() {
    // AVR timer/counter controller register TCCR1
    // bit3 = 0: CTC mode (clear timer on compare)
    // bit2bit1bit0=011: pre-scaler /64
    // 00001011: 0x0B
    // SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
    // Thus, TCNT1 register will count at 125,000 ticks/s
    TCCR1B = 0x0B;

    // AVR output compare register OCR1A.
    // Timer interrupt will be generated when TCNT1==OCR1A
    // We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
    // So when TCNT1 register equals 125,
    // 1 ms has passed. Thus, we compare to 125.
    OCR1A = 125;
    
    // AVR timer interrupt mask register
    // bit1: OCIE1A -- enables compare match interrupt
    TIMSK1 = 0x02;

    //Initialize avr counter
    TCNT1=0;

    // TimerISR will be called every _avr_timer_cntcurr milliseconds
    _avr_timer_cntcurr = _avr_timer_M;

    //Enable global interrupts: 0x80: 1000000
    SREG |= 0x80;
}

void TimerOff() {
    // bit3bit1bit0=000: timer off
    TCCR1B = 0x00;
}

void TimerISR() {
    TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
    // CPU automatically calls when TCNT1 == OCR1
    // (every 1 ms per TimerOn settings)
    // Count down to 0 rather than up to TOP (results in a more efficient comparison)
    _avr_timer_cntcurr--;
    if (_avr_timer_cntcurr == 0) {
        // Call the ISR that the user uses
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

enum BTN_STATES {
	BTN_INIT,
	BTN_WAIT,
	BTN_INC,
	BTN_DEC,
	BTN_WAIT_PRESS,
	BTN_RESET
} BTN_State;

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

void tick_btn() {
	switch (BTN_State) {
		case BTN_INIT:
		LCD_WriteData(count + '0');
		BTN_State = BTN_WAIT;
		break;
		case BTN_WAIT:
			if (GetBit(PINA, 0) && GetBit(PINA, 1))
				BTN_State = BTN_WAIT;
			else if (!GetBit(PINA, 0) && GetBit(PINA, 1) && count < 9)
				BTN_State = BTN_INC;
			else if (!GetBit(PINA, 1) && GetBit(PINA, 0) && count > 0)
				BTN_State = BTN_DEC;
			else if (!GetBit(PINA, 0) && !GetBit(PINA, 1))
				BTN_State = BTN_RESET;
			break;
		case BTN_INC:
			if(!GetBit(PINA, 0) && GetBit(PINA,1) && count < 9)
				BTN_State = BTN_INC;
			else if(!GetBit(PINA, 0) && !GetBit(PINA,1))
				BTN_State = BTN_RESET;
			else
				BTN_State = BTN_WAIT;		
			break;
		case BTN_DEC:
		if(GetBit(PINA, 0) && !GetBit(PINA,1) && count > 0)
			BTN_State = BTN_DEC;
		else if(!GetBit(PINA, 0) && !GetBit(PINA,1))
			BTN_State = BTN_RESET;
		else
			BTN_State = BTN_WAIT;
		break;
		
		case BTN_RESET:
			BTN_State = BTN_WAIT;
			break;
		default: BTN_State = BTN_INIT;
	}
	
	switch (BTN_State) {
		case BTN_INIT:
			LCD_Cursor(1);
			LCD_WriteData(count + '0');
		break;
		case BTN_INC:
			count = count + 1;
			LCD_Cursor(1);
			LCD_WriteData(count + '0');
		break;
		case BTN_DEC:
			count = count - 1;
			LCD_Cursor(1);
			LCD_WriteData(count + '0');
		break;
		case BTN_RESET:
			count = 0;
			LCD_Cursor(1);
			LCD_WriteData(count + '0');
		default: break;
	}
}



int main(void)
{
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	// Initializes the LCD display
	 DDRA = 0x00;
	 PORTA = 0xFF;

	 TimerSet(1000);
	 TimerOn();

	LCD_init();
	// Starting at position 1 on the LCD screen, writes Hello World
	BTN_State = BTN_INIT;
	while(1) {
		 tick_btn();
		 while (!TimerFlag);
		 TimerFlag = 0;
	 }
}