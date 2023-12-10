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
void (*cbButtonPressed)(uint8_t);


void setButtonCallback(void (*callback)(uint8_t)) {
    cbButtonPressed = callback;
}



void buttonsCheck(uint8_t currentButtons) {
    static uint8_t lastButtons = 0;
    static uint8_t btnClearedCounter = 0;

    if (currentButtons > 0) {
        btnClearedCounter = 0;
        lastButtons |= currentButtons;
    }
    else if (lastButtons != 0) {
        if (btnClearedCounter < 0x2F) {
            // de-bouncing
            btnClearedCounter++;
        }
        else {
            if (cbButtonPressed != NULL) {
                (*cbButtonPressed)(lastButtons);
            }
            lastButtons = 0;
            btnClearedCounter = 0;
        }
    }
}