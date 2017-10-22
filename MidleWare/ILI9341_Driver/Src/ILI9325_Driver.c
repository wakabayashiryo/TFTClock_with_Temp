#include "ILI9325_Driver.h"
#include "font.h"

static uint8_t Rotation;           // 0 to 3
static int16_t _width = ILI9325_PIXELWIDTH;   // this could probably be a constant, except it is used in Adafruit_GFX and depends on image rotation
static int16_t _height = ILI9325_PIXELHEIGHT;

typedef union
{
    uint8_t base;
    struct
    {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
    };
}BitField_t;


static inline void writebus(uint8_t data)
{
    BitField_t busdata;
    busdata.base = data;
    
    ILI9325_D0_PIN = busdata.bit0;
    ILI9325_D1_PIN = busdata.bit1;
    ILI9325_D2_PIN = busdata.bit2;
    ILI9325_D3_PIN = busdata.bit3;
    ILI9325_D4_PIN = busdata.bit4;
    ILI9325_D5_PIN = busdata.bit5;
    ILI9325_D6_PIN = busdata.bit6;
    ILI9325_D7_PIN = busdata.bit7;
}

static void writecommand(uint16_t command)
{
    ILI9325_CS_PIN = 0;
    ILI9325_RS_PIN = 0;
 
    writebus(command>>8);
    ILI9325_WR_PIN = 0;
    ILI9325_WR_PIN = 1;
 
    writebus(command);
    ILI9325_WR_PIN = 0;
    ILI9325_WR_PIN = 1;
 
    ILI9325_CS_PIN = 1;
}

static void writedata(uint16_t data)
{
    ILI9325_CS_PIN = 0;
    ILI9325_RS_PIN = 1;
 
    writebus(data>>8);
    ILI9325_WR_PIN = 0;
    ILI9325_WR_PIN = 1;
 
    writebus(data);
    ILI9325_WR_PIN = 0;
    ILI9325_WR_PIN = 1;
 
    ILI9325_CS_PIN = 1;
}

static void writeregister(uint16_t reg_val, uint16_t data_val)
{
    writecommand(reg_val);
    writedata(data_val);
}

// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in ROM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.

void ILI9325_Init(void)
{
    ILI9325_RST_PIN = 0;
    HAL_Delay(100);
    ILI9325_RST_PIN = 1;
    HAL_Delay(50);

    writeregister(0x00E5, 0x78F0);
    writeregister(0x0001, 0x0100);
    writeregister(0x0002, 0x0700);
    writeregister(0x0003, 0x1030);
    writeregister(0x0004, 0x0000);
    writeregister(0x0008, 0x0202);
    writeregister(0x0009, 0x0000);
    writeregister(0x000A, 0x0000);
    writeregister(0x000C, 0x0000); //0x0001
    writeregister(0x000D, 0x0000);
    writeregister(0x000F, 0x0000);
    
    writeregister(0x0010, 0x0000);
    writeregister(0x0011, 0x0007);
    writeregister(0x0012, 0x0000);
    writeregister(0x0013, 0x0000);
    writeregister(0x0007, 0x0001);
    HAL_Delay(50);
    
    writeregister(0x0010, 0x1090); //0x1690
    writeregister(0x0011, 0x0227);
    HAL_Delay(50);
    
    writeregister(0x0012, 0x001F); //0x009D
    HAL_Delay(50);
    
    writeregister(0x0013, 0x1500); //0x1900
    writeregister(0x0029, 0x0027); //0x0025
    writeregister(0x002B, 0x000D);
    HAL_Delay(50);
    
    writeregister(0x0020, 0x0000);
    writeregister(0x0021, 0x0000);
    HAL_Delay(50);
    
    writeregister(0x0030, 0x0000); //0x0007
    writeregister(0x0031, 0x0707); //0x0303
    writeregister(0x0032, 0x0307); //0x0003
    writeregister(0x0035, 0x0200); //0x0206
    writeregister(0x0036, 0x0008);
    writeregister(0x0037, 0x0004); //0x0406
    writeregister(0x0038, 0x0000); //0x0304
    writeregister(0x0039, 0x0707); //0x0007
    writeregister(0x003C, 0x0002); //0x0602
    writeregister(0x003D, 0x1D04); //0x0008
    HAL_Delay(50);
    
    writeregister(0x0050, 0x0000);
    writeregister(0x0051, 0x00EF);
    writeregister(0x0052, 0x0000);
    writeregister(0x0053, 0x013F);
    writeregister(0x0060, 0xA700);
    writeregister(0x0061, 0x0001);
    writeregister(0x006A, 0x0000);
    
    writeregister(0x0080, 0x0000);
    writeregister(0x0081, 0x0000);
    writeregister(0x0082, 0x0000);
    writeregister(0x0083, 0x0000);
    writeregister(0x0084, 0x0000);
    writeregister(0x0085, 0x0000);
    
    writeregister(0x0090, 0x0010);
    writeregister(0x0092, 0x0600);
    writeregister(0x0007, 0x0133);

    HAL_Delay(100);
    
    ILI9325_FillScreen(ILI9325_BLACK);
}

