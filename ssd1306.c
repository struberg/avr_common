#include "ssd1306.h"
#include "i2c.h"

#include <stdbool.h>

/************ SSD1306 START *************/

#include <avr/pgmspace.h>

// constant names from https://github.com/tibounise/SSD1306-AVR

#define SSD1306_I2C_ADDRESS                       0x3C// address is only 7 bits, last bit is R/!W

#define SSD1306_PAGESIZE                               8
#define SSD1306_WIDTH                                  128
#define SSD1306_HEIGHT                                 64
#define SSD1306_COLUMNSTART                            0
#define SSD1306_COLUMNEND                              127
#define SSD1306_PAGE_BUFFER_SIZE                       SSD1306_WIDTH*SSD1306_HEIGHT/8;

// ---------------------------------------

PROGMEM const static char init_options[] = {
    SSD1306_DISPLAYOFF,

    // timing
    SSD1306_SETDISPLAYCLOCKDIV, // default value for clock
    0x80,

    SSD1306_SETMULTIPLEX,
    0x3F, // 63 (64-1, default)

    SSD1306_SETDISPLAYOFFSET,
    0x00,

    // hardware config
    SSD1306_SETSTARTLINE, // 0x40 for line 0 (0x40 | 0x00, basically)

    SSD1306_COMSCANINC, // vertical direction

    // charge pump
    SSD1306_CHARGEPUMP,  // internal charge pump
    0x14,

    // address mode, etc.
    SSD1306_MEMORYMODE, // horizontal mode (0), vertial (1), page (2)
    0x00,

    SSD1306_SETCOMPINS, // default com pins, uxe 0x00 for 128x32 display
    0x12,

    SSD1306_SETCONTRAST, // default contrast
    0x3F,

    SSD1306_SETPRECHARGE, // default pre-charge period
    0xF1,

    SSD1306_SETVCOMDETECT, // default
    0x20,

    // fundamentals
    SSD1306_DISPLAYALLON_RESUME, // output follows ram content (default)
    SSD1306_NORMALDISPLAY, // SSD1306_INVERTDISPLAY or SSD1306_NORMALDISPLAY
    SSD1306_DISPLAYON
};

/* ASCII 5x8 font */
 PROGMEM const uint8_t ssd1306_font5x8 []=
 {
    0x00, 0x00, 0x00, 0x00, 0x00, // sp
    0x00, 0x00, 0x2f, 0x00, 0x00, // ! 00101111
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x14, 0x7f, 0x14, 0x7f, 0x14, // #
    0x24, 0x2a, 0x7f, 0x2a, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1c, 0x22, 0x41, 0x00, // (
    0x00, 0x41, 0x22, 0x1c, 0x00, // )
    0x14, 0x08, 0x3E, 0x08, 0x14, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x00, 0xA0, 0x60, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x08, 0x14, 0x22, 0x41, 0x00, // <
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x00, 0x41, 0x22, 0x14, 0x08, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x59, 0x51, 0x3E, // @ 
    0x7C, 0x12, 0x11, 0x12, 0x7C, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x09, 0x01, // F
    0x3E, 0x41, 0x49, 0x49, 0x7A, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x3F, 0x40, 0x38, 0x40, 0x3F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x07, 0x08, 0x70, 0x08, 0x07, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x7F, 0x41, 0x41, 0x00, // [
    0x02, 0x04, 0x08, 0x10, 0x20, // '\'
    0x00, 0x41, 0x41, 0x7F, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x04, 0x00, // '
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x18, 0xA4, 0xA4, 0xA4, 0x7C, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x40, 0x80, 0x84, 0x7D, 0x00, // j
    0x7F, 0x10, 0x28, 0x44, 0x00, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0xFC, 0x24, 0x24, 0x24, 0x18, // p
    0x18, 0x24, 0x24, 0x18, 0xFC, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x1C, 0xA0, 0xA0, 0xA0, 0x7C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x77, 0x00, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, // |
    0x00, 0x77, 0x08, 0x00, 0x00, // }
    0x10, 0x08, 0x10, 0x08, 0x00, // ~
    0xFF, 0x81, 0x81, 0x81, 0xFF, // block carret
 };

