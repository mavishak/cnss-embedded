/*
 * i2c.h
 *
 *  Created on: 28 Mar 2021
 *      Author: Owner
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f103xb.h" //this file is included to every header file
#include "common.h"


void init_i2c1(void);
uint8_t I2C1_Read1Byte(uint8_t DeviceAddr, uint8_t addr);


void I2C_start(uint8_t address);//, uint8_t direction);
void I2C_write(uint8_t data);
void I2C_stop();
uint8_t I2C_read_nack(); //???








void test_communication(void);

/*
 * device_address: the address of the slave.
 * mem_address: the address in the slave internal memory ??
 * data: the content to write.
 * length: data size.
 *
 * Qeustion: should the type be uint8_t? (see: refrence manual p. 754)
 * */
void i2c_write(uint8_t device_address,uint8_t mem_address, uint8_t data, uint8_t length);

/*
 * device_address: the address of the slave.
 * length: the number of bytes we expect to recieve ??
 * */
void i2c_read(uint8_t device_address, uint8_t length);

#endif /* I2C_H_ */
