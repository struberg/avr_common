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

/**
 * @brief get next short from the pseudo random generator 
 * 
 * @return uint16_t next random number
 */
uint16_t nextRandom(void);


#ifdef DEBUG_ENABLED 
  #ifndef SER_PORT
    #define SER_PORT PORTB
  #endif
  #ifndef SER_CLK
    #define SER_CLK PIN1_bm
  #endif
  #ifndef SER_DATA
    #define SER_DATA PIN0_bm
  #endif

  //X #define DEBUGLN ser_out((uint8_t) __LINE__ & 0x00ff);
  #define SER_OUT(val) ser_out(&SER_PORT, SER_CLK, SER_DATA, val);

#else
  //X #define DEBUGLN
  #define SER_OUT(dummy)
#endif


/**
 * @brief Output something to a 74HC164 shift register with 8 LEDs
 * 
 * @param val 
 */
void ser_out(volatile PORT_t* port, uint8_t clkPin, uint8_t dataPin, uint8_t val);


#endif