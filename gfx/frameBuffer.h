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
#ifndef __FRAMEBUFFER_H__
    #define __FRAMEBUFFER_H__

#include <inttypes.h>
#include <stdbool.h>

/**
 * @brief Meta information about a frame buffer
 * The FrameBuffer is organixes as row oriented byte array.
 * That means the first byte represents the first 8 pixels in row 0.
 * The MSB represents the pixel 0 in row 0.
 * It only supports monochrom displays for now.
 */
typedef struct {
    uint8_t width;      // width in pixels
    uint8_t widthBytes; // width in bytes
    uint8_t heigth;     // heigth in pixels
    uint8_t bufferLen;  // the length of the buffer in bytes
    uint8_t* buffer;
} FrameBuffer;

/**
 * @brief clear the framebuffer by setting all zeros
 * 
 * @param pFrameBuffer 
 */
void framebuffer_clear(FrameBuffer* pFrameBuffer);

/**
 * @brief Set or clear a pixel in the framebuffer
 * 
 * @param pFrameBuffer 
 * @param xPos 
 * @param yPos 
 * @param setClr true=set, false = clear pixel
 */
void framebuffer_setPixel(FrameBuffer* pFrameBuffer, uint8_t xPos, uint8_t yPos, bool setClr);

/**
 * @brief detect whether the pixel on the position is set or not
 * 
 * @param pFrameBuffer 
 * @param xPos 
 * @param yPos 
 * @return true 
 * @return false 
 */
bool framebuffer_getPixel(FrameBuffer* pFrameBuffer, uint8_t xPos, uint8_t yPos);

/**
 * @brief draw a horizontal line
 * 
 * @param pFrameBuffer 
 * @param xPosStart 
 * @param yPos 
 * @param xPosEnd must be higher than xPosStart 
 * @param setClr true=set, false = clear pixel
 */
void framebuffer_hline(FrameBuffer* pFrameBuffer, int8_t xPosStart, int8_t yPos, int8_t xPosEnd, bool setClr);

/**
 * @brief draw a vertical line
 * 
 * @param pFrameBuffer 
 * @param xPos 
 * @param yPosStart 
 * @param yPosEnd must be higher than yPosStart
 * @param setClr true=set, false = clear pixel
 */
void framebuffer_vline(FrameBuffer* pFrameBuffer, int8_t xPos, int8_t yPosStart, int8_t yPosEnd, bool setClr);

#endif