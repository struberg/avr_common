#ifndef BUTTON_H
#define BUTTON_H



typedef struct Button_t {
    // for de-bouncing
    uint8_t onCount;
} Button;


/**
 * @brief Set the Button Callback.
 * 
 * This registers the given function pointer as callback which gets invoked
 * whenever a button press is encountered.
 * 
 * @param callback 
 */
void setButtonCallback(void (*callback)(uint8_t));

/**
 * @brief Set the up buttons IO
 * 
 * Needs to be called once at startup
 */
void setup_buttons(void (*callback)(uint8_t));

/**
 * @brief check for a button press. This should only get called once every TASK tick
 */
void buttonsCheck(uint8_t currentButtons);

#endif