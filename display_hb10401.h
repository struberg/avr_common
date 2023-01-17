#ifndef DISPLAY_HB10401_H
#define DISPLAY_HB10401_H

/**
 * @file display_hb10401.h
 * @author Mark Struberg (struberg@apache.org)
 * @brief Non-blocking library to control a HB10401 LCD 4 lines, 10 char each
 * @version 0.2
 * @date 2022-02-27
 * 
 * @copyright Copyright (c) 2022, Mark Struberg
 * @license Apache License v2.0
 * 
 * This library allows to control the HB10401 display.
 * This is a 4 row, 10 character display from Pollin and other resellers.
 * It originally is from a Siemens Logo 12/24RC SPS.
 * 
 * The code is optimised for low latency and has a 10 character buffer.
 * Displaying a character minimally takes 40uS for the LCD controller.
 * Thus writing 10 characters would keep us busy for half a ms with mostly idle loops.
 * To avoid such a situation the public api only writes to a buffer.
 * From the buffer one char at a time is transferred to the controller in a task worker.
 * 
 */

#include "strub_common.h"

#include <avr/io.h>
#include "pt.h"

#define LCD_DATA_PORT PORTC // LCD data pins
#define LCD_D4      PIN3_bm
#define LCD_D5      PIN2_bm
#define LCD_D6      PIN1_bm
#define LCD_D7      PIN0_bm

#define LCD_EN_PORT PORTB
#define LCD_EN      PIN0_bm // LCD Enable pin

#define LCD_RS_PORT PORTB
#define LCD_RS      PIN1_bm // LCD Register Select 0..Command, 1..Data


// we don't read ;) set fixed to GND == write #define LCD_RW      PB6 // Read/!Write

#define LCD_ADDR_LINE1  0x00
#define LCD_ADDR_LINE2  0x40
#define LCD_ADDR_LINE3  0x0A
#define LCD_ADDR_LINE4  0x4A

#define LCD_COMMAND_CLEAR       0x01
#define LCD_COMMAND_HOME        0x02
#define LCD_COMMAND_ENTRYMODE   0x04 // + bit1=right, bit0=shift
#define LCD_COMMAND_DISPONOFF   0x08 // + bit2=entire display_on, bit1=cursor_on, bit0=blink_on
#define LCD_COMMAND_DISPSHIFT   0x10 // + bit3=SC, bit2=RL
#define LCD_COMMAND_FNSET       0x20 
#define LCD_COMMAND_4BIT        0x20 // identical to fnset 5x8, 1 line but is actually an 8-bit command
#define LCD_COMMAND_SET_CGRAM   0x40 // + 6 bit address
#define LCD_COMMAND_SET_DDRAM   0x80 // + 7 bit address

static const uint8_t lcdAddr[] = {LCD_ADDR_LINE1, LCD_ADDR_LINE2, LCD_ADDR_LINE3, LCD_ADDR_LINE4};

// **** LCD processing structure **** 

#define LCD_BUFFER_LEN 40
#define LCD_DISP_WAIT 480 // how many cycles an LCD command takes to execute, which is about 40µS


/**
 * @brief configures the LCD subsystem
 */
void setup_lcd(void);

PT_THREAD(task_lcd(void));


/**
 * @brief redraw the whole screen from the video buffer
 */
void lcd_repaint(void);

/**
 * @brief clear the whole video butter with blanks
 * Use lcd_repaint() to repaint the screen afterwards
 */
void lcd_clear(void);

/**
 * @brief write a text into the video buffer
 * 
 * @param pState 
 * @param row 0-based row
 * @param column 0-based column
 * @param pText zero terminated text to copy
 */
void lcd_print(uint8_t row, uint8_t column, char* pText);


#endif