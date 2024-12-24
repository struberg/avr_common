/*
 * Copyright 2018-2024 Mark Struberg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "tm1638.h"

#define SET_LED PORTA.OUTSET = PIN7_bm;
#define CLR_LED PORTA.OUTCLR = PIN7_bm;


void tm1638_init(void) {
    // switch the pins to output mode
    TM1638_PORT.DIRSET = TM1638_CLK_PIN | TM1638_STB_PIN | TM1638_DIO_PIN;

    TM1638_PORT.OUTSET = TM1638_STB_PIN; //passive mode STB high
    TM1638_PORT.OUTCLR = TM1638_CLK_PIN | TM1638_DIO_PIN; // clk starts with low and data 0

    tm1638_sendCmd(TM1638_CMD_SET_BRIGHTNESS | 0xB);
    tm1638_sendCmd(TM1638_CMD_AUTO_INCREMENT);

    // clear all data registers
    TM1638_PORT.OUTCLR = TM1638_STB_PIN;
    tm1638_sendDataByte(TM1638_CMD_SET_ADDRESS | 0x0);
    for (uint8_t i=0; i<16; i++) {
        tm1638_sendDataByte(0x80);
    }
    TM1638_PORT.OUTSET = TM1638_STB_PIN;
}

void tm1638_sendDataByte(uint8_t data) {
    for (uint8_t i = 0; i<8; i++) {
        TM1638_PORT.OUTCLR = TM1638_CLK_PIN;
        if (data & 0x01) {
            TM1638_PORT.OUTSET = TM1638_DIO_PIN;
        }
        else {
            TM1638_PORT.OUTCLR = TM1638_DIO_PIN;
        }
        TM1638_PORT.OUTSET = TM1638_CLK_PIN;
        data = data >> 1;
    }
}

void tm1638_sendCmd(uint8_t cmd) {
    TM1638_PORT.OUTCLR = TM1638_STB_PIN;
    tm1638_sendDataByte(cmd);
    TM1638_PORT.OUTSET = TM1638_STB_PIN;
}

void tm1638_startDataFrame(uint8_t address) {
    TM1638_PORT.OUTCLR = TM1638_STB_PIN;
    tm1638_sendDataByte(TM1638_CMD_SET_ADDRESS | (address & 0x0F) );
}

void tm1638_endDataFrame(void) {
    TM1638_PORT.OUTSET = TM1638_STB_PIN;
}

/**
 * @brief maps an uint to a 7segment pattern
 */
uint8_t tm1638_to7Seg(uint8_t val) {
    switch (val & 0x0F) {
        case 0x00: return 0x3F;
        case 0x01: return 0x06;
        case 0x02: return 0x5B;
        case 0x03: return 0x4F;
        case 0x04: return 0x66;
        case 0x05: return 0x6D;
        case 0x06: return 0x7D;
        case 0x07: return 0x07;
        case 0x08: return 0x7F;
        case 0x09: return 0x67;
        case 0x0a: return 0x77;
        case 0x0b: return 0x7C;
        case 0x0c: return 0x39;
        case 0x0d: return 0x5E;
        case 0x0e: return 0x79;
        case 0x0f: return 0x71;
        default: return 0;
    }
}



