 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Fouad Dowidar
 *
 *******************************************************************************/ 

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	KBPS_100=0X20 , KBPS_400=0X02
}Bit_Rate;

typedef struct
{
	uint8 slave_address;
	Bit_Rate bit_rate;
}TWI_ConfigType;

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 *  Description : Function for Initialize the TWI/I2C Driver
 */
void TWI_init(const TWI_ConfigType * Config_Ptr);

/*
 *  Description : Function for Sending Start Bit
 */
void TWI_start(void);

/*
 *  Description : Function for Sending Stop Bit
 */
void TWI_stop(void);

/*
 *  Description : Function for write(Send) Byte
 */
void TWI_writeByte(uint8 data);

/*
 *  Description : Function for read(Recieve) Byte with acknowledgment(ACK)
 */
uint8 TWI_readByteWithACK(void);

/*
 *  Description : Function for read(Recieve) Byte with negative-acknowledgement(NACK)
 */
uint8 TWI_readByteWithNACK(void);

/*
 *  Description : Function for getting Status
 */
uint8 TWI_getStatus(void);

#endif /* TWI_H_ */
