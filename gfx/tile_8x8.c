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

#include "tile_8x8.h"

#include <avr/pgmspace.h>

#define SET_LED PORTB.OUTSET = PIN3_bm;
#define CLR_LED PORTB.OUTCLR = PIN3_bm;



void tile_place(FrameBuffer* pFrameBuffer, int8_t xPos, int8_t yPos, Tile* pTile, bool full) {
    uint8_t width = ((pTile->size & 0x70) >> 4) + 1;
    uint8_t heigth = (pTile->size & 0x07) + 1;

    if ((xPos < 0 && xPos + width < 0) || (yPos < 0 && yPos + heigth < 0)) {
        // done, not visible
    }
    uint8_t tileStartX = xPos < 0 ? xPos * -1 : 0;
    uint8_t tileStartY = yPos < 0 ? yPos * -1 : 0;

    uint8_t screenY = yPos;

    for (uint8_t tileY = tileStartY; tileY < heigth && screenY < pFrameBuffer->heigth; tileY++) {
        // row by row
        uint8_t screenX = xPos;
        for (uint8_t tileX = tileStartX; tileX < width && screenX < pFrameBuffer->width; tileX++) {
            // pixels per row
            if (pTile->bytes[tileY] & (0x80 >> tileX)) {
                framebuffer_setPixel(pFrameBuffer, screenX, screenY, true);
            }
            else if (full) {
                framebuffer_setPixel(pFrameBuffer, screenX, screenY, false);
            }
            screenX++;
        }
        screenY++;
    }
}

void tile_erase(FrameBuffer* pFrameBuffer, int8_t xPos, int8_t yPos, Tile* pTile) {
    uint8_t width = ((pTile->size & 0x70) >> 4) + 1;
    uint8_t heigth = (pTile->size & 0x07) + 1;
    
    if ((xPos < 0 && xPos + width < 0) || (yPos < 0 && yPos + heigth < 0)) {
        // done, not visible
    }
    uint8_t tileStartX = xPos < 0 ? xPos * -1 : 0;
    uint8_t tileStartY = yPos < 0 ? yPos * -1 : 0;

    uint8_t screenY = yPos;

    for (uint8_t tileY = tileStartY; tileY < heigth && screenY < pFrameBuffer->heigth; tileY++) {
        // row by row
        uint8_t screenX = xPos;
        for (uint8_t tileX = tileStartX; tileX < width && screenX < pFrameBuffer->width; tileX++) {
            // pixels per row
            if (pTile->bytes[tileY] & (0x80 >> tileX)) {
                framebuffer_setPixel(pFrameBuffer, screenX, screenY, false);
            }
            screenX++;
        }
        screenY++;
    }
}

uint8_t tile_getWidth(Tile* pTile){
    return ((pTile->size & 0x70) >> 4) + 1;
}

uint8_t tile_getHeigth(Tile* pTile){
    return (pTile->size & 0x07) + 1;
}


void tile_loadFromProgMem(const Tile* pTileProgMem, Tile* pTileTarget) {
    uint8_t* pBytes = (uint8_t*) pTileProgMem;
    pTileTarget->size = pgm_read_byte(pBytes);

    pBytes++; // now on to the bytes
    for (int i=0; i<8; i++) {
        pTileTarget->bytes[i] = pgm_read_byte(pBytes + i);
    }
}