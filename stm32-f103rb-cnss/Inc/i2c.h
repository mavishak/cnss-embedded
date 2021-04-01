/*
 * i2c.h
 *
 *  Created on: 28 Mar 2021
 *      Author: Owner
 */

#ifndef I2C_H_
#define I2C_H_

void init_i2c(void);

void i2c_write(uint8_t device_address,uint8_t mem_address, uint8_t data, uint8_t length);

void i2c_read(uint8_t device_address, uint8_t length);

#endif /* I2C_H_ */
