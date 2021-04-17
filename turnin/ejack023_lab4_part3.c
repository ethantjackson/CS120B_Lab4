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

enum SM_STATES {SM_Start, SM_Wait, SM_First, SM_Second, SM_Unlock, SM_Lock} SM_STATE;
unsigned char prevInput = 0x00;
void Tick_Fct() {
    switch(SM_STATE) {
	case SM_Start:
	    SM_STATE = SM_Wait;
	    break;
	case SM_Wait: 
	    if (PINA & 0x80 && !(PINA & 0x04 || PINA & 0x02 || PINA & 0x01)) SM_STATE = SM_Lock;
	    else if (PINA) SM_STATE = SM_First;
	    break;
	case SM_First:
	    if (PINA & 0x80) SM_STATE = SM_Lock;
	    else if (PINA & 0x04 && !(PINA & 0x02 || PINA & 0x01)) SM_STATE = SM_Second;
	    else SM_STATE = SM_Wait;
	    break;
	case SM_Second:
            if (PINA & 0x80) SM_STATE = SM_Lock;
	    else if ((prevInput != 0x04 && !(PINA&0x04)) && PINA & 0x02 && !(PINA & 0x04 || PINA & 0x01)) SM_STATE = SM_Unlock;
  	    else if ((prevInput != 0x04 && !(PINA&0x04)) && PINA) SM_STATE = SM_Wait;
	    break;
	case SM_Unlock:
            if (PINA & 0x80) SM_STATE = SM_Lock;
	    else if (prevInput != 0x02 && !(PINA&0x02)) SM_STATE = SM_Wait;
	    break;
	case SM_Lock:
	    if (prevInput != 0x80 && !(PINA&0x80)) SM_STATE = SM_Wait;
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
	case SM_First:
	    break;
	case SM_Second:
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
