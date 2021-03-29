/*
 * i2c.h
 *
 *  Created on: 28 Mar 2021
 *      Author: Owner
 */

#ifndef I2C_H_
#define I2C_H_

void init_i2c(void);

void i2c_write_single(uint8_t device_address,uint8_t mem_address, uint8_t data);

#endif /* I2C_H_ */
