#include <avr/io.h>
#include <avr/interrupt.h>


//TimerISR sets this to 1.
volatile unsigned char TimerFlag = 0;
unsigned char threeLed = 0;
unsigned char oneLed = 0;
unsigned char sound = 0;

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

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

enum LED_STATES {
	LED_INIT,
	LED_ZERO,
	LED_ONE,
	LED_TWO
} LED_State;

enum LEDBLINK_STATE {
	LEDBLINK_INIT,
	LEDBLINK_OFF,
	LEDBLINK_ON
} LEDBLINK_State;

enum LEDSHOW_STATES {
	LEDSHOW_INIT,
	LEDSHOW_ON
} LEDSHOW_State;

enum SSTATE{
	SSTATE_INIT,
	SSTATE_ON,
	SSTATE_OFF
} SSTATE_State;

void tick_led()
{
	switch(LED_State) {
		case LED_INIT:
		LED_State = LED_ZERO;
		threeLed = 0x00;
		break;
		case LED_ZERO:
		LED_State = LED_ONE;
		break;
		case LED_ONE:
		LED_State = LED_TWO;
		break;
		case LED_TWO:
		LED_State = LED_ZERO;
		break;
		default:
		LED_State = LED_INIT;
		break;
	}
	
	switch(LED_State) {
		case LED_INIT:
		threeLed = 0;
		case LED_ZERO:
		threeLed = 0x01;
		break;
		case LED_ONE:
		threeLed = 0x02;
		break;
		case LED_TWO:
		threeLed = 0x04;
		break;
	}
}

void tick_one(){
	
	switch(LEDBLINK_State){
		case LEDBLINK_INIT:
		LEDBLINK_State = LEDBLINK_ON;
		break;
		case LEDBLINK_OFF:
		LEDBLINK_State = LEDBLINK_ON;
		break;
		case LEDBLINK_ON:
		LEDBLINK_State = LEDBLINK_OFF;
		break;
		default:
		LEDSHOW_State = LEDBLINK_INIT;
		break;
	}
	
	switch(LEDBLINK_State) {
		case LEDBLINK_INIT:
		oneLed = 0x00;
		break;
		case LEDBLINK_ON:
		oneLed = 0x08;
		break;
		case LEDBLINK_OFF:
		oneLed = 0x00;
		break;
	}
}

void tick_show()
{
	switch(LEDSHOW_State) {
		case LEDSHOW_INIT:
		LEDSHOW_State = LEDSHOW_ON;
		PORTB = 0x00;
		break;
		case LEDSHOW_ON:
		LEDSHOW_State = LEDSHOW_ON;
		PORTB = (oneLed + threeLed + sound);
		break;
		default:
		LEDSHOW_State = LEDSHOW_INIT;
		PORTB = 0x00;
		break;
	}
}

void tick_sound()
{
	switch(SSTATE_State){
		case SSTATE_INIT:
			SSTATE_State = SSTATE_OFF;
			sound = 0x00;
		break;
		case SSTATE_OFF:
			sound = 0x00;
			if(!GetBit(PINA, 2)){
				SSTATE_State = SSTATE_ON;
				sound = 0x10;
			}
			else
			{
				SSTATE_State = SSTATE_OFF;
			}
		break;
		case SSTATE_ON:
			if(!GetBit(PINA, 2))
			{
				SSTATE_State = SSTATE_ON;
				sound = 0x10;
			}
			else
			{
				SSTATE_State = SSTATE_OFF;
				sound = 0x00;
			}
		default:
			SSTATE_State = SSTATE_INIT;
		break;
			
	}
}

int main(void)
{
	DDRB = 0xFF;
	PORTB = 0x00;
	
	DDRA = 0x00;
	PORTA = 0xFF;
	unsigned long three = 0;
	unsigned long one = 0;
	const unsigned long period = 2;
	//unsigned char s = 0;

	TimerSet(period);
	TimerOn();
	
	LEDBLINK_State = LEDBLINK_INIT;
	LED_State = LED_INIT;
	LEDSHOW_State = LEDSHOW_INIT;
	SSTATE_State = SSTATE_INIT;
	
	while(1)
	{
		if(three >= 300){
			tick_led();
			three = 0;
		}
		if(one >= 1000){
			tick_one();
			one = 0;
		}
		tick_sound();
		tick_show();
		while(!TimerFlag);
		TimerFlag = 0;
		three += period;
		one += period;
	}
}