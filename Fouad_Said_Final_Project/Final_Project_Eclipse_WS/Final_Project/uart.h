 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

/*Set USART Mode*/
typedef enum
{
	ASYNCHRONOUS,SYNCHRONOUS
}USART_Mode;

/*Set the Data Mode*/
typedef enum
{
	BIT_5,BIT_6,BIT_7,BIT_8,RESERVED0,RESERVED1,RESERVED2,BIT_9
}Data_bits;

/*Set the Operating Mode (Asynchronous Normal Mode) or (Asynchronous Double Speed Mode)*/
typedef enum
{
	U2X_0,U2X_1
}Operating_Mode;

/*Set Parity Mode*/
typedef enum
{
	DISABLED,RESERVED,ENABLED_EVEN_PARITY,ENABLED_ODD_PARITY
}Parity_Mode;

/*Set Numbers Of Stop Bits*/
typedef enum
{
	BIT_1,BIT_2
}Stop_Bit;

typedef struct
{
	uint32 baud_rate;
	USART_Mode usart_mode;
	Data_bits data_mode;
	Operating_Mode op_mode;
	Parity_Mode pa_mode;
	Stop_Bit stop_mode;
}USART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const USART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '\0' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until \0

#endif /* UART_H_ */
