/*
 * File:   main.c
 * Author: xpress_embedo
 *
 * Created on 1 February, 2020, 3:41 AM
 */

#include "I2C.h"

/* Function Definition */
void I2C_Init( void )
{
  /*
  Serial Clock(SCL) - RB1
  Serial Data(SDA)  - RB0
  Configure these pins as Input
  */
  TRISB |= 0x03;
  SSPSTAT |= 0x80;        //Slew Rate Disabled
  SSPADD = 119;	
  SSPCON1 = 0b00101000;   //Master mode
  SSPADD = 119;
  /*SSPEN = 1
  Enables the Serial Port and configures the SDA and SCL
  Pins as the Serial Pins
  SSPM3:SSPM0 --> 1000
  I2C Master Mode
  Clock = Fosc/4*(SSPADD+1)
  */
}

void I2C_Start( void )
{
  SSPCON2bits.SEN = 1;
  //SSPCON2 bit 0
  while (SSPCON2bits.SEN == 1)
    continue;
  //SEN =1 initiate the Start Condition on SDA and SCL Pins
  //Automatically Cleared by Hardwre
  // 0 for Idle State
}
void I2C_Restart( void )
{
  SSPCON2bits.RSEN = 1;
  //SSPCON2 bit 1
  while (SSPCON2bits.RSEN == 1)
    continue;
  //RSEN = 1 initiate the Restart Condition
  //Automatically Cleared by Hardware
}
void I2C_Stop(void)
{
	SSPCON2bits.PEN=1;
	while(SSPCON2bits.PEN==1)
		continue;
}

void I2C_Wait(void)
{
  while(SSPSTATbits.R_NOT_W == 1)
    continue;
  if(SSPCON2bits.ACKSTAT == 1)
  {
    I2C_Stop();
  }
  //If ACKSTAT bit is 0 Acknowledgment Received Successfully
  //Otherwise Not
}

void I2C_Send(unsigned char dat)
{
	SSPBUF = dat;    /* Move data to SSPBUF */
  while(SSPSTATbits.BF);       /* wait till complete data is sent from buffer */
  I2C_Wait();       /* wait for any pending transfer */
}

unsigned char I2C_Read(void)
{
  uint8_t temp;
  SSPCON2bits.RCEN = 1;        /* Enable data reception */
  while(SSPSTATbits.BF == 0)      /* wait for buffer full */
    continue;
  temp = SSPBUF;   /* Read serial buffer and store in temp register */
  I2C_Wait();       /* wait to check any pending transfer */
  SSPCON2bits.ACKDT=1;				//send not acknowledge
  SSPCON2bits.ACKEN=1;
  while(SSPCON2bits.ACKEN == 1) 
    continue;
  //I2C_Stop();
    return temp;     /* Return the read data from bus */
}
