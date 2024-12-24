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
#include <stddef.h>
#include <avr/io.h>
#include "button.h"
#include "strub_common.h"


/**
 * @brief function pointer to the buttonPressed method
 * 
 * Example:
 * void buttonPressed(uint8_t buttons) {
 *      switch (buttons) {
 *          case 0x01:
 *               currentBank++;
 *               if (currentBank > 0x0D) {
 *                  currentBank = 0x0A;
 *               }
 *               l7seg = currentBank;
 *               break;
 *           case 0x02:
 *               currentBank--;
 *               if (currentBank < 0x0A) {
 *                   currentBank = 0x0D;
 *               }
 *               l7seg = currentBank;
 *               break;
 *           case 0x04:
 *               menuMode = LED_SELECT;
 *               currentLed = 0;
 *               determineLedStatus();
 *               break;
 *       }
 * 
 */
void (*_cbButtonPressed)(uint8_t);
uint8_t _debounceTime;


void setButtonCallback(void (*callback)(uint8_t), uint8_t debounceTime) {
    _cbButtonPressed = callback;
    _debounceTime = debounceTime;
}



void buttonsCheck(uint8_t currentButtons) {
    static uint8_t lastButtons = 0;
    static uint8_t btnClearedCounter = 0;

    if (currentButtons > 0) {
        btnClearedCounter = 0;
        lastButtons |= currentButtons;
    }
    else if (lastButtons != 0) {
        if (btnClearedCounter < _debounceTime) {
            // de-bouncing
            btnClearedCounter++;
        }
        else {
            if (_cbButtonPressed != NULL) {
                (*_cbButtonPressed)(lastButtons);
            }
            lastButtons = 0;
            btnClearedCounter = 0;
        }
    }
}