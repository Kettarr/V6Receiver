/*
 * Copyright (c) 2001 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
/**
 * @file
 * video decoding with libavcodec API example
 *
 * @example decode_video.c
 */

#pragma once

/*
#ifndef INCLUDE_STB_IMAGE_WRITE_H
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif*/




#include <iostream>
#include <stb_image_write.h>
#include "ReceiveThread.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
}




/*
void write_png(const char* filename, AVFrame* frame) {
    int width = frame->width;
    int height = frame->height;
    int channels = 4; 

    // Allocate buffer for image data
    unsigned char* imageData = new unsigned char[width * height * channels];

    // Convert AVFrame data to image data
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int frameIndex = y * frame->linesize[0] + x * channels;
            int imageIndex = y * width * channels + x * channels;

            imageData[imageIndex + 0] = frame->data[0][frameIndex + 0]; // Red
            imageData[imageIndex + 1] = frame->data[0][frameIndex + 1]; // Green
            imageData[imageIndex + 2] = frame->data[0][frameIndex + 2]; // Blue
            imageData[imageIndex + 3] = frame->data[0][frameIndex + 3]; // Alpha
        }
    }

    // Write image data to PNG file
    stbi_write_png(filename, width, height, channels, imageData, width * channels);

    // Clean up
    delete[] imageData;
}*/


 


 

       
    
  


   

 



