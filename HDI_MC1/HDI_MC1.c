/*
 * HDI_MC1.c
 *
 *  Created on: Mar 23, 2024
 *      Author: zyadm
 */


#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "Timer.h"
#include "std_types.h"
#include <util/delay.h>
#include <avr/io.h>

#define PASSWORD_SIZE 5
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

void Create_password(void);
uint8 Check_PassMatching(uint8 * pass,uint8 * pass_confirm);
void Get_Password(uint8 * pass);
void Send_password(uint8 *password);
void Open_Door(void);
void Chanage_Password(void);

static uint8 count =0;
static uint8 count1 =0;

int main(void)
{
	UART_ConfigType uart_config = {BIT_DATA_8, Even_parity, STOP_1, 9600};

	UART_init(&uart_config);
	LCD_init();
	while (UART_recieveByte() != UART_Ready);
	Create_password();
	while(1)
    {
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "+ : Open Door");
		LCD_displayStringRowColumn(1, 0, "- : Reset Password");
		/**********************waiting to enter + or -***************************/
		while (KEYPAD_getPressedKey() != '-' && KEYPAD_getPressedKey() != '+'){}
		/***********************************************************************/
		if(KEYPAD_getPressedKey() == '+')
		{
			_delay_ms(1000);
			Open_Door();
			//Send_password(Enter_password);
			/*Open door function*/
		}
		else if(KEYPAD_getPressedKey() == '-')
		{
			_delay_ms(1000);
			Chanage_Password();
			/*change password function*/
		}

    }
}

/*Description
 * To Display the password on lcd and check matching
 */
void Create_password(void)
{
	uint8 pass[PASSWORD_SIZE];
	uint8 pass_confirm[PASSWORD_SIZE];

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "plz enter pass");
	LCD_moveCursor(1, 0);
	Get_Password(pass);
	LCD_clearScreen();

	LCD_displayStringRowColumn(0, 0, "plz enter the ");
	LCD_moveCursor(1, 0);
	LCD_displayString("same pass:");
	Get_Password(pass_confirm);
	LCD_clearScreen();

	/******************************checking************************************/
	if (Check_PassMatching(pass,pass_confirm) == FALSE) {
		LCD_displayStringRowColumn(0, 2, "UnMatched");/*For not matching*/
		_delay_ms(1000);
		LCD_clearScreen();
		Create_password();

	} else {
		LCD_displayStringRowColumn(0, 2, "Confirmed!");/*For matching*/
		_delay_ms(1000);
		LCD_clearScreen();
		UART_sendByte(SET_PASSWORD);
		//send the password by uart to save it in eeprom
		Send_password(pass);
	}
}

void Open_Door(void)
{
	uint8 Enter_password[PASSWORD_SIZE];
	count++;
	if(count == 4)
	{
		LCD_clearScreen();
		UART_sendByte(Turn_on_buzzer);
		LCD_displayStringRowColumn(0, 0, "Error");
		while (UART_recieveByte() != Turn_off_buzzer){}
		LCD_displayStringRowColumn(0, 0, "u can pass!");
		_delay_ms(1000);
		count = 0;
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "+ : Open Door");
		LCD_displayStringRowColumn(1, 0, "- : Reset Password");
		while (KEYPAD_getPressedKey() != '-' && KEYPAD_getPressedKey() != '+'){}
		if(KEYPAD_getPressedKey() == '+')
		{
			_delay_ms(500);
			Open_Door();

		}
		else if(KEYPAD_getPressedKey() == '-')
		{
			_delay_ms(500);
			Chanage_Password();
		}
	}
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "plz enter Pass:");
	LCD_moveCursor(1, 0);
	Get_Password(Enter_password);

	UART_sendByte(Enter_PASSWORD);
	Send_password(Enter_password);
	UART_sendByte(UART_Ready);

	if(UART_recieveByte() == TRUE)//matching password
	{
		UART_sendByte(open_door);
		LCD_clearScreen();
		LCD_displayString("Door is ");
		LCD_moveCursor(1,0);
		LCD_displayString("Unlocking");
		while (UART_recieveByte() != open_the_door){}
		LCD_clearScreen();
		LCD_displayString("Door is");
		LCD_moveCursor(1,0);
		LCD_displayString("locking");
		while (UART_recieveByte() != close_the_door) {}
		count = 0;
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "+ : Open Door");
		LCD_displayStringRowColumn(1, 0, "- : Reset Password");
		while (KEYPAD_getPressedKey() != '-' && KEYPAD_getPressedKey() != '+'){}
		if(KEYPAD_getPressedKey() == '+')
		{
			_delay_ms(500);
			Open_Door();

		}
		else if(KEYPAD_getPressedKey() == '-')
		{
			_delay_ms(500);
			Chanage_Password();
		}
	}
	else
	{
		LCD_clearScreen();
		LCD_displayString("Wrong Password");
		_delay_ms(3000);
		Open_Door();

	}

}



