 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the AVR TIMER driver
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	TIMER0,TIMER1,TIMER2
}Timer_Number;

typedef enum
{
	NORMAL,PHASE_CORRECT_PWM,CTC,FAST_PWM
}Timer_Mode;

typedef enum{
	NO_CLOCK,NO_PRESCALER,PRESCALER_8,PRESCALER_64,PRESCALER_256,PRESCALER_1024,EXTERNAL_CLOCK_FALLING,EXTERNAL_CLOCK_RISING,
	T2_PRESCALAR_32=3,T2_PRESCALAR_64=4,T2_PRESCALAR_128=5,T2_PRESCALAR_256=6,T2_PRESCALAR_1024=7
}Timer_Prescaler;

typedef struct
{
	/*Set initial value*/
	uint16 inital_value;

	/*Set Compare Value*/
	uint16 compare_value;

	Timer_Number timer;
	Timer_Mode mode;
	Timer_Prescaler prescaler;
}Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description : Function to initialize the TIMER driver
 * 	1. Set the required timer.
 * 	2. Set the required Mode.
 * 	3. Set the required Prescaler.
 * 	4. Set the required compare value.
 * 	4. Initialize timer Registers
 */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description: Function to set the Call Back function address.
 */
void Timer_setCallBack(void(*a_ptr)(void),const Timer_Number timer);

/*
 * Description: Function to disable the Timer to stop the TIMER Driver
 */
void Timer_DeInit(const Timer_Number timer);

#endif /* TIMER_H_ */
