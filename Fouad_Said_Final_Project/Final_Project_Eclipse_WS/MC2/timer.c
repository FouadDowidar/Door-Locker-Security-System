 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the AVR TIMER driver
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/

#include "gpio.h"
#include "timer.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h> /* To use Timer Registers */
#include <avr/interrupt.h> /* For TIMER ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr0)(void) = NULL_PTR;

static volatile void (*g_callBackPtr1)(void) = NULL_PTR;

static volatile void (*g_callBackPtr2)(void) = NULL_PTR;
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr0 != NULL_PTR)
	{
		/* Call the Call Back function in the application*/
		(*g_callBackPtr0)();/* another method to call the function using pointer to function g_callBackPtr0(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr0 != NULL_PTR)
	{
		/* Call the Call Back function in the application*/
		(*g_callBackPtr0)();/* another method to call the function using pointer to function g_callBackPtr0(); */
	}
}

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr1 != NULL_PTR)
	{
		/* Call the Call Back function in the application*/
		(*g_callBackPtr1)();/* another method to call the function using pointer to function g_callBackPtr1(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr1 != NULL_PTR)
	{
		/* Call the Call Back function in the application*/
		(*g_callBackPtr1)();/* another method to call the function using pointer to function g_callBackPtr1(); */
	}
}

ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtr2 != NULL_PTR)
	{
		/* Call the Call Back function in the application*/
		(*g_callBackPtr2)();/* another method to call the function using pointer to function g_callBackPtr2(); */
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr2 != NULL_PTR)
	{
		/* Call the Call Back function in the application*/
		(*g_callBackPtr2)();/* another method to call the function using pointer to function g_callBackPtr2(); */
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description : Function to initialize the TIMER driver
 * 	1. Set the required timer.
 * 	2. Set the required Mode.
 * 	3. Set the required Prescaler.
 * 	4. Set the required compare value.
 * 	4. Initialize timer Registers
 */

void Timer_init(const Timer_ConfigType * Config_Ptr)
{
	/*Enable global interrupts in MC by setting the I-Bit.*/
	SREG  |= (1<<7);

	if(Config_Ptr->timer == TIMER0)
	{
		/*Set Timer0 initial value*/
		TCNT0 = Config_Ptr->inital_value;

		/*Set Compare Value*/
		OCR0  = Config_Ptr->compare_value;

		/*Set Prescalar value
		  CS02,CS01,CS00
		  IN TCCR0 Register*/
		TCCR0 = (( TCCR0 & 0xF8 ) | (Config_Ptr->prescaler));

		if(Config_Ptr->mode == NORMAL)
		{
			/* Configure timer0 control register
			 * 1. Non PWM mode FOC0=1
			 * 2. Normal Mode WGM01=0 & WGM00=0
			 * 3. Normal Mode COM00=0 & COM01=0*/
			TCCR0 |= (1<<FOC0);
			TCCR0 &= (~(1<<WGM00)) & (~(1<<WGM01));

			/*Enable Timer0 Overflow Interrupt*/
			TIMSK |= (1<<TOIE0);
		}
		else if(Config_Ptr->mode == CTC)
		{
			/* Configure timer0 control register
			 * 1. Non PWM mode FOC0=1
			 * 2. CTC Mode WGM01=1 & WGM00=0
			 * 3. No need for OC0 so COM00=0 & COM01=0
			 */
			TCCR0 |= (1<<FOC0) | (1<<WGM01);
			TCCR0 &= (~(1<<WGM00));

			/*Enable Timer0 Compare Interrupt*/
			TIMSK |= (1<<OCIE0);
		}
		else if(Config_Ptr->mode == FAST_PWM)
		{
			/* Configure timer0 control register
			 * 1. Fast PWM mode FOC0=0
			 * 2. Fast PWM Mode WGM01=1 & WGM00=1
			 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
			 */
			TCCR0 |= (1<<WGM00) | (1<<WGM01) | (1<<COM01);

            /*To be able to generate an output from the PWM in Timer 0,
			the Output Compare Pin of Timer0 (OC0) must be set as output pin.*/
			GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);

		}
	}
	else if(Config_Ptr->timer == TIMER1)
	{
		/*Set Timer1 initial value*/
		TCNT1 = Config_Ptr->inital_value;

		/*Set Compare Value*/
		OCR1A  = Config_Ptr->compare_value;

		/*Set Prescalar value
		  CS12,CS11,CS10
		  IN TCCR1B Register*/
		TCCR1B = (( TCCR1B & 0xF8 ) | (Config_Ptr->prescaler));

		if(Config_Ptr->mode == NORMAL)
		{
			/* Configure timer1 control register TCCR1A
			 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
			 * 1. Non PWM mode FOC1A=1 FOC1B=0
			 * 2. Normal Mode WGM10=0 WGM11=0 (Mode Number 0)
			 */
			TCCR1A |= (1<<FOC1A);
			TCCR1A &= (~(1<<WGM11)) & (~(1<<WGM10));

			/* Configure timer1 control register TCCR1B
			 * 1. Normal Mode WGM12=0 WGM13=0 (Mode Number 0)
			 */
			TCCR1B &= (~(1<<WGM13)) & (~(1<<WGM12));

			/*Enable Timer1 Overflow Interrupt*/
			TIMSK |= (1<<TOIE1);
		}
		else if(Config_Ptr->mode == CTC)
		{
			/* Configure timer1 control register TCCR1A
			 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
			 * 2. FOC1A=1 FOC1B=0
			 * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4)
			 */
			TCCR1A |= (1<<FOC1A);
			TCCR1A &= (~(1<<WGM11)) & (~(1<<WGM10));

			/* Configure timer1 control register TCCR1B
			 * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
			 */
			TCCR1B |= (1<<WGM12);
			TCCR1B &= (~(1<<WGM13));

            /*Enable Timer1 Compare A Interrupt*/
			TIMSK  |= (1<<OCIE1A);
		}
		else if(Config_Ptr->mode == FAST_PWM)
		{
			/* Configure timer1 control register TCCR1A
			 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
			 * 2. Disconnect OC1B  COM1B0=0 COM1B1=0
			 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
			 * 4. Fast Pwm Mode WGM10=1 WGM11=1 (Mode Number 15)
			 */
			TCCR1A |= (1<<WGM11) | (1<<WGM10) | (1<<COM1A1);

			/* Configure timer1 control register TCCR1B
			 * 1. Fast Pwm Mode WGM12=1 WGM13=1 (Mode Number 15)
			 */
			TCCR1B |= (1<<WGM12) | (1<<WGM13);

			/*To be able to generate an output from the PWM in Timer 1,
			the Output Compare Pin of Timer1 (OC1A) must be set as output pin.*/
			GPIO_setupPinDirection(PORTD_ID,PIN5_ID,PIN_OUTPUT);
		}

	}
	else if(Config_Ptr->timer == TIMER2)
	{
		/*Set Timer2 initial value*/
		TCNT2 = Config_Ptr->inital_value;

		/*Set Compare Value*/
		OCR2  = Config_Ptr->compare_value;

		/*Asynchronous Status Register – ASSR*/
		/*When AS2 is written to zero,
		  Timer/Counter 2 is clocked from the I/O clock, clkI/O.*/
		ASSR  &= (~(1<<AS2));

		/*Set Prescalar value
		  CS22,CS21,CS20
		  IN TCCR0 Register*/
		TCCR2 = (( TCCR2 & 0xF8 ) | (Config_Ptr->prescaler));

		if(Config_Ptr->mode == NORMAL)
		{
			/* Configure timer2 control register
			 * 1. Non PWM mode FOC2=1
			 * 2. Normal Mode WGM21=0 & WGM20=0
			 * 3. Normal Mode COM20=0 & COM21=0*/
			TCCR2 |= (1<<FOC2);
			TCCR2 &= (~(1<<WGM20)) & (~(1<<WGM21));

			/*Enable Timer2 Overflow Interrupt*/
			TIMSK |= (1<<TOIE2);
		}
		else if(Config_Ptr->mode == CTC)
		{
			/* Configure timer2 control register
			 * 1. Non PWM mode FOC2=1
			 * 2. CTC Mode WGM21=1 & WGM20=0
			 * 3. No need for OC2 so COM20=0 & COM21=0
			 */
			TCCR2 |= (1<<FOC2) | (1<<WGM21);
			TCCR2 &= (~(1<<WGM20));

			/*Enable Timer2 Compare Interrupt*/
			TIMSK |= (1<<OCIE2);
		}
		else if(Config_Ptr->mode == FAST_PWM)
		{
			/* Configure timer2 control register
			 * 1. Fast PWM mode FOC2=0
			 * 2. Fast PWM Mode WGM21=1 & WGM20=1
			 * 3. Clear OC2 when match occurs (non inverted mode) COM20=0 & COM21=1
			 */
			TCCR2 |= (1<<WGM20) | (1<<WGM21) | (1<<COM21);

			/*To be able to generate an output from the PWM in Timer 2,
			  the Output Compare Pin of Timer2 (OC2) must be set as output pin.*/
			GPIO_setupPinDirection(PORTD_ID,PIN7_ID,PIN_OUTPUT);
		}
	}
}

