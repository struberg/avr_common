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

#include "frameBuffer.h"

void framebuffer_clear(FrameBuffer* pFrameBuffer) {
    for (uint16_t i = 0; i < pFrameBuffer->bufferLen; i++) {
        pFrameBuffer->buffer[i] = 0x00;
    }
}

void framebuffer_setPixel(FrameBuffer* pFrameBuffer, uint8_t xPos, uint8_t yPos, bool setClr) {
    if (yPos > pFrameBuffer->heigth || xPos > pFrameBuffer->width) {
        return;
    }

    uint8_t bytePos = yPos * pFrameBuffer->widthBytes + xPos/8;
    uint8_t bitPos = xPos % 8;
    if (setClr) {
        // set the pixel
        pFrameBuffer->buffer[bytePos] |= 0x80>>bitPos;
    }
    else {
        // clear the pixel
        pFrameBuffer->buffer[bytePos] &= ~(0x80>>bitPos);
    }
}

void framebuffer_hline(FrameBuffer* pFrameBuffer, int8_t xPosStart, int8_t yPos, int8_t xPosEnd, bool setClr) {
    if (yPos < 0 || yPos >= pFrameBuffer->heigth) {
        // not visible
        return;
    }

    int8_t screenXStart = xPosStart < 0 ? 0 : xPosStart;
    int8_t screenXEnd = xPosEnd >= pFrameBuffer->width ? pFrameBuffer->width - 1 : xPosEnd;
    
    for (uint8_t x = screenXStart; x <= screenXEnd; x++) {
        framebuffer_setPixel(pFrameBuffer, x, yPos, setClr);
    }
}

void framebuffer_vline(FrameBuffer* pFrameBuffer, int8_t xPos, int8_t yPosStart, int8_t yPosEnd, bool setClr) {
    if (xPos < 0 || xPos >= pFrameBuffer->width) {
        // not visible
        return;
    }

    int8_t screenYStart = yPosStart < 0 ? 0 : yPosStart;
    int8_t screenYEnd = yPosEnd >= pFrameBuffer->heigth ? pFrameBuffer->heigth - 1 : yPosEnd;
    
    for (uint8_t y = screenYStart; y <= screenYEnd; y++) {
        framebuffer_setPixel(pFrameBuffer, xPos, y, setClr);
    }
}
