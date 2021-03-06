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

enum SM_STATES {SM_Start, SM_WaitFirst, SM_First, SM_WaitSecond, SM_Second, SM_Check, SM_Unlock, SM_Lock} SM_STATE;
unsigned char prevInput = 0x00;
unsigned char Inputs [2];
void Tick_Fct() {
    switch(SM_STATE) {
	case SM_Start:
	    SM_STATE = SM_WaitFirst;
	    break;
	case SM_WaitFirst: 
	    if (PINA && !prevInput) SM_STATE = SM_First;
	    else SM_STATE = SM_WaitFirst;
	    break;
	case SM_First:
	    SM_STATE = SM_WaitSecond;
	    break;
	case SM_WaitSecond:
	    if (PINA && !prevInput) SM_STATE = SM_Second;
	    else SM_STATE = SM_WaitSecond;
	    break;
	case SM_Second:
	    SM_STATE = SM_Check;
	    break;
	case SM_Check:
            if (Inputs[0]==0x04 && Inputs[1]==0x02) SM_STATE = SM_Unlock;
            else SM_STATE = SM_WaitFirst;	
	    break;
	case SM_Unlock:
	    if (PINA & 0x80) SM_STATE = SM_Lock;
            else SM_STATE = SM_Unlock;
	    break;
	case SM_Lock:
	    SM_STATE = SM_WaitFirst;
	    break;
	default:
	    SM_STATE = SM_WaitFirst;
	    break;
    }
    switch(SM_STATE) {
	case SM_Start:
	    break;
        case SM_WaitFirst:
	    break;
	case SM_First:
	    Inputs[0] = PINA;
	    break;
	case SM_WaitSecond:
	    break;
	case SM_Second: 
	    Inputs[1] = PINA;
	    break;
	case SM_Check: 
 	    break;
	case SM_Unlock:
	    PORTB = 0x01;
	    break;
	case SM_Lock:
	    PORTB = 0x00;
	    break;
	default:
	    break;
    }
    prevInput = PINA;	
}

int main(void) {

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    SM_STATE = SM_Start;

    while (1) {
	Tick_Fct();
    }
    return 1;
}