/*
 * Description: Function to set the Call Back function address.
 */

void Timer_setCallBack(void(*a_ptr)(void),const Timer_Number timer)
{
	if(timer == TIMER0)
	{
		/* Save the address of the Call back function in a global variable */
		g_callBackPtr0 = a_ptr;
	}
	else if(timer == TIMER1)
	{
		/* Save the address of the Call back function in a global variable */
		g_callBackPtr1 = a_ptr;
	}
	else if(timer == TIMER2)
	{
		/* Save the address of the Call back function in a global variable */
		g_callBackPtr2 = a_ptr;
	}
}

/*
 * Description: Function to disable the Timer to stop the TIMER Driver
 */

void Timer_DeInit(const Timer_Number timer)
{
	if(timer == TIMER0)
	{
		/* Clear All Timer0 Registers */
		/* Stop Timer0 by clear the Clock bits (CS00, CS01 and CS02)*/
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0  = 0;

		/* Disable the interrupt */
		TIMSK &= (~(1<<TOIE0)) & (~(1<<OCIE0));
	}
	else if(timer == TIMER1)
	{
		/* Clear All Timer1 Registers */
		/* Stop Timer1 by clear the Clock bits (CS10, CS11 and CS12)*/
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		OCR1A = 0;

		/* Disable the interrupt */
		TIMSK &= (~(TOIE1)) & (~(OCIE1A));
	}
	else if(timer == TIMER2)
	{
		/* Clear All Timer2 Registers */
		/* Stop Timer2 by clear the Clock bits (CS20, CS21 and CS22)*/
		TCCR2 = 0;
		TCNT2 = 0;
		OCR2  = 0;

		/* Disable the interrupt */
		TIMSK &= (~(1<<TOIE2)) & (~(1<<OCIE2));
	}
}
