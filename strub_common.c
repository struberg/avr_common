#include "strub_common.h"

#include <stdbool.h>
#include <util/delay.h>


inline char toBcd(uint8_t val) {
    if (val < 10) {
        return val + 0x30;
    }
    else {
        return ('A' - 10) + val;
    }
}

void toHexString_8(uint8_t value, char* buffer) {
    buffer[0] = toBcd((value & 0xf0) >> 4);
    buffer[1] = toBcd(value &  0x0f);
    buffer[2] = 0;
}

void toHexString_16(uint16_t value, char* buffer) {
    buffer[0] = toBcd((value & 0xf000) >> 12);
    buffer[1] = toBcd((value & 0x0f00) >> 8);
    buffer[2] = toBcd((value & 0x00f0) >> 4);
    buffer[3] = toBcd(value &  0x000f);
    buffer[4] = 0;
}

void toHexString_32(uint32_t value, char* buffer) {
    toHexString_16((uint16_t)((value &0xFFFF0000) >> 16), buffer);
    toHexString_16((uint16_t)(value &0x0000FFFF), buffer+4);
}

// e.g. CALC_DIGIT(uint8_t, 10)
#define CALC_DIGIT(valueType, digitValue, bufferPos) \
    for (valueType i = 0; i < 10; i++) { \
        if (rest >= digitValue) { \
            rest -=digitValue; \
        } \
        else { \
            if (nr || i> 0 ) { \
                buffer[bufferPos] = toBcd(i); \
                nr = true; \
            } \
            else { \
                buffer[bufferPos] = ' '; \
            } \
            break; \
        } \
    }
// END define

void toBcdString_8(uint8_t value, char* buffer) {
    uint8_t rest = value;
    bool nr = false;

    // hundreds
    if (value >= 200) {
        buffer[0] = '2';
        rest = value - 200;
        nr = true;
    }
    else if (value >=100) {
        buffer[0] = '1';
        rest = value - 100;
        nr = true;
    }
    else {
        buffer[0] = ' ';
    }

    // tens
    CALC_DIGIT(uint8_t, 10, 1)
    
    // ones
    buffer[2] = toBcd(rest);
    buffer[3] = 0;
}

void toBcdString_16(uint16_t value, char* buffer) {
    uint16_t rest = value;
    bool nr = false;

    // 10k
    CALC_DIGIT(uint16_t, 10000, 0)
    CALC_DIGIT(uint16_t, 1000 , 1)
    CALC_DIGIT(uint16_t, 100  , 2)
    CALC_DIGIT(uint16_t, 10   , 3)
    // ones
    buffer[4] = toBcd(rest);
    buffer[5] = 0;
}

void toBcdString_32(uint32_t value, char* buffer) {
    uint16_t rest = value;
    bool nr = false;

    // 10k
    CALC_DIGIT(uint32_t, 1000000000, 0)
    CALC_DIGIT(uint32_t, 100000000, 1)
    CALC_DIGIT(uint32_t, 10000000, 2)
    CALC_DIGIT(uint32_t, 1000000, 3)
    CALC_DIGIT(uint32_t, 100000, 4)
    CALC_DIGIT(uint32_t, 10000, 5)
    CALC_DIGIT(uint32_t, 1000 , 6)
    CALC_DIGIT(uint32_t, 100  , 7)
    CALC_DIGIT(uint32_t, 10   , 8)
    // ones
    buffer[9] = toBcd(rest);
    buffer[10] = 0;
 }


#define SER_PORT VPORTB
#define SER_CLK  PIN4_bp
#define SER_DATA PIN5_bp



void ser_out(uint8_t val) {
    static bool ser_initialised = false;

    if (!ser_initialised) {
        
        PORTB.OUTCLR = (1<<SER_CLK) | (1<<SER_DATA);
        PORTB.DIRSET = (1<<SER_CLK) | (1<<SER_DATA);
        ser_initialised = true;
    }


    for (int i= 0; i <8; i++) {
        clear_bit(VPORTB_OUT, SER_CLK);
        
        if (val & 0x01) {
            set_bit(VPORTB_OUT, SER_DATA);
        }
        else {
            clear_bit(VPORTB_OUT, SER_DATA);
        }
        set_bit(VPORTB_OUT, SER_CLK);
        val = val >>1;
    }
    
}
