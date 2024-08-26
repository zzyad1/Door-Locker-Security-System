/*
 * DC-Motor.c
 *
 *  Created on: Feb 25, 2024
 *      Author: zyadm
 */
#include "DC-Motor.h"
#include "gpio.h"
#include "PWM.h"


void DcMotor_Init(void) {
    // Initialize motor pins
    GPIO_setupPinDirection(PORTD_ID,MOTOR_PIN1,PIN_OUTPUT);
    GPIO_setupPinDirection(PORTD_ID,MOTOR_PIN2,PIN_OUTPUT);
    // Stop the motor
    GPIO_writePin(PORTD_ID,MOTOR_PIN1,LOGIC_LOW);
    GPIO_writePin(PORTD_ID,MOTOR_PIN2,LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{
    // Set motor direction based on state
    switch (state) {
        case CW:
            GPIO_writePin(PORTD_ID,MOTOR_PIN1, LOGIC_HIGH);
            GPIO_writePin(PORTD_ID,MOTOR_PIN2, LOGIC_LOW);
            PWM_Timer0_Start(speed);
            break;
        case ACW:
            GPIO_writePin(PORTD_ID,MOTOR_PIN1, LOGIC_LOW);
            GPIO_writePin(PORTD_ID,MOTOR_PIN2, LOGIC_HIGH);
            PWM_Timer0_Start(speed);

            break;
        case STOP:
            GPIO_writePin(PORTD_ID,MOTOR_PIN1, LOGIC_LOW);
            GPIO_writePin(PORTD_ID,MOTOR_PIN2, LOGIC_LOW);
            PWM_Timer0_Start(speed);
            break;
    }

}


