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
#ifndef __TILE_8X8_H__
    #define __TILE_8X8_H__

#include <inttypes.h>
#include <stdbool.h>

#include "frameBuffer.h"


/**
 * @brief a single tile of maximum 8x8.
 * 
 * An instance of a tile with a maximum of 8x8.
 * 
 */
typedef struct {
    /**
     * @brief The size and kind of the tile
     * bits     7 6 5 4 3 2 1 0
     *          T W W W X H H H
     * 
     * T.. 0=tile; 1=stamp
     * W.. 3 bit width-1 7=width8, 0=width1
     * X.. unused 
     * H.. 3 bit heigth-1 7=heigth8, 0=heigth1
     */
    uint8_t size;

    /**
     * @brief the font data, row by row
     * First byte is line 0, last byte is line 7.
     * The MSB represents the first pixel, LSB is the rightmost pixel.
     */
    uint8_t bytes[8];
} Tile;


/**
 * @brief place the tile into the frame buffer on the given location
 * 
 * @param pFrameBuffer frame buffer
 * @param xPos 
 * @param yPos 
 * @param pTile 
 * @param full if true we will also clear non-set pixels, if false we will only set pixels
 */
void tile_place(FrameBuffer* pFrameBuffer, int8_t xPos, int8_t yPos, Tile* pTile, bool full);

/**
 * @brief erase the tile from the frame buffer on the given location
 * 
 * That means any pixel of this location will be set to off;
 * 
 * @param pFrameBuffer frame buffer
 * @param xPos 
 * @param yPos 
 * @param pTile 
 */
void tile_erase(FrameBuffer* pFrameBuffer, int8_t xPos, int8_t yPos, Tile* pTile);

/**
 * @return uint8_t the width of the given tile
 */
uint8_t tile_getWidth(Tile* pTile);

/**
 * @return uint8_t the heigth of the given tile
 */
uint8_t tile_getHeigth(Tile* pTile);

/**
 * @brief load a tile from the given address in the program memory
 * 
 * @param pTileProgMem 
 * @param pTileTarget 
 */
void tile_loadFromProgMem(const Tile* pTileProgMem, Tile* pTileTarget);


#endif
