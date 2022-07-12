 /******************************************************************************
 *
 * Module: DC MOTOR
 *
 * File Name: dcmotor.h
 *
 * Description: Header file for the DC MOTOR driver
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum{
	STOP,CW,A_CW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Description :
 * The Function responsible for setup the direction
 * for the two motor pins through the GPIO driver.
 * Stop at the DC-Motor at the beginning through the GPIO driver.*/

void DcMotor_Init(void);

/* Description:
 * The function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.
 */

void DcMotor_Rotate(DcMotor_State state);


#endif /* DCMOTOR_H_ */
