#ifndef __DISPLAY_SSD1306_H__
    #define __DISPLAY_SSD1306_H__

#include <stdint.h>

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

#endif