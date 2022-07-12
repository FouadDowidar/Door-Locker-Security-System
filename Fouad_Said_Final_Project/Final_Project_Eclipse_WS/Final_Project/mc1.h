 /******************************************************************************
 *
 * Module: Door Locker Security System
 *
 * File Name: mc1.h
 *
 * Description: Header file for the Door Locker Security System
 *
 * HUMAN-MACHINE INTERFACE ECU
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/

#ifndef MC1_H_
#define MC1_H_

#include"uart.h"
#include"timer.h"
#include"keypad.h"
#include"lcd.h"
#include <avr/io.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define F_CPU 8000000
#define SIZE 6
#define OPEN_DOOR   '+'
#define CHANGE_PASS '-'

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 *  Description : Function to initialize the TIMER driver
 * 	1. Set the required timer.
 * 	2. Set the required Mode.
 * 	3. Set the required Prescaler.
 * 	4. Set the required compare value.
 */
void delay(uint16 period);

void startDelay(void);

/*
 *  Description : Function for setting password
 */
void setPassword(void);

/*
 *  Description : Main-Option Function for Opening the Door or Changing Password
 */
void Main_Option(void);

#endif /* MC1_H_ */
