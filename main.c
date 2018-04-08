/*
 * car2.c
 *
 * Created: 4/7/2018 9:47:46 PM
 * Author : ThucLe
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "DriverLCD16x2.h"
#include <stdio.h>

#ifndef sbi
#define sbi(port,bit) port|=(1 << bit)
#endif

#ifndef cbi
#define cbi(port,bit) port&=~(1 << bit)
#endif
int   outlcd(char c, FILE *stream) {
	put_char(c);
	return 0;
}
static FILE mystdout = FDEV_SETUP_STREAM(outlcd,NULL, _FDEV_SETUP_WRITE );

int main(void)
{
	init_LCD();
	stdout = &mystdout;
	clr_LCD();
	MvCur(6,2);
	printf("Thuc%d",4);
    while (1)
    {
		
    }
}

