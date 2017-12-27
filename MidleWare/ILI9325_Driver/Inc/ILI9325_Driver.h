
/**
 ******************************************************************************
* File Name          : main.c
* Description        : Main program body
******************************************************************************
** This notice applies to any and all portions of this file
* that are not between comment pairs USER CODE BEGIN and
* USER CODE END. Other portions of this file, whether 
* inserted by the user or by software development tools
* are owned by their respective copyright owners.
*
* COPYRIGHT(c) 2017 STMicroelectronics
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/
#ifndef __ILI9325_DRIVER_H
#define __ILI9325_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define ILI9325_PIXELWIDTH      240
#define ILI9325_PIXELHEIGHT     320

#define ILI9325_RD_PIN    GPIOA_Bits->P0
#define ILI9325_RST_PIN   GPIOC_Bits->P1
#define ILI9325_WR_PIN    GPIOA_Bits->P1
#define ILI9325_RS_PIN    GPIOA_Bits->P4
#define ILI9325_CS_PIN    GPIOB_Bits->P0

#define ILI9325_D0_PIN    GPIOA_Bits->P9
#define ILI9325_D1_PIN    GPIOC_Bits->P7
#define ILI9325_D2_PIN    GPIOA_Bits->P10
#define ILI9325_D3_PIN    GPIOB_Bits->P3
#define ILI9325_D4_PIN    GPIOB_Bits->P5
#define ILI9325_D5_PIN    GPIOB_Bits->P4
#define ILI9325_D6_PIN    GPIOB_Bits->P10
#define ILI9325_D7_PIN    GPIOA_Bits->P8

// Register names from Peter Barrett's Microtouch code
#define ILI9325_START_OSC          0x0000
#define ILI9325_DRIV_OUT_CTRL      0x0001
#define ILI9325_DRIV_WAV_CTRL      0x0002
#define ILI9325_ENTRY_MOD          0x0003
#define ILI9325_RESIZE_CTRL        0x0004
#define ILI9325_DISP_CTRL1         0x0007
#define ILI9325_DISP_CTRL2         0x0008
#define ILI9325_DISP_CTRL3         0x0009
#define ILI9325_DISP_CTRL4         0x000A
#define ILI9325_RGB_DISP_IF_CTRL1  0x000C
#define ILI9325_FRM_MARKER_POS     0x000D
#define ILI9325_RGB_DISP_IF_CTRL2  0x000F
#define ILI9325_POW_CTRL1          0x0010
#define ILI9325_POW_CTRL2          0x0011
#define ILI9325_POW_CTRL3          0x0012
#define ILI9325_POW_CTRL4          0x0013
#define ILI9325_GRAM_HOR_AD        0x0020
#define ILI9325_GRAM_VER_AD        0x0021
#define ILI9325_RW_GRAM            0x0022
#define ILI9325_POW_CTRL7          0x0029
#define ILI9325_FRM_RATE_COL_CTRL  0x002B
#define ILI9325_GAMMA_CTRL1        0x0030
#define ILI9325_GAMMA_CTRL2        0x0031
#define ILI9325_GAMMA_CTRL3        0x0032
#define ILI9325_GAMMA_CTRL4        0x0035
#define ILI9325_GAMMA_CTRL5        0x0036
#define ILI9325_GAMMA_CTRL6        0x0037
#define ILI9325_GAMMA_CTRL7        0x0038
#define ILI9325_GAMMA_CTRL8        0x0039
#define ILI9325_GAMMA_CTRL9        0x003C
#define ILI9325_GAMMA_CTRL10       0x003D
#define ILI9325_HOR_START_AD       0x0050
#define ILI9325_HOR_END_AD         0x0051
#define ILI9325_VER_START_AD       0x0052
#define ILI9325_VER_END_AD         0x0053
#define ILI9325_GATE_SCAN_CTRL1    0x0060
#define ILI9325_GATE_SCAN_CTRL2    0x0061
#define ILI9325_GATE_SCAN_CTRL3    0x006A
#define ILI9325_PART_IMG1_DISP_POS 0x0080
#define ILI9325_PART_IMG1_START_AD 0x0081
#define ILI9325_PART_IMG1_END_AD   0x0082
#define ILI9325_PART_IMG2_DISP_POS 0x0083
#define ILI9325_PART_IMG2_START_AD 0x0084
#define ILI9325_PART_IMG2_END_AD   0x0085
#define ILI9325_PANEL_IF_CTRL1     0x0090
#define ILI9325_PANEL_IF_CTRL2     0x0092
#define ILI9325_PANEL_IF_CTRL3     0x0093
#define ILI9325_PANEL_IF_CTRL4     0x0095
#define ILI9325_PANEL_IF_CTRL5     0x0097
#define ILI9325_PANEL_IF_CTRL6     0x0098

// Color definitions
#define ILI9325_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9325_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9325_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9325_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9325_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9325_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9325_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9325_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9325_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9325_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9325_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9325_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9325_RED         0xF800      /* 255,   0,   0 */
#define ILI9325_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9325_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9325_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9325_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9325_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9325_PINK        0xF81F

// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in ROM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.
void ILI9325_Init(void);

//------------ILI9325_DrawPixel------------
// Color the pixel at the given coordinates with the given color.
// Requires 13 bytes of transmission
// Input: x     horizontal position of the pixel, columns from the left edge
//               must be less than 128
//               0 is on the left, 126 is near the right
//        y     vertical position of the pixel, rows from the top edge
//               must be less than 160
//               159 is near the wires, 0 is the side opposite the wires
//        color 16-bit color, which can be produced by ILI9325_Color565()
// Output: none
void ILI9325_DrawPixel(int16_t x, int16_t y, uint16_t color);

//------------ILI9325_DrawFastVLine------------
// Draw a vertical line at the given coordinates with the given height and color.
// A vertical line is parallel to the longer side of the rectangular display
// Requires (11 + 2*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        h     vertical height of the line
//        color 16-bit color, which can be produced by ILI9325_Color565()
// Output: none
void ILI9325_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

//------------ILI9325_DrawFastHLine------------
// Draw a horizontal line at the given coordinates with the given width and color.
// A horizontal line is parallel to the shorter side of the rectangular display
// Requires (11 + 2*w) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        w     horizontal width of the line
//        color 16-bit color, which can be produced by ILI9325_Color565()
// Output: none
void ILI9325_DrawFastHLine(int16_t x, int16_t y, int16_t length,uint16_t color);

//------------ILI9325_FillScreen------------
// Fill the screen with the given color.
// Requires 40,971 bytes of transmission
// Input: color 16-bit color, which can be produced by ILI9325_Color565()
// Output: none
void ILI9325_FillScreen(uint16_t color);

//------------ILI9325_FillRect------------
// Draw a filled rectangle at the given coordinates with the given width, height, and color.
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the top left corner of the rectangle, columns from the left edge
//        y     vertical position of the top left corner of the rectangle, rows from the top edge
//        w     horizontal width of the rectangle
//        h     vertical height of the rectangle
//        color 16-bit color, which can be produced by ILI9325_Color565()
// Output: none
void ILI9325_FillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, uint16_t color);

void ILI9325_WriteLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color);

void ILI9325_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color);
// Draw a circle outline
void ILI9325_DrawCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color);
//------------ILI9325_Color565------------
// Pass 8-bit (each) R,G,B and get back 16-bit packed color.
// Input: r red value
//        g green value
//        b blue value
// Output: 16-bit color
uint16_t ILI9325_Color565(uint8_t r, uint8_t g, uint8_t b);

//------------ILI9325_SwapColor------------
// Swaps the red and blue values of the given 16-bit packed color;
// green is unchanged.
// Input: x 16-bit color in format B, G, R
// Output: 16-bit color in format R, G, B
uint16_t ILI9325_SwapColor(uint16_t x);

//------------ILI9325_DrawBitmap------------
// Displays a 16-bit color BMP image.  A bitmap file that is created
// by a PC image processing program has a header and may be padded
// with dummy columns so the data have four byte alignment.  This
// function assumes that all of that has been stripped out, and the
// array image[] has one 16-bit halfword for each pixel to be
// displayed on the screen (encoded in reverse order, which is
// standard for bitmap files).  An array can be created in this
// format from a 24-bit-per-pixel .bmp file using the associated
// converter program.
// (x,y) is the screen location of the lower left corner of BMP image
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the bottom left corner of the image, columns from the left edge
//        y     vertical position of the bottom left corner of the image, rows from the top edge
//        image pointer to a 16-bit color BMP image
//        w     number of pixels wide
//        h     number of pixels tall
// Output: none
// Must be less than or equal to 128 pixels wide by 160 pixels high
void ILI9325_DrawBitmap(int16_t x, int16_t y, const uint16_t *image, int16_t w, int16_t h);

//------------ILI9325_DrawChar------------
// Advanced character draw function.  This is similar to the function
// from Adafruit_GFX.c but adapted for this processor.  However, this
// function only uses one call to setAddrWindow(), which allows it to
// run at least twice as fast.
// Requires (11 + size*size*6*8) bytes of transmission (assuming image fully on screen)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character
//        bgColor   16-bit color of the background
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none
void ILI9325_DrawChar(int16_t  x, int16_t  y, int8_t  c, uint16_t  color, uint8_t  size);

//------------ILI9325_DrawString------------
// String draw function.  
// 16 rows (0 to 15) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
// Input: x         columns from the left edge (0 to 20)
//        y         rows from the top edge (0 to 15)
//        pt        pointer to a null terminated string to be printed
//        textColor 16-bit color of the characters
// bgColor is Black and size is 1
// Output: number of characters printed
void ILI9325_DrawString(int16_t  x, int16_t  y, int8_t  *c, uint16_t  color, uint8_t  size);

//-----------------------fillmessage-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void fillmessage(uint32_t n);

//------------ILI9325_SetRotation------------
// Change the image rotation.
// Requires 2 bytes of transmission
// Input: m new rotation value (0 to 3)
// Output: none
void ILI9325_SetRotation(uint8_t x);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/