// Set the region of the screen RAM to be modified
// Pixel colors are sent left to right, top to bottom
// (same as font table is encoded; different from regular bitmap)
// Requires 11 bytes of transmission
static void setAddrWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) 
{
    int x, y, t;

    switch(Rotation) 
    {
        default:
            x  = x1;
            y  = y1;
        break;
        case 1:
            t  = y1;
            y1 = x1;
            x1 = _width  - 1 - y2;
            y2 = x2;
            x2 = _width  - 1 - t;
            x  = x2;
            y  = y1;
        break;
        case 2:
            t  = x1;
            x1 = _width  - 1 - x2;
            x2 = _width  - 1 - t;
            t  = y1;
            y1 = _height - 1 - y2;
            y2 = _height - 1 - t;
            x  = x2;
            y  = y2;
        break;
        case 3:
            t  = x1;
            x1 = y1;
            y1 = _height - 1 - x2;
            x2 = y2;
            y2 = _height - 1 - t;
            x  = x1;
            y  = y2;
        break;
    }

    writeregister(0x0050, x1); // Set address window
    writeregister(0x0051, x2);
    writeregister(0x0052, y1);
    writeregister(0x0053, y2);
    writeregister(0x0020, x ); // Set address counter to top left
    writeregister(0x0021, y );
}
// Send two bytes of data, most significant byte first
// Requires 2 bytes of transmission
static void pushColor(uint16_t color) 
{
    writeregister(0x0022, color);
}


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
void ILI9325_DrawPixel(int16_t x, int16_t y, uint16_t color) 
{

  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

//  setAddrWindow(x,y,x+1,y+1); // original code, bug???
  setAddrWindow(x,y,x,y);

  pushColor(color);
}


//------------ILI9325_DrawFastVLine------------
// Draw a vertical line at the given coordinates with the given height and color.
// A vertical line is parallel to the longer side of the rectangular display
// Requires (11 + 2*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        h     vertical height of the line
//        color 16-bit color, which can be produced by ILI9325_Color565()
// Output: none
void ILI9325_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) 
{
  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((y+h-1) >= _height) h = _height-y;
  setAddrWindow(x, y, x, y+h-1);

  while (h--)writedata(color);
}


//------------ILI9325_DrawFastHLine------------
// Draw a horizontal line at the given coordinates with the given width and color.
// A horizontal line is parallel to the shorter side of the rectangular display
// Requires (11 + 2*w) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        w     horizontal width of the line
//        color 16-bit color, which can be produced by ILI9325_Color565()
// Output: none
void ILI9325_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) 
{
  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  setAddrWindow(x, y, x+w-1, y);

  while (w--)writedata(color);
}


//------------ILI9325_FillScreen------------
// Fill the screen with the given color.
// Requires 40,971 bytes of transmission
// Input: color 16-bit color, which can be produced by ILI9325_Color565()
// Output: none
void ILI9325_FillScreen(uint16_t color)
{
  ILI9325_FillRect(0, 0, _width, _height, color);  // original
//  screen is actually 129 by 161 pixels, x 0 to 128, y goes from 0 to 160
}


//------------ILI9325_FillRect------------
// Draw a filled rectangle at the given coordinates with the given width, height, and color.
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the top left corner of the rectangle, columns from the left edge
//        y     vertical position of the top left corner of the rectangle, rows from the top edge
//        w     horizontal width of the rectangle
//        h     vertical height of the rectangle
//        color 16-bit color, which can be produced by ILI9325_Color565()
// Output: none
void ILI9325_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  setAddrWindow(x, y, x+w-1, y+h-1);

  for(y=h; y>0; y--) 
  {
    for(x=w; x>0; x--) 
    {
        writedata(color);
    }
  }
}


