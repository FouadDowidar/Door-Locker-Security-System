 /******************************************************************************
 *
 * Module: Door Locker Security System
 *
 * File Name: mc2.h
 *
 * Description: Header file for the Door Locker Security System
 *
 * CONTROL-ECU
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/

#ifndef MC2_H_
#define MC2_H_

#include"uart.h"
#include"twi.h"
#include"timer.h"
#include"external_eeprom.h"
#include"dcmotor.h"
#include"buzzer.h"
#include"avr/io.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define F_CPU 8000000
#define SIZE 6

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
 *  Description : Function for Saving Password in EEPROM
 */
void Save_EEPROM(uint8 *Password1,uint8* Password2);

/*
 *  Description : Function for Checking on Password
 */
void Check_Password(void);

/*
 *  Description : Function for making Buzzer work for 1-min
 */
void buzzer_Alarm(void);

/*
 *  Description : Function for OPENING DOOR using DC-Motor
 */
void open_Door(void);

#endif /* MC2_H_ */
