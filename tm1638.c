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


static volatile PORT_t* tm1638_port;
static uint8_t tm1638_stbPin;
static uint8_t tm1638_clkPin;
static uint8_t tm1638_dioPin;

void tm1638_init(volatile PORT_t* port, uint8_t stbPin, uint8_t clkPin, uint8_t dioPin) {
    tm1638_port = port;
    tm1638_stbPin = stbPin;
    tm1638_clkPin = clkPin;
    tm1638_dioPin = dioPin;

    // switch the pins to output mode
    tm1638_port->DIRSET = (1<<tm1638_clkPin) | (1<<tm1638_stbPin) | (1<<tm1638_dioPin);

    tm1638_port->OUTSET = (1<<tm1638_stbPin); //passive mode STB high
    tm1638_port->OUTCLR = (1<<tm1638_clkPin) | (1<<tm1638_dioPin); // clk starts with low and data 0

    tm1638_sendCmd(TM1638_CMD_SET_BRIGHTNESS | 0xB);
    tm1638_sendCmd(TM1638_CMD_AUTO_INCREMENT);

    // clear all data registers
    tm1638_port->OUTCLR = (1<<tm1638_stbPin);
    tm1638_sendDataByte(TM1638_CMD_SET_ADDRESS | 0x0);
    for (uint8_t i=0; i<16; i++) {
        tm1638_sendDataByte(0x80);
    }
    tm1638_port->OUTSET = (1<<tm1638_stbPin);
}

void tm1638_sendDataByte(uint8_t data) {
    for (uint8_t i = 0; i<8; i++) {
        tm1638_port->OUTCLR = (1<<tm1638_clkPin);
        if (data & 0x01) {
            tm1638_port->OUTSET = (1<<tm1638_dioPin);
        }
        else {
            tm1638_port->OUTCLR = (1<<tm1638_dioPin);
        }
        tm1638_port->OUTSET = (1<<tm1638_clkPin);
        data = data >> 1;
    }
}

void tm1638_sendCmd(uint8_t cmd) {
    tm1638_port->OUTCLR = (1<<tm1638_stbPin);
    tm1638_sendDataByte(cmd);
    tm1638_port->OUTSET = (1<<tm1638_stbPin);
}

void tm1638_startDataFrame(uint8_t address) {
    tm1638_port->OUTCLR = (1<<tm1638_stbPin);
    tm1638_sendDataByte(TM1638_CMD_SET_ADDRESS | (address & 0x0F) );
}

void tm1638_endDataFrame(void) {
    tm1638_port->OUTSET = (1<<tm1638_stbPin);
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



