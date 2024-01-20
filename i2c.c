#include "strub_common.h"
#include "i2c.h"



void i2c_setup(uint8_t baud) {
	TWI0.MBAUD = baud; /* set MBAUD register */

	TWI0.MCTRLA = 1 << TWI_ENABLE_bp        /* Enable TWI Master: enabled */
	              | 0 << TWI_QCEN_bp        /* Quick Command Enable: disabled */
	              | 0 << TWI_SMEN_bp        /* Smart Mode Enable: enabled */
	              | TWI_TIMEOUT_200US_gc    /* Bus Timeout */
	              | 0 << TWI_RIEN_bp        /* Read Interrupt Enable: disabled */
	              | 0 << TWI_WIEN_bp;       /* Write Interrupt Enable: disabled */

    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
    TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm | TWI_BUSERR_bm);

    // enable internal pullups on the ports
    PORTB.PIN0CTRL |= PORT_PULLUPEN_bm;
    PORTB.PIN1CTRL |= PORT_PULLUPEN_bm;
}

void i2c_start_write(uint8_t address) {
    TWI0.MADDR = address <<1; // last bit is R/!W

    // wait until address is written to the bus
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
}

uint8_t i2c_start_read(uint8_t address) {
    uint16_t i2cTimeout=0xf300;

    TWI0.MSTATUS |= TWI_WIF_bm | TWI_RIF_bm; // clear em
    TWI0.MADDR = address <<1 | 1; // last bit is R/!W

    // wait until address is written to the bus
    while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))) {
        i2cTimeout--;
        if (i2cTimeout == 0x0000) {
            return TWI0.MSTATUS;
        }
    }

    return 0;
}

bool i2c_write_byte(uint8_t data) {
    TWI0.MDATA = data;

    // wait until data is sent
    uint16_t i2cTimeout=0xf300;
    while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))) {
        i2cTimeout--;
        if (i2cTimeout == 0x0000) {
            return TWI0.MSTATUS;
        }
    }

    // return if we got an NACK back
    return TWI0.MSTATUS & TWI_RXACK_bm;
}

bool i2c_read_bytes(uint8_t* pData, uint8_t len) {
    for (uint8_t i=0; i < len; i++) {
        uint16_t i2cTimeout=0xFFFF;
        while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))) {
            i2cTimeout--;
            if (i2cTimeout == 0x0000) {
                return TWI0.MSTATUS;
            }
        }

        if (TWI0.MSTATUS & TWI_RIF_bm) {
            pData[i] = TWI0.MDATA;
            if (i == len-1) {
                // write an NACK to indicate this was the last byte.
                TWI0.MCTRLB |= TWI_ACKACT_NACK_gc;
            }
            else {
                // clear the NACK flag in case it was set before
                TWI0.MCTRLB &= ~(TWI_ACKACT_NACK_gc);
                // send the ACK so we get the next byte
                TWI0.MCTRLB |= TWI_MCMD_RECVTRANS_gc;
            }
        }

    }

    return 0;
}

void i2c_stop(void) {
    TWI0.MCTRLB |= TWI_MCMD_STOP_gc; // send stop condition
    int16_t i2cTimeout=0xFFFF;
    while (!(TWI0.MSTATUS & TWI_BUSSTATE_IDLE_gc)) {
        i2cTimeout--;
        if (i2cTimeout == 0x0000) {
            TWI0.MSTATUS = TWI_BUSERR_bm | TWI_BUSSTATE_IDLE_gc;
            return;
        }
    }
}
