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
#ifndef __DISPLAY_SSD1306_H__
    #define __DISPLAY_SSD1306_H__

#include <stdint.h>

#define SSD1306_CONTROL_BYTE_ONE_COMMAND               0x00
#define SSD1306_CONTROL_BYTE_MULTIPLE_COMMANDS         0x80
#define SSD1306_CONTROL_BYTE_ONE_DATA                  0x40
#define SSD1306_CONTROL_BYTE_MULTIPLE_DATA             0xC0

#define SSD1306_SETCONTRAST                            0x81
#define SSD1306_DISPLAYALLON_RESUME                    0xA4
#define SSD1306_DISPLAYALLON                           0xA5
#define SSD1306_NORMALDISPLAY                          0xA6
#define SSD1306_INVERTDISPLAY                          0xA7
#define SSD1306_DISPLAYOFF                             0xAE
#define SSD1306_DISPLAYON                              0xAF
#define SSD1306_SETDISPLAYOFFSET                       0xD3
#define SSD1306_SETCOMPINS                             0xDA
#define SSD1306_SETVCOMDETECT                          0xDB
#define SSD1306_SETDISPLAYCLOCKDIV                     0xD5
#define SSD1306_SETPRECHARGE                           0xD9
#define SSD1306_SETMULTIPLEX                           0xA8
#define SSD1306_SETLOWCOLUMN                           0x00
#define SSD1306_SETHIGHCOLUMN                          0x10
#define SSD1306_SETSTARTLINE                           0x40 // will require some | [A5:A0]
#define SSD1306_MEMORYMODE                             0x20
#define SSD1306_COLUMNADDR                             0x21
#define SSD1306_PAGEADDR                               0x22
#define SSD1306_COMSCANINC                             0xC0
#define SSD1306_COMSCANDEC                             0xC8
#define SSD1306_SEGREMAP                               0xA0
#define SSD1306_SEGREMAPINV                            0xA1
#define SSD1306_CHARGEPUMP                             0x8D
#define SSD1306_SWITCHCAPVCC                           0x02
#define SSD1306_NOP                                    0xE3


/**
 * @brief initialize the I2C and the SSD1306 controller
 * 
 * @return uint8_t 0 if ok
 */
uint8_t ssd1306_init(void);

/**
 * @brief send a single byte to the SSD1306 controller
 * 
 * @param data 
 */
void ssd1306_send_single_data(char data);

/**
 * @brief send multiple commands and data to the SSD1306 controller
 * 
 * @param length 
 * @param data 
 * @return uint8_t 
 */
uint8_t ssd1306_send_multiple_data(int length, char data[]);

uint8_t ssd1306_send_single_command(char command);
uint8_t ssd1306_send_multiple_commands(int length, char commands[]);


/**
 * @brief set the bank and column position for drawing pixels
 * 
 * @param bank 0-based bank 0-7, starting from top to bottom. A bank is a 8-bit high row. 
 *              LSB is the uppermost pixel.
 * @param column 0-based column from 0 to 128 from left to right. 
 * @return uint8_t 0 if ok
 */
uint8_t ssd1306_setBankColPos(char bank, char column);

/**
 * @brief clear the whole display by filling it with blanks.
 * 
 * @return uint8_t 0 if ok
 */
uint8_t ssd1306_clear_display(void);

/**
 * @brief set cursor position for text 8x5 pixel based output
 * 
 * @param row 0 based row 0-7
 * @param column of 6-pixel wide positions.
 */
void ssd1306_setCursorPos(uint8_t row, uint8_t column);

/**
 * @brief print text in a 8x5 pixel font
 * 
 * @param txt to print, zero terminated
 * @param maxLen maximum length if zero termination is missing or broken.
 */
void ssd1306_print(char* txt, uint8_t maxLen);

void ssd1306_printP(uint8_t row, uint8_t column, char* txt, uint8_t maxLen);

void ssd1306_printChar_largeP(uint8_t row, uint8_t column, unsigned char c);

/**
 * @brief print text in double size 8x5 pixel font
 * 
 * This results in 2 lines 16x11 font.
 * The row/col position is the lower line.
 * To print a large font skip the row 0 and position the cursor on row 1.
 * 
 * @param row 0-based row from 1..7
 * @param colunn 0-based 6-bit wide column
 * @param txt to print, zero terminated
 * @param maxLen maximum length if zero termination is missing or broken.
 */
void ssd1306_print_largeP(uint8_t row, uint8_t column, char* txt, uint8_t maxLen);

#endif