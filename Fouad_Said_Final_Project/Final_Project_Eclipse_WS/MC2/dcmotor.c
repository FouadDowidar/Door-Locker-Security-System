 /******************************************************************************
 *
 * Module: DC MOTOR
 *
 * File Name: dcmotor.c
 *
 * Description: Source file for the DC MOTOR driver
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/

#include "dcmotor.h"
#include "gpio.h"

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

/* Description :
 * The Function responsible for setup the direction
 * for the two motor pins through the GPIO driver.
 * Stop at the DC-Motor at the beginning through the GPIO driver.
 */

void DcMotor_Init(void)
{

	GPIO_setupPinDirection(PORTD_ID,PIN3_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(PORTD_ID,PIN4_ID,PIN_OUTPUT);

	GPIO_writePin(PORTD_ID,PIN3_ID,LOGIC_LOW);
	GPIO_writePin(PORTD_ID,PIN4_ID,LOGIC_LOW);

}

/* Description:
 * The function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.
 */

void DcMotor_Rotate(DcMotor_State state)
{
	switch(state)
	{
	case STOP:
		GPIO_writePin(PORTD_ID,PIN3_ID,LOGIC_LOW);
		GPIO_writePin(PORTD_ID,PIN4_ID,LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(PORTD_ID,PIN3_ID,LOGIC_LOW);
		GPIO_writePin(PORTD_ID,PIN4_ID,LOGIC_HIGH);
		break;
	case A_CW:
		GPIO_writePin(PORTD_ID,PIN3_ID,LOGIC_HIGH);
		GPIO_writePin(PORTD_ID,PIN4_ID,LOGIC_LOW);
		break;
	}
}
