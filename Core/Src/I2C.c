/*
 * I2C.c
 *
 *  Created on: Jul 21, 2024
 *      Author: musta
 */
#include "I2C.h"
#include "stm32f446xx.h"

void I2C_Config(void) {

	// Enable the I2C CLOCK and GPIO CLOCK
	RCC->APB1ENR |= (1 << 21);  // enable I2C CLOCK
	RCC->AHB1ENR |= (1 << 1);  // Enable GPIOB CLOCK

	// Configure the I2C PINs for ALternate Functions
	GPIOB->MODER |= (2 << 16) | (2 << 18); // Bits (17:16)= 1:0 --> Alternate Function for Pin PB8; Bits (19:18)= 1:0 --> Alternate Function for Pin PB9
	GPIOB->OTYPER |= (1 << 8) | (1 << 9);  //  Bit8=1, Bit9=1  output open drain
	GPIOB->OSPEEDR |= (3 << 16) | (3 << 18); // Bits (17:16)= 1:1 --> High Speed for PIN PB8; Bits (19:18)= 1:1 --> High Speed for PIN PB9
	GPIOB->PUPDR |= (1 << 16) | (1 << 18); // Bits (17:16)= 0:1 --> Pull up for PIN PB8; Bits (19:18)= 0:1 --> pull up for PIN PB9
	GPIOB->AFR[1] |= (4 << 0) | (4 << 4); // Bits (3:2:1:0) = 0:1:0:0 --> AF4 for pin PB8;  Bits (7:6:5:4) = 0:1:0:0 --> AF4 for pin PB9

	// Reset the I2C
	I2C1->CR1 |= (1 << 15);
	I2C1->CR1 &= ~(1 << 15);

	// Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
	I2C1->CR2 |= (90 << 0);  //

	// Configure the clock control registers
	I2C1->CCR = 30 << 0;  // 1 Mhz

	// Configure the rise time register
	I2C1->TRISE = 91;

	// Program the I2C_CR1 register to enable the peripheral
	I2C1->CR1 |= (1 << 0);  // Enable I2C
}

void I2C_Start(void) {

	I2C1->CR1 |= (1 << 10);  // Enable the ACK
	I2C1->CR1 |= (1 << 8);  // Generate START
	while (!(I2C1->SR1 & (1 << 0)));  // Wait fror SB bit to set
}

void I2C_Write(uint8_t data) {

	while (!(I2C1->SR1 & (1 << 7)));  // wait for TXE bit to set
	I2C1->DR = data;
	while (!(I2C1->SR1 & (1 << 2)));  // wait for BTF bit to set

}

void I2C_Address(uint8_t Address) {

	I2C1->DR = Address;  //  send the address
	while (!(I2C1->SR1 & (1 << 1)));  // wait for ADDR bit to set
	uint8_t temp = I2C1->SR1 | I2C1->SR2; // read SR1 and SR2 to clear the ADDR bit
}

void I2C_Stop(void) {

	I2C1->CR1 |= (1 << 9);  // Stop I2C

}


