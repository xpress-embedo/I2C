/*
 * @file i2c.h
 * @author xpress_embedo
 * @date 1 Feb, 2020
 * 
 * @brief This file contains I2C drivers for PIC18F micro-controller
 *
 */

#ifndef I2C_H
#define I2C_H

#include "main.h"

#ifdef  __cplusplus
extern "C" {
#endif

// I2C Speed Control Macros
#define I2C_STANDARD_SPEED        0u
#define I2C_HIGH_SPEED            1u

/*Public Function Declaration*/
void I2C_Init( uint8_t speed );
void I2C_Start( void );
void I2C_Restart( void );
void I2C_Stop( void );
void I2C_Wait( void );
void I2C_Send( uint8_t data );
uint8_t I2C_Read ( void );

#ifdef  __cplusplus
}
#endif

#endif  /* I2C _H*/