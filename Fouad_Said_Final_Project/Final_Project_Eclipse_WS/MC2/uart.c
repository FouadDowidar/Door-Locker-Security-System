 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const USART_ConfigType * Config_Ptr)
{
	uint16 ubrr_value = 0;

	/* U2X = 0 for normal transmission speed */
	/* U2X = 1 for double transmission speed */
	UCSRA = ((UCSRA & 0xFD) | ((Config_Ptr->op_mode)<<1));

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 5-bit or 6-bit or 7-bit or 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/ 
	UCSRB = (1<<RXEN) | (1<<TXEN);
	
	if(Config_Ptr->data_mode == BIT_9)
	{
		/*UCSZ2 = 1 For 9-bit data mode*/
		/*RXB8 & TXB8 used for 9-bit data mode*/
		UCSRB |= (1<<UCSZ2);
	}

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UMSEL   = 1 Synchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * UPM1:0  = 10 Enable Even parity bit
	 * UPM1:0  = 11 Enable Odd parity bit
	 * USBS    = 0 One stop bit
	 * USBS    = 1 Two stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCSZ1:0 = 10 For 7-bit data mode
	 * UCSZ1:0 = 01 For 6-bit data mode
	 * UCSZ1:0 = 00 For 5-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSRC |= (1<<URSEL);

	UCSRC = ((UCSRC & 0xBF)  | ((Config_Ptr->usart_mode)<<6));

	UCSRC = ((UCSRC & 0xCF)  | ((Config_Ptr->pa_mode)<<4));

	UCSRC = ((UCSRC & 0xF7)  | ((Config_Ptr->stop_mode)<<3));

	if(Config_Ptr->data_mode == BIT_9)
	{
		UCSRC |= (1<<UCSZ1) | (1<<UCSZ0);
	}
	else
	{
		UCSRC = ((UCSRC & 0xF9)  | ((Config_Ptr->data_mode)<<1));
	}

	/* Calculate the UBRR register value */
	if(Config_Ptr->op_mode == U2X_1)
	{
		ubrr_value = (uint16)(((F_CPU / (Config_Ptr->baud_rate * 8UL))) - 1);
	}
	else if(Config_Ptr->op_mode == U2X_0)
	{
		ubrr_value = (uint16)(((F_CPU / (Config_Ptr->baud_rate * 16UL))) - 1);
	}

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;

	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR;		
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	UART_sendByte(Str[i]);
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}		
	*******************************************************************/
}

/*
 * Description :
 * Receive the required string until the '\0' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '\0' */
	while(Str[i] != '\0')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

}
