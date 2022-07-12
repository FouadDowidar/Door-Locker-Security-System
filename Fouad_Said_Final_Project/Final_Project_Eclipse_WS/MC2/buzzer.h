/******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the AVR BUZZER driver
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT_ID    PORTC_ID
#define BUZZER_PIN_ID     PIN5_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*Description : Function to initialize the BUZZER driver*/
void Buzzer_init(void);

/*Description: Function to Turn ON the BUZZER.*/
void Buzzer_on(void);

/*Description: Function to Turn OFF the BUZZER.*/
void Buzzer_off(void);

#endif /* BUZZER_H_ */
