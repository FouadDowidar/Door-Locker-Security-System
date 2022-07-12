 /******************************************************************************
 *
 * Module: Door Locker Security System
 *
 * File Name: mc1.c
 *
 * Description: Source file for the Door Locker Security System
 *
 * HUMAN-MACHINE INTERFACE ECU
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/

#include"mc1.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

uint8 Password1[SIZE];
uint8 Password2[SIZE];

uint8 flag_Set = 0;
uint16 g_Tick=0,final_Tick=0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 *  Description : Function to initialize the TIMER driver
 * 	1. Set the required timer.
 * 	2. Set the required Mode.
 * 	3. Set the required Prescaler.
 * 	4. Set the required compare value.
 */
void delay(uint16 period)
{
	g_Tick=0;
	final_Tick=period/100;
	Timer_ConfigType Config_Ptr={0,3125,TIMER1,CTC,PRESCALER_256};
	Timer_init(&Config_Ptr);
	Timer_setCallBack(startDelay,TIMER1);
	while(g_Tick!=final_Tick){}
}

void startDelay(void)
{
	g_Tick++;
	if(g_Tick==final_Tick)
	{
		Timer_DeInit(TIMER1);
	}
}

/*
 *  Description : Function for setting password
 */
void setPassword(void)
{

	uint8 key_num;
	uint8 i = 0;
	uint8 result;

	LCD_displayString("Enter Pass:");
	LCD_moveCursor(1,0);
	do
	{
		/* Get the pressed button from keypad */
		key_num = KEYPAD_getPressedKey();

		LCD_displayCharacter('*');

		Password1[i] = key_num ;

		delay(400);

		i++;

	}while(key_num != 13);

	Password1[i] = '\0';

	/*Sending the First entered Password to MC2 to compare it with the re-entered Password*/
	UART_sendString(Password1);

	i=0;

	LCD_clearScreen();
	LCD_displayString("re-Enter Pass:");
	LCD_moveCursor(1,0);

	do
	{
		/* Get the pressed button from keypad */
		key_num = KEYPAD_getPressedKey();

		LCD_displayCharacter('*');

		Password2[i] = key_num ;

		delay(400);

		i++;

	}while(key_num != 13);

	Password2[i] = '\0';

	/*Sending the Second entered Password to MC2 to compare it with the First Password*/
	UART_sendString(Password2);

	delay(200);

	/*Getting the result of comparing the two Passwords*/
	result = UART_recieveByte();

	if(result == 1)
	{
		LCD_clearScreen();
		LCD_displayString("Pass Matched:");
		delay(1000);
		LCD_clearScreen();
		LCD_displayString("PASSWORD IS SET");
		delay(1000);
		LCD_clearScreen();
		flag_Set = 1;
		return;
	}
	else
	{
		LCD_clearScreen();
		LCD_displayString("Pass Mismatched:");
		delay(1000);
		LCD_clearScreen();
		LCD_displayString("Back to step 1:");
		delay(1000);
		LCD_clearScreen();
		setPassword();
	}
}

/*
 *  Description : Main-Option Function for Opening the Door or Changing Password
 */
void Main_Option(void)
{
	uint8 Password[SIZE];
	uint8 key_num;
	uint8 Option;
	uint8 result;
	uint8 i = 0;

	LCD_displayString("+: OPEN DOOR");
	LCD_moveCursor(1,0);
	LCD_displayString("-: CHANGE PASS");

	do
	{
	  /* Get the pressed button from keypad */
	   Option = KEYPAD_getPressedKey();
	   delay(400);
	}while((Option != OPEN_DOOR) && (Option != CHANGE_PASS));

	LCD_clearScreen();
	LCD_displayString("Enter Pass:");
	LCD_moveCursor(1,0);

	do
	{
		/* Get the pressed button from keypad */
		key_num = KEYPAD_getPressedKey();

		LCD_displayCharacter('*');

		Password[i] = key_num ;

		delay(400);

		i++;

	}while(key_num != 13);

	Password[i] = '\0';

	/*Sending Password to MC2 to check on the password*/
	UART_sendString(Password);

	delay(200);

	/*Getting the result of the Password if it matched or not*/
	result = UART_recieveByte();

	if(result == 1)
	{
		if(Option == OPEN_DOOR)
		{
			LCD_clearScreen();

			/*Sending to MC2 to open door*/
			UART_sendByte(result);

			/*Door Rotates in CW for 15 sec*/
			LCD_displayString("DOOR IS OPENING");
			delay(15000);


			LCD_clearScreen();

			/*Door holds opened for 3 sec*/
			LCD_displayString("DOOR IS OPENED");
			delay(3000);


			LCD_clearScreen();

			/*Door Rotates in A_CW for 15 sec*/
			LCD_displayString("DOOR IS CLOSING");
			delay(15000);

			LCD_clearScreen();
			return;
		}
		else if(Option == CHANGE_PASS)
		{
			LCD_clearScreen();
			LCD_displayString("CHANGE PASS:");
			delay(1000);
			LCD_clearScreen();
			setPassword();
		}
	}
	else if(result == 0)
	{

		LCD_clearScreen();
		LCD_displayString("Wrong Pass:");
		delay(1000);
		LCD_clearScreen();
		Main_Option();
	}
	else if(result == 2)
	{
		LCD_clearScreen();
		LCD_displayString("ERROR:WARNING!");
		delay(60000);
		LCD_clearScreen();
		Main_Option();
	}
}

/*
 *  Description : Main Function for HMI-ECU
 */
int main(void)
{

	/* Setup the Frame format like number of data bits, parity bit type and number of stop bits.
	 * Setup the UART baud rate.
	 */
	USART_ConfigType Config_Ptr = {9600,ASYNCHRONOUS,BIT_8,U2X_1,DISABLED,BIT_1};

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_init(&Config_Ptr);

	/*Initialize the LCD:*/
	LCD_init();

	while(1)
	{
		if(flag_Set == 0)
		{
			setPassword();
		}
		Main_Option();

	}

}