uint8_t ssd1306_send_single_command(char command) {
	i2c_start_write(SSD1306_I2C_ADDRESS);
	i2c_write_byte(SSD1306_CONTROL_BYTE_ONE_COMMAND);
	uint8_t nack = i2c_write_byte(command);
	i2c_stop();
    return nack;
}

uint8_t ssd1306_send_multiple_commands(int length, char commands[]) {
	if (length <= 0) return 0;

    uint8_t nack = 0;

	i2c_start_write(SSD1306_I2C_ADDRESS);
	for (int i = 0; i < length; i++) {
        // the last command doesn't indicate continuation:
        nack = i2c_write_byte(i == (length-1) ? SSD1306_CONTROL_BYTE_ONE_COMMAND : SSD1306_CONTROL_BYTE_MULTIPLE_COMMANDS);
		nack |= i2c_write_byte(commands[i]);
	}

	i2c_stop();

    return nack;
}


uint8_t ssd1306_setup_gdram(void) {
    uint8_t nack = 0;
    // setup width of the display
    char columnConfig[] = {SSD1306_COLUMNADDR, 0, SSD1306_WIDTH-1};
    nack |=  ssd1306_send_multiple_commands(sizeof(columnConfig), columnConfig);

    // also setup the number of pages -> the heigth
    char pageConfig[] = {SSD1306_PAGEADDR, 0, (SSD1306_HEIGHT/8)-1};
    nack |=  ssd1306_send_multiple_commands(sizeof(pageConfig), pageConfig);
    return nack;
}

uint8_t ssd1306_init(void) {
	i2c_start_write(SSD1306_I2C_ADDRESS);
    uint8_t nack = 0;

	for (uint8_t i = 0; i < sizeof(init_options); i++) {
        // set continuation bit only until last command
		nack |= i2c_write_byte(i == sizeof(init_options)-1 ? SSD1306_CONTROL_BYTE_ONE_COMMAND: SSD1306_CONTROL_BYTE_MULTIPLE_COMMANDS);
		nack |= i2c_write_byte(pgm_read_byte(init_options + i)); // progmem access
	}
	i2c_stop();

    return nack;
}

void ssd1306_send_single_data(char data) {
	i2c_start_write(SSD1306_I2C_ADDRESS);
	i2c_write_byte(SSD1306_CONTROL_BYTE_ONE_DATA);
	i2c_write_byte(data);
	i2c_stop(); 
}

uint8_t ssd1306_send_multiple_data(int length, char data[]) {
    if (length==0) {
        return 0;
    }

	i2c_start_write(SSD1306_I2C_ADDRESS);
	uint8_t nack = 0;
	for (int i = 0; i < length; i++) {
        nack |= i2c_write_byte(i == length-1 ? SSD1306_CONTROL_BYTE_ONE_DATA : SSD1306_CONTROL_BYTE_MULTIPLE_DATA);
		nack |= i2c_write_byte(data[i]);
	}
	i2c_stop();

    return nack;
}

uint8_t ssd1306_send_progmem_multiple_data(const int length, const char *data) {
    if (length==0) {
        return 0;
    }

	i2c_start_write(SSD1306_I2C_ADDRESS);
	uint8_t nack = 0;
	for (int i = 0; i < length; i++) {
        nack |= i2c_write_byte(i == length-1 ? SSD1306_CONTROL_BYTE_ONE_DATA : SSD1306_CONTROL_BYTE_MULTIPLE_DATA);
		nack |= i2c_write_byte(pgm_read_byte(data + i));
	}
	i2c_stop();
    return nack;
}

uint8_t ssd1306_setBankColPos(char bank, char column) {
    char commands[] = {SSD1306_PAGEADDR, bank, 7,
                       SSD1306_COLUMNADDR, column, 127 
    };

    return ssd1306_send_multiple_commands(sizeof(commands), commands);
}

