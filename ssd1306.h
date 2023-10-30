#ifndef __DISPLAY_SSD1306_H__
    #define __DISPLAY_SSD1306_H__

#include <stdint.h>


uint8_t ssd1306_init(void);

void ssd1306_send_single_data(char data);
uint8_t ssd1306_send_multiple_data(int length, char data[]);

uint8_t ssd1306_setBankColPos(char bank, char column);

uint8_t ssd1306_clear_display(void);
void ssd1306_setCursorPos(uint8_t row, uint8_t column);
void ssd1306_print(char* txt, uint8_t maxLen);

#endif