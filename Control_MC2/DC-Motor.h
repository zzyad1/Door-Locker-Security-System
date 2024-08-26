/*
 * DC-Motor.h
 *
 *  Created on: Feb 25, 2024
 *      Author: zyadm
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_
#include "std_types.h"


#define MOTOR_PIN1  6 // Define pin for motor direction
#define MOTOR_PIN2  7 // Define pin for motor direction.

typedef enum {
    CW,
    ACW,
    STOP,
} DcMotor_State;

void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* DCMOTOR_H_ */
