/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #3 Part #2 Exercise #2
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

//TimerISR sets this to 1.
volatile unsigned char TimerFlag = 0;

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

enum LED_States { LED_INIT, 
                  LED1_ON1,
                  LED1_ON2,
                  LED2_ON1,
                  LED2_ON2,
                  LED3_ON1,
                  LED3_ON2,
                  LED_PRESSED,
                  LED_WAIT
                } LED_State;

void tick_led(void) {

    switch (LED_State) { // Transitions
        case LED_INIT:
            PORTB = 0x00;
            if (GetBit(PINA, 0))
                LED_State = LED1_ON1;
            break;
        case LED1_ON1:
            if (!GetBit(PINA, 0)) {
                LED_State = LED_PRESSED;
            }
            else {
                LED_State = LED1_ON2;
            }
            break;
        case LED1_ON2:
            LED_State = LED2_ON1;
            break;
        case LED2_ON1:
            if (!GetBit(PINA, 0)) {
                LED_State = LED_PRESSED;
            }
            else {
                LED_State = LED2_ON2;
            }
            break;
        case LED2_ON2:
            LED_State = LED3_ON1;
            break;
        case LED3_ON1:
            if (!GetBit(PINA, 0)) {
                LED_State = LED_PRESSED;
            }
            else {
                LED_State = LED3_ON2;   
            }
            break;
        case LED3_ON2:
            LED_State = LED1_ON1;
            break;
        case LED_PRESSED:
            if (!GetBit(PINA, 0)) {
                LED_State = LED_PRESSED;
            }
            else {
                LED_State = LED_WAIT;
            }
            break;
        case LED_WAIT:
            if (GetBit(PINA, 0)) {
                LED_State = LED_WAIT;
            }
            else {
                LED_State = LED_INIT;
            }
            break;
        default:
            LED_State = LED_INIT;
            break;
    }
    
    switch (LED_State) { // actions
        case LED1_ON1:
            PORTB = 0x01;
            break;
        case LED2_ON1:
            PORTB = 0x02;
            break;
        case LED3_ON1:
            PORTB = 0x04;
            break;
        default: break;
    }
}


int main(void)
{
    // Set port B to output
    // Init port B to 0s
    DDRB = 0xFF;
    PORTB = 0x00;

    DDRA = 0x00;
    PORTA = 0xFF;

    TimerSet(150);
    TimerOn();

    LED_State = LED_INIT;
    while(1) {
        tick_led();
        while (!TimerFlag);
        TimerFlag = 0;
    }
}