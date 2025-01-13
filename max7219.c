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

#include "max7219.h"


void max7219_init(uint8_t numberModules) {
    // set the ports to output
    MAX7219_CLK_PORT.DIRSET  = (1<<MAX7219_CLK_PIN);   
    MAX7219_DATA_PORT.DIRSET = (1<<MAX7219_DATA_PIN);   
    MAX7219_CS_PORT.DIRSET   = (1<<MAX7219_CS_PIN);

    // no decode mode
    max7219_startDataFrame();
    for (uint8_t i=0; i < numberModules; i++) {
        max7219_sendData(MAX7219_CMD_DECODE_MODE, MAX7219_DECODE_MODE_NONE);
    }
    max7219_endDataFrame();

    // disable scan limit
    max7219_startDataFrame();
    for (uint8_t i=0; i < numberModules; i++) {
        max7219_sendData(MAX7219_CMD_SCAN_LIMIT, 0x07);
    }
    max7219_endDataFrame();


    // send all zeros to the data register
    for (uint8_t digit=0; digit < 8; digit++) {
        max7219_startDataFrame();
        for (uint8_t i=0; i < numberModules; i++) {
            max7219_sendData(MAX7219_CMD_DIGIT_0 + digit, 0x00);
        }
        max7219_endDataFrame();
    }

    // switch display on
    max7219_startDataFrame();
    for (uint8_t i=0; i < numberModules; i++) {
        max7219_sendData(MAX7219_CMD_DISPLAY, MAX7219_DISPLAY_MODE_ON);
    }
    max7219_endDataFrame();

}

void max7219_startDataFrame() {
    MAX7219_CLK_PORT.OUTCLR  = (1<<MAX7219_CLK_PIN);
    MAX7219_DATA_PORT.OUTCLR = (1<<MAX7219_DATA_PIN);
    MAX7219_CS_PORT.OUTCLR   = (1<<MAX7219_CS_PIN);
}

void max7219_sendDataByte(uint8_t data) {
   for (uint8_t i = 0; i<8; i++) {
        MAX7219_CLK_PORT.OUTCLR = (1<<MAX7219_CLK_PIN);
        if (data & 0x80) {
            MAX7219_DATA_PORT.OUTSET = (1<<MAX7219_DATA_PIN);
        }
        else {
            MAX7219_DATA_PORT.OUTCLR = (1<<MAX7219_DATA_PIN);
        }
        MAX7219_CLK_PORT.OUTSET = (1<<MAX7219_CLK_PIN);
        data = data << 1;
    }
}

void max7219_sendData(uint8_t cmd, uint8_t data) {
    max7219_sendDataByte(cmd);
    max7219_sendDataByte(data);
}

void max7219_endDataFrame(void) {
    MAX7219_CS_PORT.OUTSET   = (1<<MAX7219_CS_PIN);
}


void max7219_renderData(FrameBuffer* pFrameBuffer, uint8_t numberModules) {
    // switch display off to avoid flickering effects
    max7219_startDataFrame();
    for (uint8_t i=0; i < numberModules; i++) {
        max7219_sendData(MAX7219_CMD_DISPLAY, MAX7219_DISPLAY_MODE_OFF);
    }
    max7219_endDataFrame();

    uint8_t* pBufferPos = pFrameBuffer->buffer;
    for (uint8_t row = 0; row < 8; row++) {
        max7219_startDataFrame();
        for (uint8_t module = 0; module < numberModules; module++) {
            max7219_sendData(MAX7219_CMD_DIGIT_0 + row, *pBufferPos++);
        }
        max7219_endDataFrame();
    }

    // switch display on again
    max7219_startDataFrame();
    for (uint8_t i=0; i < numberModules; i++) {
        max7219_sendData(MAX7219_CMD_DISPLAY, MAX7219_DISPLAY_MODE_ON);
    }
    max7219_endDataFrame();

}


