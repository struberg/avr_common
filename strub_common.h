#ifndef __STRUB_COMMON_H__
    #define __STRUB_COMMON_H__


#include <avr/io.h>

// we run at 10 MHz, so counting to5k gives us 0.5 ms or 2000 task ticks per second.
#ifndef TASK_TIMER_OVERFLOW
  #define TASK_TIMER_OVERFLOW 5000
#endif

#ifndef TASK_TIMER
  #define TASK_TIMER TCB0
#endif

#define set_bit(register, bit)  register |= (1 << bit)
#define clear_bit(register, bit)  register &= ~(1 << bit)


// bit-leiste um zu merken ob der task in dem zyklus schon abgearbeitet wurde
// wird vom timer overflow auf FF gesetzt
extern volatile uint8_t taskTriggered; 


char toBcd(uint8_t val);

/**
 * @brief Convert this to a hexadecimal string
 * 
 * @param value 
 * @param buffer minimum 3 bytes
 */
void toHexString_8(uint8_t value, char* buffer);

/**
 * @brief Convert this to a hexadecimal string
 * 
 * @param value 
 * @param buffer minimum 5 bytes
 */
void toHexString_16(uint16_t value, char* buffer);

/**
 * @brief Convert this to a hexadecimal string
 * 
 * @param value 
 * @param buffer minimum 9 bytes
 */
void toHexString_32(uint32_t value, char* buffer);

/**
 * @brief Convert this to a decimal string
 * 
 * @param value 
 * @param buffer minimum 4 bytes
 */
void toBcdString_8(uint8_t value, char* buffer);

/**
 * @brief Convert this to a decimal string
 * 
 * @param value 
 * @param buffer minimum 6 bytes
 */
void toBcdString_16(uint16_t value, char* buffer);

/**
 * @brief Convert this to a decimal string
 * 
 * @param value 
 * @param buffer minimum 11 bytes
 */
void toBcdString_32(uint32_t value, char* buffer);

#ifdef DEBUG_ENABLED 
  #define DEBUG ser_out((uint8_t) __LINE__ & 0x00ff)

#else
  #define DEBUG
#endif

/**
 * @brief Output something to a 74HC164 shift register with 8 LEDs
 * 
 * @param val 
 */
void ser_out(uint8_t val);


#endif