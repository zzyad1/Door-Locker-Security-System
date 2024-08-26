/*
 * Bazzar.c
 *
 *  Created on: Mar 23, 2024
 *      Author: zyadm
 */

#include <util/delay.h> /* For the delay functions */
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "Bazzer.h"
#include "gpio.h"

/*
 * Description
 * The Function responsible for setup the pin of buzzer through the GPIO driver.
 *  */
void Buzzer_Init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT,BUZZER_PIN,PIN_OUTPUT);
	/*At the beginning buzzer is stopped */
    GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);

}
/*
 * Description
 * The function responsible for ON /OFF buzzer based on the state input state value.
 * */

void Buzzer_ON_OFF(Buzzer_State  buzzer_state)
{
	if(buzzer_state == ON)
	{
		GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_HIGH);
	}
	else
	{
		GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);
	}
}
