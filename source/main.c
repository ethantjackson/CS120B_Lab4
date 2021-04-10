/*	Author: ejack023
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum SM_STATES {SM_Start, SM_PB0, SM_PB1} SM_STATE;
unsigned char prevInput = 0x00;
void Tick_Fct() {
    switch(SM_STATE) {
	case SM_Start:
	    SM_STATE = SM_PB0;
	    break;
	case SM_PB0:
	    if (PINA & 0x01 & !prevInput) {
		SM_STATE = SM_PB1;
	    }
	    else SM_STATE = SM_PB0;
	    break;
	case SM_PB1:
	    if (PINA & 0x01 & !prevInput) {
		SM_STATE = SM_PB0;
	    }
	    else SM_STATE = SM_PB1;
	    break;
	default:
	    SM_STATE = SM_PB0;
	    break;
    }
    switch(SM_STATE) {
	case SM_Start:
	    break;
	case SM_PB0:
	    PORTB = 0x01;
	    break;
	case SM_PB1:
	    PORTB = 0x02;
	    break;
	default:
	    break;
    }
    prevInput = PINA&0x01;
}

int main(void) {

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    SM_STATE = SM_Start;

    while (1) {
	Tick_Fct();
    }
    return 1;
}
