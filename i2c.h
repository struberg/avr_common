#ifndef __COMMON_I2C_H__
    #define __COMMON_I2C_H__

#include <stdbool.h>

#define TWI0_BAUD(F_SCL, T_RISE)                                                                                       \
	((((((float)F_CPU / (float)F_SCL)) - 10 - ((float)F_CPU * T_RISE / 1000000))) / 2)

typedef enum {
	I2C_NOERR, // The message was sent.
	I2C_BUSY,  // Message was NOT sent, bus was busy.
	I2C_FAIL   // Message was NOT sent, bus failure
	           // If you are interested in the failure reason,
	           // Sit on the event call-backs.
} i2c_error_t;


/**
 * @brief set the baud rate
 * e.g. for 400kHz pass
 * (uint8_t)TWI0_BAUD(400000, 0)
 */
void i2c_setup(uint8_t baud);

void i2c_start_write(uint8_t address);
uint8_t i2c_start_read(uint8_t address);

bool i2c_write_byte(uint8_t data);

bool i2c_read_bytes(uint8_t* pData, uint8_t len);

void i2c_stop(void);

#endif