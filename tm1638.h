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
#ifndef __TM_1638_H__
    #define __TM_1638_H__

/**
 * @file TM1638.h
 * @author Mark Struberg (struberg@apache.org)
 * @brief Library to control a TM1638 based 8x7seg board.
 * @version 0.1
 * @date 2024-12-23
 * 
 * @copyright Copyright (c) 2024
 * 
 * The display I do use is the LED&KKEY module
 * with 8 7-segment displays, 8 LEDs, 8 keys
 * 
 * The 7-seg is connected as following
 *      ** 01 **
 *      20    02
 *      ** 40 **
 *      10    04
 *      ** 08 ** 80
 * 
 * The 7-segment display is on even addresses, starting with 0x00.
 * 
 * The 7 LEDs are bit 0x01 on odd addresses 0x01, 0x03, etc
 */

#include <avr/io.h>
#include <stdbool.h>


#ifndef TM1638_PORT
    #define TM1638_PORT PORTA
#endif

#ifndef TM1638_STB_PI
    // high -> passive, low -> chip select active
    #define TM1638_STB_PIN PIN4
#endif
#ifndef TM1638_CLK_PIN
    // data output is set with rising edge, data read is on falling edge
    #define TM1638_CLK_PIN PIN5
#endif
#ifndef TM1638_DIO_PIN
    #define TM1638_DIO_PIN PIN6
#endif

#define TM1638_CMD_READ_KEYS      0x42
#define TM1638_CMD_AUTO_INCREMENT 0x40
#define TM1638_CMD_FIXED_ADDRESS  0x44


// OR this with the address in the lower nibble
#define TM1638_CMD_SET_ADDRESS    0xC0

// OR this with the brightness in the lower nibble
// Note that bit 3 (0x08) is the on/off bit!
// Thus effectively the brightness is from 8..F
#define TM1638_CMD_SET_BRIGHTNESS 0x80

/**
 * @brief initialise the tm1638 connection
 * This method has to be invoked before any other tm1638 function call.
 */
void tm1638_init(void);

/**
 * @brief Send a command
 */
void tm1638_sendCmd(uint8_t cmd);

/**
 * @brief Start sending display data
 * 
 * The data is sent via subsequent calls to #tm1638_sendDataByte
 * 
 * After all data is sent call #tm1638_endDataFrame
 * 
 * @param address the starting address 
 */
void tm1638_startDataFrame(uint8_t address);
void tm1638_sendDataByte(uint8_t data);
void tm1638_endDataFrame(void);

/**
 * @brief map a numeric value from 0..F to it's 7-segment pattern
 */
uint8_t tm1638_to7Seg(uint8_t val);




#endif