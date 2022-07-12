 /******************************************************************************
 *
 * Module: Door Locker Security System
 *
 * File Name: mc2.c
 *
 * Description: Source file for the Door Locker Security System
 *
 * CONTROL-ECU
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/

#include"mc2.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

uint8 flag_Check = 0;
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
 *  Description : Function for Saving Password in EEPROM
 */
void Save_EEPROM(uint8 *Password1,uint8* Password2)
{
	uint8 result;
	uint8 i;

	/*Getting first password for comparing it with the second password*/
	UART_receiveString(Password1);

	delay(200);

	/*Getting second password for comparing it with the first password*/
	UART_receiveString(Password2);

	for(i=0;i<6;i++)
	{
		if(Password1[i] == Password2[i])
		{
			result=1;
		}
		else
		{
			result=0;
			break;
		}
	}

	/*Sending result of comparing two passwords*/
	UART_sendByte(result);

	if(result==0)
	{
		Save_EEPROM(Password1,Password2);
	}
	else if(result == 1)
	{

		/*Writing Password in EEPROM to Save it*/
		for(i=0 ; i<5 ; i++)
		{
			EEPROM_writeByte(0x0311+i,Password1[i]);
			delay(100);
		}
	}
}

/*
 *  Description : Function for Checking on Password
 */
void Check_Password(void)
{
	uint8 Password[SIZE];
	uint8 result = 1;
	uint8 EEPROM_PASS;
	uint8 i;

	/*Getting Password from MC1 to Check it with the Password saved in EEPROM*/
	UART_receiveString(Password);

	for(i=0 ; i<5 ; i++)
	{
		EEPROM_readByte(0x0311+i,&EEPROM_PASS);
		delay(100);

		if(Password[i] != EEPROM_PASS)
		{
			result = 0;
			flag_Check++;
			break;
		}
	}

	/*if Password is entered wrong three times the buzzer will alarm*/
	if(flag_Check == 3)
	{
		result = 2;
		UART_sendByte(result);
		flag_Check = 0;
		buzzer_Alarm();
		Check_Password();
	}
	if(result == 1)
	{
		UART_sendByte(result);
		return;
	}
	else if(result == 0)
	{
		UART_sendByte(result);
		Check_Password();
	}
}

/*
 *  Description : Function for making Buzzer work for 1-min
 */
void buzzer_Alarm(void)
{
	/*initialize the BUZZER driver*/
	Buzzer_init();

	/*Turn ON the BUZZER.*/
	Buzzer_on();

	delay(60000);

	/*Turn OFF the BUZZER.*/
	Buzzer_off();

}

/*
 *  Description : Function for OPENING DOOR using DC-Motor
 */
void open_Door(void)
{
	/*initialize the DC-MOTOR driver*/
	DcMotor_Init();

	/*rotate the DC Motor CW for 15sec*/
	DcMotor_Rotate(CW);
	delay(15000);

	/*stop the motor for 3sec*/
	DcMotor_Rotate(STOP);
	delay(3000);

	/*rotate the DC Motor A-CW for 15sec*/
	DcMotor_Rotate(A_CW);
	delay(15000);

	/*stop the motor*/
	DcMotor_Rotate(STOP);
}

/*
 *  Description : Main Function for CONTROL-ECU
 */
int main(void)
{
    uint8 Password1[SIZE];
	uint8 Password2[SIZE];
	uint8 result = 0;

	/* Setup the Frame format like number of data bits, parity bit type and number of stop bits.
	 * Setup the UART baud rate.
	 */
	USART_ConfigType Config_Ptr = {9600,ASYNCHRONOUS,BIT_8,U2X_1,DISABLED,BIT_1};

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_init(&Config_Ptr);

	/* Setup the Slave Address.
	 * Setup the TWI bit rate.
	 */
	TWI_ConfigType Config_Ptr_TWI = {0B00000001,KBPS_400};

	/* Initialize the TWI/I2C Driver */
	TWI_init(&Config_Ptr_TWI);

	while(1)
	{
		Save_EEPROM(Password1,Password2);

		Check_Password();

		result = UART_recieveByte();
		if(result == 1)
		{
			result = 0;
			open_Door();
		}
	}
}


