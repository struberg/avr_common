#ifndef __COMMON_I2C_H__
    #define __COMMON_I2C_H__

#include <stdbool.h>

#define TWI0_BAUD(F_SCL, T_RISE)                                                                                       \
	((((((float)10000000 / (float)F_SCL)) - 10 - ((float)10000000 * T_RISE / 1000000))) / 2)


void i2c_setup(void);

void i2c_start_write(uint8_t address);
void i2c_start_read(uint8_t address);

bool i2c_write_byte(uint8_t data);

void i2c_stop(void);

#endif