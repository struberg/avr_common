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
 * @param callback which gets called when a button click is detected
 * @param debounceTime for key debouncing
 */
void setButtonCallback(void (*callback)(uint8_t), uint8_t debounceTime);

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