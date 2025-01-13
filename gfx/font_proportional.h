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
#ifndef __FONT_FIXED_5X8_H__
    #define __FONT_FIXED_5X8_H__

#include "tile_8x8.h"

/**
 * @brief load the character into the given tile structure
 * 
 * @param character to load
 * @param pTile pointer to a Tile to load into
 */
void fontp_loadCharTile(uint8_t character, Tile* pTile);

/**
 * @brief Check, whether the pixels of the firstChar and secondChar bump into each other
 * 
 * In other words: tell us whether there is no space between them and we
 * need to draw an empty pixel between them.
 * 
 * @param pFirstChar 
 * @param pSecondChar 
 * @return true if a pixel of the first char is directly beside a pixel of the second char 
 * @return false 
 */
bool fontp_collide(Tile* pFirstChar, Tile* pSecondChar);

#endif