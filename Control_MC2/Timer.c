/*
 * Timer.c
 *
 *  Created on: Mar 23, 2024
 *      Author: zyadm
 */


#include "Timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"

static void (*timer1_callback)(void)= NULL_PTR;

ISR(TIMER1_COMPA_vect) {
    if (timer1_callback != NULL_PTR) {
        timer1_callback();
    }
}

ISR(TIMER0_OVF_vect)
{
	if(timer1_callback != NULL_PTR){
		(*timer1_callback)();
	}
}
void Timer1_init(const Timer1_ConfigType *Config_Ptr) {

	/*
	 * set TCNT1 with the initial value
	 */
	TCNT1 = Config_Ptr->initial_value;

	TCCR1A = (1 << FOC1A) | (1 << FOC1B) | (Config_Ptr->mode & 0x0003);

	/*
	 * configure timer1 to operate in CTC mode or normal mode and
	 * specify the prescaler
	 */
	TCCR1B = ((Config_Ptr->mode & 0x000C) << 1) | (Config_Ptr->prescaler);

	/*
	 * set the compera register if working in CTC mode
	 */
	if (Config_Ptr->mode == CTC_OCR1A) {
		OCR1A = Config_Ptr->compare_value;
	}

	/*
	 * Enable the interrupt of the specified mode
	 */
	if (Config_Ptr->mode == CTC_OCR1A) {
		TIMSK = (1 << OCIE1A);
	} else {
		TIMSK = (1 << TOIE1);
	}
}

void Timer1_setCallBack(void (*a_ptr)(void)) {
    timer1_callback = a_ptr;
}

void Timer1_deInit(void) {
    // Disable Timer1 and interrupts
	TCCR1A=0;
	TCCR1B=0;
	TCNT1=0;
	OCR1A=0;
	OCR1B=0;
	CLEAR_BIT(TIMSK,OCIE1A);
	CLEAR_BIT(TIMSK,OCIE1B);
	CLEAR_BIT(TIMSK,TOIE1);
}


