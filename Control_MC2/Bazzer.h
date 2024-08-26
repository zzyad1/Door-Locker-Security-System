/*
 * Bazzer.h
 *
 *  Created on: Mar 23, 2024
 *      Author: zyadm
 */

#ifndef BAZZER_H_
#define BAZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define BUZZER_PORT   PORTD_ID
#define BUZZER_PIN    PIN2_ID

typedef enum{
 OFF,ON
}Buzzer_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description
 * The Function responsible for setup the pin of buzzer through the GPIO driver.
 *  */
void Buzzer_Init(void);
/*
 * Description
 * The function responsible for ON /OFF buzzer based on the state input state value.
 * */

void Buzzer_ON_OFF(Buzzer_State buzzer_state);

#endif /* BAZZER_H_ */
