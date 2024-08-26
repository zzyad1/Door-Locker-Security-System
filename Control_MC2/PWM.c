// /******************************************************************************
// *
// * Module: PWM
// *
// * File Name: PWM.c
// *
// * Description: Source file for the ATmega32 ADC driver
// *
// * Author: zyad mostafa
// *
// *******************************************************************************/

#include "PWM.h"

#include <avr/io.h>

void PWM_Timer0_Start(uint8 duty_cycle) {
	TCNT0 = 0;
    OCR0 = (duty_cycle *255) /100;
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
    DDRB |= (1 << PB3);
}

