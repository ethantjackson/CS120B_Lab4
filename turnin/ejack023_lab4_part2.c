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

enum SM_STATES {SM_Start, SM_Wait, SM_Inc, SM_Dec, SM_Reset} SM_STATE;
unsigned char prevInput = 0x00;
void Tick_Fct() {
    switch(SM_STATE) {
	case SM_Start:
	    SM_STATE = SM_Wait;
	    break;
	case SM_Wait:
	    if (PINA & 0x01 && PINA & 0x02) SM_STATE = SM_Reset;
	    else if (PINA & 0x01 && prevInput != 0x01) SM_STATE = SM_Inc;
	    else if (PINA & 0x02 && prevInput != 0x02) SM_STATE = SM_Dec;
	    else SM_STATE = SM_Wait; 
	    break;
	case SM_Inc:
	    SM_STATE = SM_Wait;
	    break;
	case SM_Dec:
	    SM_STATE = SM_Wait;
	    break;
	case SM_Reset:
	    SM_STATE = SM_Wait;
	    break;
	default:
	    SM_STATE = SM_Wait;
	    break;
    }
    switch(SM_STATE) {
	case SM_Start:
	    PORTC = 0x00;
	    break;
	case SM_Wait:
	    break;
	case SM_Inc:
	    if (PINC < 9) PORTC += 1;
	    break;
	case SM_Dec:
	    if (PINC > 0) PORTC -= 1;
	    break;
	case SM_Reset:
	    PORTC = 0x00;
	    break;
	default:
	    break;
    }
    prevInput = PINA;	
}

int main(void) {

    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    SM_STATE = SM_Start;

    while (1) {
	Tick_Fct();
    }
    return 1;
}
