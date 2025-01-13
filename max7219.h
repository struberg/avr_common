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
#ifndef __MAX_7219_H__
    #define __MAX_7219_H__

/**
 * @brief library for the MAX7219 8x8 LED matrix driver
 * @author Mark Struberg (struberg@apache.org)
 * 
 * This version is optimised for the multi-8x8 LED matrix version.
 * I use bit-banging instead of hw SPI to be more flexible with the IO pins to use.
 * There is not that much of a speed difference at those low speeds and the MAX7219
 * doesn't use a fully SPI compat interface anyway.
 */

#include <avr/io.h>

#include "gfx/frameBuffer.h"


#ifndef MAX7219_CLK_PORT
    #define MAX7219_CLK_PORT PORTA
#endif
#ifndef MAX7219_CLK_PIN
    #define MAX7219_CLK_PIN PIN3
#endif

#ifndef MAX7219_DATA_PORT
    #define MAX7219_DATA_PORT PORTA
#endif
#ifndef MAX7219_DATA_PIN
    #define MAX7219_DATA_PIN PIN2
#endif

#ifndef MAX7219_CS_PORT
    #define MAX7219_CS_PORT PORTA
#endif
#ifndef MAX7219_CS_PIN
    #define MAX7219_CS_PIN PIN1
#endif

#define MAX7219_CMD_NO_OP   0x00
#define MAX7219_CMD_DIGIT_0 0x01
#define MAX7219_CMD_DIGIT_1 0x02
#define MAX7219_CMD_DIGIT_2 0x03
#define MAX7219_CMD_DIGIT_3 0x04
#define MAX7219_CMD_DIGIT_4 0x05
#define MAX7219_CMD_DIGIT_5 0x06
#define MAX7219_CMD_DIGIT_6 0x07
#define MAX7219_CMD_DIGIT_7 0x08
#define MAX7219_CMD_DECODE_MODE 0x09
#define MAX7219_CMD_INTENSITY   0x0A
#define MAX7219_CMD_SCAN_LIMIT  0x0B
#define MAX7219_CMD_DISPLAY     0x0C
#define MAX7219_CMD_DISP_TEST   0x0F

#define MAX7219_DECODE_MODE_NONE    0x00
#define MAX7219_DECODE_MODE_CODEB0  0x01
#define MAX7219_DECODE_MODE_CODEB4  0x0F
#define MAX7219_DECODE_MODE_CODEB7  0xFF

#define MAX7219_DISPLAY_MODE_ON     0x01
#define MAX7219_DISPLAY_MODE_OFF    0x00

#define MAX
/**
 * @brief initialises the n 8x8 MAX7219 modules
 * 
 * Those modules have to be connected in 'series'
 * DOUT->DIN of the next module
 * 
 * @param numberModules how many MAX7219 modules are chained up
 */
void max7219_init(uint8_t numberModules);

/**
 * @brief Start sending display data
 * 
 * The data is sent via subsequent calls to #max7219_sendData
 * 
 * After all data is sent call #max7219_endDataFrame
 */
void max7219_startDataFrame(void);

/**
 * @brief Send a 16bit data structure
 * 
 * Since multiple 8x8 matrix are daisy chained
 * we shift out  all of the data while CS is low.
 * The data will be activated in the MAX7219s when
 * CS gets high again.
 * This happens via #max7219_endDataFrame()
 * 
 * @param cmd the command
 * @param data the actual data information
 */
void max7219_sendData(uint8_t cmd, uint8_t data);
void max7219_endDataFrame(void);

/**
 * @brief render the display data in the buffer
 * 
 * The buffer must at least be of the size 8* #numberModules
 * The buffer is organised in rows.
 * The topleft pixel is the MSB of the first byte.
 * The first byte is the first row of the topleft matrix.
 * 
 * @param pBuffer pointer to the display buffer
 * @param numberModules number of 8x8 MAX7219 display modules
 */
void max7219_renderData(FrameBuffer* pBuffer, uint8_t numberModules);


#endif