void Chanage_Password(void)
{
	uint8 Enter_password[PASSWORD_SIZE];
	count1++;
	if (count1 == 4) {
		LCD_clearScreen();
		UART_sendByte(Turn_on_buzzer);
		LCD_displayStringRowColumn(0, 0, "Unsafe Condition");
		_delay_ms(500);
		while (UART_recieveByte() != Turn_off_buzzer){}
		count1 = 0;
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "+ : Open Door");
		LCD_displayStringRowColumn(1, 0, "- : Reset Password");
		while (KEYPAD_getPressedKey() != '-' && KEYPAD_getPressedKey() != '+'){}
		if(KEYPAD_getPressedKey() == '+')
		{
			_delay_ms(500);
			Open_Door();

		}
		else if(KEYPAD_getPressedKey() == '-')
		{
			_delay_ms(500);
			Chanage_Password();
		}
	}
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Enter Password");
	LCD_moveCursor(1, 0);
	Get_Password(Enter_password);

	UART_sendByte(Enter_PASSWORD);
	Send_password(Enter_password);
	UART_sendByte(UART_Ready);

	if(UART_recieveByte() == TRUE)//matching password
	{
		Create_password();
		LCD_clearScreen();
		LCD_displayString("Password changed!");
		_delay_ms(500);
		count1 = 0;
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "+ : Open Door");
		LCD_displayStringRowColumn(1, 0, "- : Reset Password");
		while (KEYPAD_getPressedKey() != '-' && KEYPAD_getPressedKey() != '+'){}
		if(KEYPAD_getPressedKey() == '+')
		{
			_delay_ms(500);
			Open_Door();

		}
		else if(KEYPAD_getPressedKey() == '-')
		{
			_delay_ms(500);
			Chanage_Password();
		}
	}
	else {
		LCD_clearScreen();
		LCD_displayString("Wrong Password");
		_delay_ms(500);
		Chanage_Password();
	}

}

/*Description
 * To get the password and save it in buffer
 */
void Get_Password(uint8 * pass)
{
	for(uint8 i=0 ; i< PASSWORD_SIZE ; i++)
	{
		/* Get the pressed button from keypad */
		pass[i] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		//LCD_intgerToString(pass_buffer);
		_delay_ms(500);
	}
}
/*Description
 * To check if password and confirm are matching or not
 */
uint8 Check_PassMatching(uint8 * pass,uint8 * pass_confirm)
{
	for (uint8 i = 0; i <  PASSWORD_SIZE; i++) {
		if(pass[i]!=pass_confirm[i])
		{
			return FALSE;
		}
	}
	return TRUE;
}
/*Description
 * To send the password to 2nd ECU using UART
 */
void Send_password(uint8 *password)
{
	 while (UART_recieveByte() != UART_Ready);
    for ( uint8 i = 0; i < PASSWORD_SIZE; i++)
    {
        UART_sendByte(password[i]);
        _delay_ms(200);
    }
}