//------------ILI9325_Color565------------
// Pass 8-bit (each) R,G,B and get back 16-bit packed color.
// Input: r red value
//        g green value
//        b blue value
// Output: 16-bit color
uint16_t ILI9325_Color565(uint8_t r, uint8_t g, uint8_t b) 
{
  return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
}


//------------ILI9325_SwapColor------------
// Swaps the red and blue values of the given 16-bit packed color;
// green is unchanged.
// Input: x 16-bit color in format B, G, R
// Output: 16-bit color in format R, G, B
uint16_t ILI9325_SwapColor(uint16_t x) 
{
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}


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
void ILI9325_DrawBitmap(int16_t x, int16_t y, const uint16_t *image, int16_t w, int16_t h)
{
  int16_t skipC = 0;                      // non-zero if columns need to be skipped due to clipping
  int16_t originalWidth = w;              // save this value; even if not all columns fit on the screen, the image is still this width in ROM
  int i = w*(h - 1);

  if((x >= _width) || ((y - h + 1) >= _height) || ((x + w) <= 0) || (y < 0))
  {
    return;                             // image is totally off the screen, do nothing
  }
  if((w > _width) || (h > _height))
  {    // image is too wide for the screen, do nothing
    //***This isn't necessarily a fatal error, but it makes the
    //following logic much more complicated, since you can have
    //an image that exceeds multiple boundaries and needs to be
    //clipped on more than one side.
    return;
  }
  if((x + w - 1) >= _width)
  {            // image exceeds right of screen
    skipC = (x + w) - _width;           // skip cut off columns
    w = _width - x;
  }
  if((y - h + 1) < 0)
  {                  // image exceeds top of screen
    i = i - (h - y - 1)*originalWidth;  // skip the last cut off rows
    h = y + 1;
  }
  if(x < 0)
  {                            // image exceeds left of screen
    w = w + x;
    skipC = -1*x;                       // skip cut off columns
    i = i - x;                          // skip the first cut off columns
    x = 0;
  }
  if(y >= _height)
  {                     // image exceeds bottom of screen
    h = h - (y - _height + 1);
    y = _height - 1;
  }

  setAddrWindow(x, y-h+1, x+w-1, y);

  for(y=0; y<h; y=y+1)
  {
    for(x=0; x<w; x=x+1)
    {
      writedata(image[i]);
      i = i + 1;                        // go to the next pixel
    }
    i = i + skipC;
    i = i - 2*originalWidth;
  }
}
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
void ILI9325_DrawChar(uint8_t  x, uint8_t  y, int8_t  c, uint16_t  color, uint8_t  size)
{
    uint8_t  i, j;

    uint8_t  letter = c < 0x52 ? c - 0x20 : c - 0x52;
    for (i =0; i<5; i++ ) 
    {
        uint8_t  line = c < 0x52 ? Alpha1[letter*5+i] : Alpha2[letter*5+i];

        for (j = 0; j<8; j++) 
        {
            if (line & 0x1)
            {
                if (size == 1) // default size
                    ILI9325_DrawPixel(x+i, y+j, color);
                else // big size
                    ILI9325_FillRect(x+i*size, y+j*size, size, size, color);
            }
            line >>= 1;
        }
    }
}

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
void ILI9325_DrawString(uint8_t  x, uint8_t  y, int8_t  *c, uint16_t  color, uint8_t  size)
{
    while (c[0] != 0) 
    {
        ILI9325_DrawChar(x, y, c[0], color, size);
        x += size*6;
        c++;
        if (x + 5 >= _width) 
        {
            y += 10;
            x = 0;
        }
    }
}
//-----------------------fillmessage-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
char Message[12];
uint32_t Messageindex;

void fillmessage(uint32_t n)
{
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10)
  {
    fillmessage(n/10);
    n = n%10;
  }
  Message[Messageindex] = (n+'0'); /* n is between 0 and 9 */
  if(Messageindex<11)Messageindex++;
}

//------------ILI9325_SetRotation------------
// Change the image rotation.
// Requires 2 bytes of transmission
// Input: m new rotation value (0 to 3)
// Output: none
void ILI9325_SetRotation(uint8_t m) 
{
    Rotation = m % 4;
    uint16_t t;

    switch(Rotation) 
    {
        default: t = 0x1030; break;
        case 1 : t = 0x1028; break;
        case 2 : t = 0x1000; break;
        case 3 : t = 0x1018; break;
    }

    writeregister(0x0003, t ); // MADCTL
    // For 932X, init default full-screen address window:
    setAddrWindow(0, 0, _width - 1, _height - 1); // CS_IDLE happens here
}
