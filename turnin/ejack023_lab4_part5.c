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

enum SM_STATES {SM_Start, SM_Wait, SM_GetBtn, SM_Check, SM_Unlock, SM_Lock} SM_STATE;
unsigned char prevInput = 0x00;
unsigned char Inputs [4];
unsigned char seqIndex = 0;
void Tick_Fct() {
    switch(SM_STATE) {
	case SM_Start:
	    SM_STATE = SM_Wait;
	    break;
	case SM_Wait: 
	    if (PINA && !prevInput) SM_STATE = SM_GetBtn;
	    else SM_STATE = SM_Wait;
	    break;
	case SM_GetBtn:
	    if (seqIndex > 3) SM_STATE = SM_Check;
	    else SM_STATE = SM_Wait;
	    break;
	case SM_Check:
            if (Inputs[0]==0x04 && Inputs[1]==0x01 && Inputs[2]==0x02 && Inputs[3]==0x01) {
	        if (PINB==0x00) SM_STATE = SM_Unlock;
	        else if (PINB==0x01) SM_STATE = SM_Lock;
	    }
            else SM_STATE = SM_Wait;	
	    break;
	case SM_Unlock:
            SM_STATE = SM_Wait;
	    break;
	case SM_Lock:
	    SM_STATE = SM_Wait;
	    break;
	default:
	    SM_STATE = SM_Wait;
	    break;
    }
    switch(SM_STATE) {
	case SM_Start:
	    break;
        case SM_Wait:
	    break;
	case SM_GetBtn:
	    Inputs[seqIndex++] = PINA;
	    break;
	case SM_Check: 
	    seqIndex = 0;
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
