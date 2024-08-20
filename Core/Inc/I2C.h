/*
 * I2C.h
 *
 *  Created on: Jul 21, 2024
 *      Author: musta
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32f446xx.h"

void I2C_Config (void);
void I2C_Start (void);
void I2C_Write (uint8_t data);
void I2C_Address (uint8_t Address);
void I2C_Stop (void);

#endif /* INC_I2C_H_ */
