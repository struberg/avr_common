#include "strub_common.h"
#include "i2c.h"



void i2c_setup(void) {
	TWI0.MBAUD = (uint8_t)TWI0_BAUD(400000, 0); /* set MBAUD register */

	TWI0.MCTRLA = 1 << TWI_ENABLE_bp        /* Enable TWI Master: enabled */
	              | 0 << TWI_QCEN_bp        /* Quick Command Enable: disabled */
	              | 1 << TWI_RIEN_bp        /* Read Interrupt Enable: enabled */
	              | 0 << TWI_SMEN_bp        /* Smart Mode Enable: disabled */
	              | TWI_TIMEOUT_DISABLED_gc /* Bus Timeout Disabled */
	              | 0 << TWI_WIEN_bp;       /* Write Interrupt Enable: disabled */

    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

void i2c_start_write(uint8_t address) {
    TWI0.MADDR = address <<1; // last bit is R/!W

    // wait until address is written to the bus
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
}

void i2c_start_read(uint8_t address) {
    TWI0.MADDR = address <<1 | 1; // last bit is R/!W

    // wait until address is written to the bus
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
}

bool i2c_write_byte(uint8_t data) {
    TWI0.MDATA = data;

    // wait until data is sent
    while (!(TWI0.MSTATUS & TWI_WIF_bm));

    // return if we got an NACK back
    return TWI0.MSTATUS & TWI_RXACK_bm;
    
}

void i2c_stop(void) {
    TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
}
