/* 
 * File:   I2C.h
 * Author: xpress_embedo
 *
 * Created on 1 February, 2020, 3:49 AM
 */

#ifndef I2C_H
#define I2C_H

#ifdef  __cplusplus
extern "C" {
#endif

/*Public Function Declaration*/
void I2C_Init( void );
void I2C_Start( void );
void I2C_Restart( void );
void I2C_Stop( void );
void I2C_Wait( void );
void I2C_Send( uint8_t data );

#ifdef  __cplusplus
}
#endif

#endif  /* I2C _H*/