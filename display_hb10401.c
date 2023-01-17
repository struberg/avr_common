#include "display_hb10401.h"

#include <stdbool.h>
#include <util/delay.h>

#include "pt.h"


struct lcd_print_state {
    struct pt pt;
    uint16_t timer;

    bool repaint;
    uint8_t repaintPos; 
    char videoBuffer[LCD_BUFFER_LEN];
};

static struct lcd_print_state tsLcd = {0,};


/**
 * @brief is did timer pass the given duration?
 * 
 * A single tic is about 2.5 uS. Should not be used for anything > 0x50
 * because of the overrun.
 * 
 * @param oldTimerVal 
 * @param duration 
 * @return bool true if time is not yet over
 */
bool isAfter(uint16_t oldTimerVal, uint16_t duration) {
    uint16_t tNow = TASK_TIMER.CNT;
    uint16_t max = oldTimerVal + duration;
    if (max >= TASK_TIMER_OVERFLOW) {
        max = max - TASK_TIMER_OVERFLOW;
        // 0xE3 + 0x20 -> 0x03 because overflow
        return tNow > max && tNow < oldTimerVal;
    } else {
        // 0x83 + 0x20 -> 0xA3 no overflow
        return tNow > max;
    }
}


void lcd_sendNibble(uint8_t upperNibble) {
    uint8_t setBits = 0;
    uint8_t clearBits = 0;
    LCD_EN_PORT.OUTSET =LCD_EN;

    if (upperNibble & 0x80) {
        setBits |= LCD_D7;
    }
    else {
        clearBits |= LCD_D7;
    }

    if (upperNibble & 0x40) {
        setBits |= LCD_D6;
    }
    else {
        clearBits |= LCD_D6;
    }

    if (upperNibble & 0x20) {
        setBits |= LCD_D5;
    }
    else {
        clearBits |= LCD_D5;
    }

    if (upperNibble & 0x10) {
        setBits |= LCD_D4;
    }
    else {
        clearBits |= LCD_D4;
    }
    LCD_DATA_PORT.OUTSET = setBits;
    LCD_DATA_PORT.OUTCLR = clearBits;

    LCD_EN_PORT.OUTCLR = LCD_EN;
}


void lcd_command(uint8_t cmd) {
    LCD_RS_PORT.OUTCLR = LCD_RS; //RS=0 means instruction

    lcd_sendNibble(cmd);
    lcd_sendNibble(cmd << 4);
}

void lcd_data(uint8_t data) {
    LCD_RS_PORT.OUTSET = LCD_RS; //RS=1 means data

    lcd_sendNibble(data);
    lcd_sendNibble(data << 4);
}

/**
 * @brief set the cursor position to the given position on the screen
 * 
 * @param row 0-based row number from 0..3 
 * @param column 0based column number from 0..9
 */
void lcd_setPosition(uint8_t row, uint8_t column) {
    lcd_command(LCD_COMMAND_SET_DDRAM | (lcdAddr[row] + column));
}

void setup_lcd(void) {
    // display needs some time to start working.
    _delay_ms(20);
  
    // clear pins
    LCD_RS_PORT.OUTCLR = LCD_RS;
    LCD_EN_PORT.OUTCLR = LCD_EN;
    LCD_DATA_PORT.OUTCLR = LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;

    // EN, RS, RW and data pins as output
    LCD_RS_PORT.DIRSET = LCD_RS;
    LCD_EN_PORT.DIRSET = LCD_EN;
    LCD_DATA_PORT.DIRSET = LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;

    // clear the screen buffer
    for (int i=0; i < LCD_BUFFER_LEN; i++) {
        tsLcd.videoBuffer[i] = ' ';
    }

    // magic reset
    lcd_sendNibble(0x03); 
    _delay_ms(6);
    lcd_sendNibble(0x03); 
    _delay_ms(1);
    lcd_sendNibble(0x03); 
    _delay_ms(1);


    // enable 4 bit mode
    LCD_RS_PORT.OUTCLR = LCD_RS; //RS=0 means instruction

    // enable 4bit mode, this itself is a 8-bit command
    lcd_sendNibble(LCD_COMMAND_4BIT); 
    _delay_us(50);

    // and now swittch to 2line with 5x8 mode
    lcd_command(LCD_COMMAND_FNSET | 0x08);
    _delay_us(37);

    lcd_command(LCD_COMMAND_DISPONOFF | 0x04); // display on, but no cursor nor blinking    
    _delay_us(37);
    
    lcd_command(LCD_COMMAND_CLEAR);
    _delay_ms(50);

    lcd_command(LCD_COMMAND_SET_DDRAM | LCD_ADDR_LINE1);
    _delay_us(37);

    lcd_setPosition(0,0);
    _delay_us(37);
    //X lcd_command(LCD_COMMAND_ENTRYMODE | 0x02);
}

/**
 * @brief redraw the whole screen from the video buffer
 */
void lcd_repaint(void) {
    tsLcd.repaint = true; // start repainting
}

/**
 * @brief clear the whole video butter with blanks
 * Use lcd_repaint() to repaint the screen afterwards
 */
void lcd_clear(void) {
    for (int i=0; i < LCD_BUFFER_LEN; i++) {
        tsLcd.videoBuffer[i] = ' ';
    }
}


/**
 * @brief write a text into the video buffer
 * Use lcd_repaint() to repaint the screen afterwards
 * 
 * @param pState 
 * @param row 0-based row
 * @param column 0-based column
 * @param pText zero terminated text to copy
 */
void lcd_print(uint8_t row, uint8_t column, char* pText) {
    uint8_t bufPos = (row * 10) + column; // signed int to protect overflow
    
    int i=0;
    while (bufPos < LCD_BUFFER_LEN && pText[i] != 0) {
        tsLcd.videoBuffer[bufPos++] = pText[i++];
    }
}

PT_THREAD(task_lcd(void)) 
{
    PT_BEGIN(&tsLcd.pt);

    if (tsLcd.repaint) {
        tsLcd.repaintPos = 0;
        tsLcd.repaint=false;

        lcd_setPosition(0, 0);
        tsLcd.timer = TASK_TIMER.CNT;
        PT_YIELD_UNTIL(&tsLcd.pt, isAfter(tsLcd.timer, LCD_DISP_WAIT));

        while (tsLcd.repaintPos < LCD_BUFFER_LEN) {
            lcd_data(tsLcd.videoBuffer[tsLcd.repaintPos]);
            tsLcd.timer = TASK_TIMER.CNT;
            PT_YIELD_UNTIL(&tsLcd.pt, isAfter(tsLcd.timer, LCD_DISP_WAIT));

            tsLcd.repaintPos++;

            if (tsLcd.repaintPos == 10) {
                lcd_setPosition(1, 0);
                tsLcd.timer = TASK_TIMER.CNT;
                PT_YIELD_UNTIL(&tsLcd.pt, isAfter(tsLcd.timer, LCD_DISP_WAIT));
            }
            else if (tsLcd.repaintPos == 20) {
                lcd_setPosition(2, 0);
                tsLcd.timer = TASK_TIMER.CNT;
                PT_YIELD_UNTIL(&tsLcd.pt, isAfter(tsLcd.timer, LCD_DISP_WAIT));
            }
            else if (tsLcd.repaintPos == 30) {
                lcd_setPosition(3, 0);
                tsLcd.timer = TASK_TIMER.CNT;
                PT_YIELD_UNTIL(&tsLcd.pt, isAfter(tsLcd.timer, LCD_DISP_WAIT));
            }
        }
    }
    
    PT_END(&tsLcd.pt);
}