uint8_t ssd1306_clear_display(void) {
    uint8_t nack = 0;
    char command[] = {SSD1306_PAGEADDR, 0, SSD1306_PAGESIZE-1, SSD1306_COLUMNADDR, SSD1306_COLUMNSTART, SSD1306_COLUMNEND};
    ssd1306_send_multiple_commands(sizeof(command), command);

	i2c_start_write(SSD1306_I2C_ADDRESS);
    uint16_t len = SSD1306_WIDTH * SSD1306_PAGESIZE;
    for (uint16_t i=0; i < len; i++) {
        nack |= i2c_write_byte(i == len-1 ? SSD1306_CONTROL_BYTE_ONE_DATA : SSD1306_CONTROL_BYTE_MULTIPLE_DATA);
		nack |= i2c_write_byte(0x00);
    }
    i2c_stop();
    return nack;
}

void ssd1306_printChar(unsigned char c) {
    if (c < 32 || c > 127) {
        c = 127; // the block carret
    }
    char data[6];
    data[0] = 0x00; // blank row between characters
    const uint8_t* font = ssd1306_font5x8 + (c-32)*5;
    for (int i=0; i<5; i++) {
        data[i+1] = pgm_read_byte(font + i);
    }
    ssd1306_send_multiple_data(6, data);
}


/**
 * @brief set the cursor position to the given position in 6x8 font grid
 * 
 * @param row 0-based row from 0..0
 * @param colunn 0-based column from 0..21
 */
void ssd1306_setCursorPos(uint8_t row, uint8_t column) {
    ssd1306_setBankColPos(row, SSD1306_COLUMNSTART + column*6);
}

void ssd1306_printP(uint8_t row, uint8_t column, char* txt, uint8_t maxLen) {
    ssd1306_setCursorPos(row, column);
    ssd1306_print(txt, maxLen);
}

void ssd1306_print(char* txt, uint8_t maxLen) {
    for (int i=0; i<maxLen; i++) {
        if (txt[i] == 0) {
            return;
        }
        ssd1306_printChar(txt[i]);
    }
}

void ssd1306_printChar_largeP(uint8_t row, uint8_t column, unsigned char c) {
    if (c < 32 || c > 127) {
        c = 127; // the block carret
    }

    // paint upper half bytes
    const uint8_t* font = ssd1306_font5x8 + (c-32)*5;
    char data[12];
    data[0] = 0x00; // blank row between characters
    data[11] = 0x00; // blank row between characters
    
    // paint upper half bytes
    ssd1306_setCursorPos(row-1, column);
    for (int i=0; i < 5; i++) {
        uint8_t fontCol = pgm_read_byte(font + i);

        uint8_t px = (fontCol & 0x08) << 4;
        px |= (fontCol & 0x08) << 3;
        px |= (fontCol & 0x04) << 3;
        px |= (fontCol & 0x04) << 2;
        px |= (fontCol & 0x02) << 2;
        px |= (fontCol & 0x02) << 1;
        px |= (fontCol & 0x01) << 1;

        data[(i<<1)+1]=px;
        data[(i<<1)+2]=px;
    }
    ssd1306_send_multiple_data(12, data);

    // paint lower half bytes
    ssd1306_setCursorPos(row, column);
    for (int i=0; i < 5; i++) {
        uint8_t fontCol = pgm_read_byte(font + i);

        uint8_t px = (fontCol & 0x80) >> 1;
        px |= (fontCol & 0x40) >> 1;
        px |= (fontCol & 0x40) >> 2;
        px |= (fontCol & 0x20) >> 2;
        px |= (fontCol & 0x20) >> 3;
        px |= (fontCol & 0x10) >> 3;
        px |= (fontCol & 0x10) >> 4;

        data[(i<<1)+1]=px;
        data[(i<<1)+2]=px;
    }
    ssd1306_send_multiple_data(12, data);
}

void ssd1306_print_largeP(uint8_t row, uint8_t column, char* txt, uint8_t maxLen) {
    if (row <1 || row > 7) {
        ssd1306_setCursorPos(0,0);
        ssd1306_print("Illegal row", 10);
        return;
    }

     for (int i=0; i<maxLen; i++) {
        if (txt[i] == 0) {
            return;
        }
        ssd1306_printChar_largeP(row, column + 2*i, txt[i]);
    }
}

/************ SSD1306 END *************/

