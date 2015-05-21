/*
 * James Hollister <jholl013@ucr.edu>
 * Virgilio Garcia <vgarc018@ucr.edu>
 * Lab Section: 23
 * Assignment: Lab #7 Part #5
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 


#include <avr/io.h>
#include "timer.h"
#include "io.h"

volatile unsigned char TimerFlag = 0;

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

/*****Keypad Arrangement*********
PC7  1	2	3	A
PC6	 4	5	6	B
PC5  7	8	9	C
PC4  *	0	#	D
	PC3	PC2 PC1 PC0
*******************************/
unsigned char GetKeypadKey()
{
	PORTA = 0x7F; // check row0 PC7
	asm("nop");
	if (GetBit(PINA,0)==0) { return('A'); }
	if (GetBit(PINA,1)==0) { return('3'); }
	if (GetBit(PINA,2)==0) { return('2'); }
	if (GetBit(PINA,3)==0) { return('1'); }
	
	PORTA = 0xBF; // check row1 PC6
	asm("nop");
	if (GetBit(PINA,0)==0) { return('B'); }
	if (GetBit(PINA,1)==0) { return('6'); }
	if (GetBit(PINA,2)==0) { return('5'); }
	if (GetBit(PINA,3)==0) { return('4'); }
	
	PORTA = 0xDF; // check row2 PC5
	asm("nop");
	if (GetBit(PINA,0)==0) { return('C'); }
	if (GetBit(PINA,1)==0) { return('9'); }
	if (GetBit(PINA,2)==0) { return('8'); }
	if (GetBit(PINA,3)==0) { return('7'); }
	
	PORTA = 0xEF; //check row3 PC4
	asm("nop");
	if (GetBit(PINA,0)==0) { return('D'); }
	if (GetBit(PINA,1)==0) { return('#'); }
	if (GetBit(PINA,2)==0) { return('0'); }
	if (GetBit(PINA,3)==0) { return('*'); }
	return('\0'); // no button pressed
}
unsigned char i = 0;
unsigned char run = 1;
void print()
{
    /*unsigned char x = GetKeypadKey();*/
	static unsigned char pos = 3;
	static unsigned char i = 0;
	unsigned char str[] = "                #                   #            #          ";
	unsigned char str2 [33];
    int j;
	for (j = 0 ; j < 32 ; ++j)
	str2[j] = str[(i+j)%48];
	i = (i+1)%20;
	str2[32] = 0;
	if(str2[pos] == '#')
	{
		LCD_DisplayString(1,"   Hurray for   Embedded Systems");
		run = 0;
	}
	else
		LCD_DisplayString(1,str2);
    /*if(x == 'A')*/
    if (!GetBit(PINA, 1))
		pos = 3;
    /*else if(x == 'B')*/
    else if (!GetBit(PINA, 2))
		pos = 19;
	LCD_Cursor(pos);
}

int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F; // Keypad Enable Columns , check rows
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	TimerSet(300);
	TimerOn();
	LCD_init();
						  
	unsigned char x = '\0';
	
	while(1)
	{
		if(run)
			print();
        /*x = GetKeypadKey();*/
        /*if (x == 'C')*/
        if (!GetBit(PINA, 0))
			run = 1;
		
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
