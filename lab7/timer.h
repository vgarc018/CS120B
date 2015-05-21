#ifndef AVR_TIMER_H
#define AVR_TIMER_H


//TimerISR sets this to 1.
extern volatile unsigned char TimerFlag;
void TimerOn(void);
void TimerOff(void);
void TimerISR(void);
void TimerSet(unsigned long);

#endif
