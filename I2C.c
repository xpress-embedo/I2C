/*
 * @file i2x.c
 * @author xpress_embedo
 * @date 1 Feb, 2020
 * 
 * @brief This file contains I2C drivers for PIC18F micro-controller
 *
 */

#include "I2C.h"

/* Function Definition */


/**
 * @brief I2C Initialization
 * Call this function as follow
 * @code
 * I2C_Init(I2C_STANDARD_SPEED);
 * @endcode
 * @param speed Parameter to control I2C Speed
 */
void I2C_Init( uint8_t speed )
{
  /*
  Serial Clock(SCL) - RB1
  Serial Data(SDA)  - RB0
  Configure these pins as Input
  */
  TRISB |= 0x03;
  /* Slew Rate Disabled for Standard Speed & Enabled for High Speed */
  if( speed == I2C_STANDARD_SPEED )
  {
    SSPSTAT |= 0x80;
  }
  else
  {
    SSPSTAT &= ~0x80;
  }
  /*
   * WCOL:0
   * SSPOV:0
   * SSPEN:1 -> Enables Serial Port & configures the SDA and SCL as serial port pins
   * CKP:0
   * SSPM3:SSPM0:1000 -> I2C Master Mode, clock=FOSC/(4*(SSPADD+1))
   * FOSC = 48MHZ, and to get a clock of 100Khz => SSPADD = 119u
   */
  SSPCON1 = 0b00101000;
  SSPADD = 119u;            /* 100KHz Frequency */
}

/**
 * @brief I2C Start Signal
 */
void I2C_Start( void )
{
  /* Initiate Start Condition on SDA and SCL Lines */
  SSPCON2bits.SEN = 1;
  /* Wait till start condition is over, this bit is automatically cleared by HW */
  while (SSPCON2bits.SEN == 1)
    continue;
}

/**
 * @breif I2C Re-Start Signal
 */
void I2C_Restart( void )
{
  /* Initiate Re-start Condition on SDA and SCL Lines */
  SSPCON2bits.RSEN = 1;
  /* Wait till start condition is over, this bit is automatically cleared by HW */
  while (SSPCON2bits.RSEN == 1)
    continue;
}

/**
 * @breif I2C Stop Signal
 */
void I2C_Stop(void)
{
  /* Initiate Stop condition on SDA & SCL Lines*/
  SSPCON2bits.PEN=1;
  while(SSPCON2bits.PEN==1)
    continue;
}

/**
 * @breif I2C Wait Signal
 */
void I2C_Wait(void)
{
  /* Wait till transmission is in progress */
  while( SSPSTATbits.R_NOT_W == 1 )
    continue;
  /* If ACKSTAT bit is 0 Acknowledgment Received Successfully else not*/
  if( SSPCON2bits.ACKSTAT == 1 )
  {
    I2C_Stop();
  }
}

/**
 * @breif I2C Send Data
 * @param data data to sent over I2C Bus
 */
void I2C_Send( uint8_t data )
{
  SSPBUF = data;            /* Move data to SSPBUF */
  while(SSPSTATbits.BF);    /* wait till complete data is sent from buffer */
  I2C_Wait();               /* wait for any pending transfer */
}

/**
 * @breif I2C Receive Data
 * @return Data read from the I2C Bus
 */
uint8_t I2C_Read( void )
{
  uint8_t temp;
  SSPCON2bits.RCEN = 1;         /* Enable data reception */
  while(SSPSTATbits.BF == 0)    /* wait for buffer full */
    continue;
  temp = SSPBUF;                /* Read serial buffer and store in temp register */
  I2C_Wait();                   /* wait to check any pending transfer */
  SSPCON2bits.ACKDT=1;			/* send not acknowledge */
  SSPCON2bits.ACKEN=1;
  while(SSPCON2bits.ACKEN == 1) 
    continue;
  //I2C_Stop();
  return temp;                  /* Return the read data from bus */
}
