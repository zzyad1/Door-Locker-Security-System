/*
 * Timer.h
 *
 *  Created on: Mar 23, 2024
 *      Author: zyadm
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include "std_types.h"

typedef enum {
	NORMAL,
//	PWM_8,
//	PWM_9,
//	PWM_10,
	CTC_OCR1A =4
//	FAST_PWM_8,
//	FAST_PWM_9,
//	FAST_PWM_10,
//	PWM_Phase_and_Frequency_Correct_ICR1,
//	PWM_Phase_and_Frequency_Correct_OCR1A,
//	PWM_Phase_Correct_ICR1,
//	PWM_Phase_Correct_OCR1A,
//	CTC_ICR1,
//	FAST_PWM_ICR1,
//	FAST_PWM_OCRA1
} Timer1_Mode;

typedef enum {
	TIMER1_PRESCALER_no_Clk,
    TIMER1_PRESCALER_1,
    TIMER1_PRESCALER_8,
    TIMER1_PRESCALER_64,
    TIMER1_PRESCALER_256,
    TIMER1_PRESCALER_1024,
	External_clock_falling,
	External_clock_rising
} Timer1_Prescaler;

typedef struct {
    uint16 initial_value;
    uint16 compare_value;
    Timer1_Prescaler prescaler;
    Timer1_Mode mode;
} Timer1_ConfigType;

void Timer1_init(const Timer1_ConfigType *Config_Ptr);
void Timer1_deInit(void);
void Timer1_setCallBack(void (*a_ptr)(void));


#endif /* TIMER_H_ */
