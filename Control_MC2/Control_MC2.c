/*
 * Control_MC2.c
 *
 *  Created on: Mar 23, 2024
 *      Author: zyadm
 */


#include <avr/io.h>
#include <util/delay.h>
#include "Timer.h"
#include "DC-Motor.h"
#include "uart.h"
#include "twi.h"
#include "Bazzer.h"
#include "external_eeprom.h"
#include "util/delay.h"
#include "std_types.h"

#define PASSWORD_SIZE 5
#define ADDRESSS 0x0F
#define UART_Ready 70
#define SET_PASSWORD 10
#define Enter_PASSWORD 20
#define Chanage_PASSWORD 30
#define UNmatching_PASSWORD 40
#define matching_PASSWORD 50
#define Turn_on_buzzer 80
#define Turn_off_buzzer 85
#define open_door 15
#define close_door 25
#define open_the_door 60
#define close_the_door 100
#define Hold_the_door 90

static uint8 recive[PASSWORD_SIZE];
volatile uint8 g_tick = 0;
volatile uint8 g_count = 0;
volatile uint8 time_requier = 0;
volatile uint8 flag =0;
volatile uint8 buzzer_sec =0;

Timer1_ConfigType config_ptr ={0,15624 ,TIMER1_PRESCALER_64 ,CTC_OCR1A};
TWI_ConfigType  twi_config ={0x10 ,400000};
UART_ConfigType uart_config = {BIT_DATA_8, Even_parity, STOP_1, 9600};


//void Display_password(void);
uint8 Check_PassMatching(void);
void Recive_password(void);
void Hand_Chacking(void);
void save_password(void);
void Buzzer_on(void);
void Buzzer_off(void);
void open_Door(void);
void Motor_ON(void);

int main(void)
{
	UART_init(&uart_config);
	_delay_ms(20);
	TWI_init(&twi_config);
	UART_sendByte(UART_Ready);
	Buzzer_Init();
	DcMotor_Init();
	SREG |= (1 << 7);

    while(1)
    {
		switch (UART_recieveByte()) {
		case SET_PASSWORD:
			save_password();
			break;
		case Enter_PASSWORD:
			Recive_password();
			Hand_Chacking();
			break;
		case open_door:
			open_Door();
			break;
		case Turn_on_buzzer:
			 Buzzer_on();
			break;
		}

    }
}
/*
* Description : Function to check the correctness of entered password
* 	1. get the original password from eeprom
* 	2. compare between the entered password and the original password
*  3. return 0 if false , 1 if true
*/
uint8 Check_PassMatching(void)
{
	uint8 eepromr[PASSWORD_SIZE];
	for (uint8 i = 0; i < PASSWORD_SIZE; i++) {
		EEPROM_readByte( ADDRESSS + i , &eepromr[i]);
		_delay_ms(10);
		if(eepromr[i] != recive[i])
		{
			return (0);
		}
	}
	return (1);
}
/*
* Description : Function to get passwords from the other ecu
* 	 store the incoming bytes in global array
*/
void Recive_password(void)
{
	UART_sendByte(UART_Ready);
    for (uint8 i = 0; i < PASSWORD_SIZE; i++)
    {
    	recive[i] = UART_recieveByte();
    	 _delay_ms(200);
    }
}
void Hand_Chacking(void)
{
	if (Check_PassMatching()) {
		while (UART_recieveByte() != UART_Ready) {}
		UART_sendByte(TRUE);
	}
	else {
		while (UART_recieveByte() != UART_Ready) {}
		UART_sendByte(FALSE);
	}
}
/*
* Function to save the password in eeprom
* get the byte and store it in eeprom on continuous addresses
*/
void save_password(void)
{
	UART_sendByte(UART_Ready);
	for (int i = 0; i < PASSWORD_SIZE; i++) {
		EEPROM_writeByte((ADDRESSS + i), UART_recieveByte());
		_delay_ms(200);
	}
}


void Buzzer_on(void)
{
	buzzer_sec = 60;
	Timer1_setCallBack(Buzzer_off);
	Buzzer_ON_OFF(ON);
	Timer1_init(&config_ptr);
}
/*
* Description : call back Function to stop the buzzer after 60 secs
* 1.increment counter
* 2.if the counter reaches 60 turn off the buzzer
* 3.send "Turn_off_buzzer" message to the other ecu
* 4.disable the timer
*/

void Buzzer_off(void)
{
    g_tick++;
    if (g_tick == 120)
    {
		g_tick = 0;
		g_count++;
		if (g_count == buzzer_sec) {
			g_count = 0;
			Buzzer_ON_OFF(OFF);
			UART_sendByte(Turn_off_buzzer);
			Timer1_deInit();
		}
    }
}

/* Description : Function to open the gate door
*  1.increment counter
* 2.if the counter reaches 15 sec reverse the motor
* 3.send open_the_door message to the other ecu
* 4.if counter reaches 15 sec stop the motor
* 5.disable the timer
*/

void open_Door(void) {
	time_requier=5;
	Timer1_setCallBack(Motor_ON);
	DcMotor_Rotate(ACW , 100);
	Timer1_init(&config_ptr);

    while(flag != 1);
	time_requier =1;
	Timer1_setCallBack(Motor_ON);
	DcMotor_Rotate(STOP , 0);
	Timer1_init(&config_ptr);

	while(flag != 2);
	UART_sendByte(open_the_door);
	time_requier =5;
	Timer1_setCallBack(Motor_ON);
	DcMotor_Rotate(CW , 100);
	Timer1_init(&config_ptr);

}
/*
* Description : call back Function
* 1.increment counter
* 2.if the counter reaches 2 reverse the motor
* 3.send gate_open message to the other ecu
* 4.if counter reaches 4 stop the motor
* 5.disable the timer
*/

void Motor_ON(void)
{
	g_tick++;
	if (g_tick == 33)
	{
		g_tick=0;
		g_count++;
		if(g_count == time_requier)
		{
			g_count=0;
			DcMotor_Rotate(STOP , 0);
			Timer1_deInit();
			flag ++;
			if(flag == 3)
			UART_sendByte(close_the_door);
		}
	}
}

