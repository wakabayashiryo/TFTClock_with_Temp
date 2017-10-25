#include "ILI9325_Driver.h"
#include "font.h"

static uint8_t Rotation = 0;           // 0 to 3
static int16_t _width = ILI9325_PIXELWIDTH;   // this could probably be a constant, except it is used in Adafruit_GFX and depends on image Rotation
static int16_t _height = ILI9325_PIXELHEIGHT;

#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }

#define WR_STROBO() ILI9325_WR_PIN = 0;ILI9325_WR_PIN = 1;

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


inline static void writebus(uint8_t data)
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

    WR_STROBO();
}

inline static void writecommand(uint16_t command)
{
    ILI9325_CS_PIN = 0;
    ILI9325_RS_PIN = 0;
 
    writebus(command>>8);
 
    writebus(command);
 
    ILI9325_CS_PIN = 1;
}

inline static void writedata(uint16_t data)
{
    ILI9325_CS_PIN = 0;
    ILI9325_RS_PIN = 1;
 
    writebus(data>>8);
 
    writebus(data);
 
    ILI9325_CS_PIN = 1;
}

inline static void writeregister(uint16_t reg_val, uint16_t data_val)
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
    HAL_Delay(200);
    
    ILI9325_RD_PIN = 1;//RD = 1;
    ILI9325_CS_PIN = 1;//CS = 1;             // CS High
    ILI9325_WR_PIN = 1;//WR = 1;             // WR High
    
    ILI9325_RST_PIN = 0;
    HAL_Delay(100);
    ILI9325_RST_PIN = 1;
    HAL_Delay(50);

    writeregister(0x00E3, 0x3008);    // Set internal timing
    writeregister(0x00E7, 0x0012);    // Set internal timing
    writeregister(0x00EF, 0x1231);    // Set internal timing
    writeregister(0x0001, 0x0000);    // set SS and SM bit
    writeregister(0x0002, 0x0700);    // set line inversion
    writeregister(0x0003, 0x1030);    // set GRAM write direction and BGR=1, 16bit color
    writeregister(0x0004, 0x0000);    // Resize register
    writeregister(0x0008, 0x0202);    // set the back porch and front porch
    writeregister(0x0009, 0x0000);    // set non-display area refresh cycle ISC[3:0]
    writeregister(0x000A, 0x0000);    // FMARK function
    writeregister(0x000C, 0x0000);    // RGB interface setting
    writeregister(0x000D, 0x0000);    // Frame marker Position
    writeregister(0x000F, 0x0000);    // RGB interface polarity
    /********* 電源オンシーケンス  **************/
    writeregister(0x0010, 0x0000);    // SAP, BT[3:0], AP[2:0], DSTB, SLP, STB
    writeregister(0x0011, 0x0007);    // DC1[2:0], DC0[2:0], VC[2:0]
    writeregister(0x0012, 0x0000);    // VREG1OUT voltage
    writeregister(0x0013, 0x0000);    // VDV[4:0] for VCOM amplitude
    HAL_Delay(200);          // コンデンサ放電待ち
    writeregister(0x0010, 0x1690);    // SAP, BT[3:0], AP[2:0], DSTB, SLP, STB
    writeregister(0x0011, 0x0227);    // R11h=0x0221 at VCI=3.3V ,DC1[2:0], DC0[2:0], VC[2:0]
    HAL_Delay(50);           // 遅延 50ms
    writeregister(0x0012, 0x001a);    // External reference voltage= Vci;
    HAL_Delay(50);           // 遅延 50ms
    writeregister(0x0013, 0x1800);    // R13=0F00 when R12=009E;VDV[4:0] for VCOM amplitude
    writeregister(0x0029, 0x002A);    // R29=0019 when R12=009E;VCM[5:0] for VCOMH//0012//
    writeregister(0x002B, 0x000D);    // Frame Rate = 91Hz
    HAL_Delay(50);           // 遅延 50ms
    writeregister(0x0020, 0x0000);    // GRAM horizontal Address
    writeregister(0x0021, 0x0000);    // GRAM Vertical Address
    /**********  ガンマ補正  **********/
    writeregister(0x0030, 0x0000);
    writeregister(0x0031, 0x0000);
    writeregister(0x0032, 0x0000);
    writeregister(0x0035, 0x0206);
    writeregister(0x0036, 0x0808);
    writeregister(0x0037, 0x0007);
    writeregister(0x0038, 0x0201);
    writeregister(0x0039, 0x0000);
    writeregister(0x003C, 0x0000);
    writeregister(0x003D, 0x0000);
    /*********** GRAM領域設定 *********/
    writeregister(0x0050, 0x0000);    // Horizontal GRAM Start Address
    writeregister(0x0051, 0x00EF);    // Horizontal GRAM End Address
    writeregister(0x0052, 0x0000);    // Vertical GRAM Start Address
    writeregister(0x0053, 0x013F);    // Vertical GRAM Start Address
    writeregister(0x0060, 0x2700);    // Gate Scan Line 横表示用 0x2700<描画反転>
    writeregister(0x0061, 0x0003);    // NDL,VLE, REV
    writeregister(0x006A, 0x0000);    // set scrolling line
    /************* 部分表示制御 ************/
    writeregister(0x0080, 0x0000);    // 部分表示1位置
    writeregister(0x0081, 0x0000);    // 部分表示1RAM開始アドレス
    writeregister(0x0082, 0x0000);    // 部分表示1RAM終了アドレス
    writeregister(0x0083, 0x0000);    // 部分表示2位置  
    writeregister(0x0084, 0x0000);    // 部分表示2RAN開始アドレス
    writeregister(0x0085, 0x0000);    // 部分表示2RAM終了アドレス
    /************** パネル制御 ************/
    writeregister(0x0090, 0x0010);    // 1ラインクロック数
    writeregister(0x0092, 0x0000);    // ゲートオーバーラップクロック数
    writeregister(0x0093, 0x0003);    // 出力タイミング
    writeregister(0x0095, 0x1100);    // RGBの1ラインクロック数
    writeregister(0x0097, 0x0000);    // 出力タイミング
    writeregister(0x0098, 0x0000);    // 出力タイミング
    /***** 表示制御 *****/ 
    writeregister(0x0007, 0x0133);    // 262K color and display ON

    ILI9325_SetRotation(Rotation);

    ILI9325_FillScreen(ILI9325_BLACK);
}

// Set the region of the screen RAM to be modified
// Pixel colors are sent left to right, top to bottom
// (same as font table is encoded; different from regular bitmap)
// Requires 11 bytes of transmission
static void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) 
{
    uint32_t x, y, t;
    
    switch(Rotation)
    {
     default:
      x  = x1;
      y  = y1;
      break;
     case 1:
      t  = y1;
      y1 = x1;
      x1 = ILI9325_PIXELWIDTH  - 1 - y2;
      y2 = x2;
      x2 = ILI9325_PIXELWIDTH  - 1 - t;
      x  = x2;
      y  = y1;
      break;
     case 2:
      t  = x1;
      x1 = ILI9325_PIXELWIDTH  - 1 - x2;
      x2 = ILI9325_PIXELWIDTH  - 1 - t;
      t  = y1;
      y1 = ILI9325_PIXELHEIGHT - 1 - y2;
      y2 = ILI9325_PIXELHEIGHT - 1 - t;
      x  = x2;
      y  = y2;
      break;
     case 3:
      t  = x1;
      x1 = y1;
      y1 = ILI9325_PIXELHEIGHT - 1 - x2;
      x2 = y2;
      y2 = ILI9325_PIXELHEIGHT - 1 - t;
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
inline static void pushColor(uint16_t color) 
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
    int16_t t;
    switch(Rotation) 
    {
        default:
        break;
     case 1:
      t = x;
      x = ILI9325_PIXELWIDTH  - 1 - y;
      y = t;
      break;
     case 2:
      x = ILI9325_PIXELWIDTH  - 1 - x;
      y = ILI9325_PIXELHEIGHT - 1 - y;
      break;
     case 3:
      t = x;
      x = y;
      y = ILI9325_PIXELHEIGHT - 1 - t;
      break;
    }

    writeregister(0x0020, x);
    writeregister(0x0021, y);
    writeregister(0x0022, color);
}

// Fast block fill operation for fillScreen, fillRect, H/V line, etc.
// Requires setAddrWindow() has previously been called to set the fill
// bounds.  'len' is inclusive, MUST be >= 1.
static void ILI9325_flood(uint16_t color, uint32_t len)
{
    // for(uint32_t i = 0;i<len;i++)
    // pushColor(color);

    uint16_t blocks;
    uint8_t  i, hi = color >> 8,
                lo = color;
             
    writecommand(0x0022);

    ILI9325_CS_PIN = 0;
    ILI9325_RS_PIN = 1;
    
    // Write first pixel normally, decrement counter by 1
    writebus(hi);
    writebus(lo);
    len--;
  
    blocks = (uint16_t)(len / 64); // 64 pixels/block
    if(hi == lo) 
    {
      // High and low bytes are identical.  Leave prior data
      // on the port(s) and just toggle the write strobe.
      while(blocks--) 
      {
        i = 16; // 64 pixels/block / 4 pixels/pass
        do 
        {    
            WR_STROBO();WR_STROBO();WR_STROBO();WR_STROBO();
            WR_STROBO();WR_STROBO();WR_STROBO();WR_STROBO();
        } while(--i);
      }
      // Fill any remaining pixels (1 to 64)
      for(i = (uint8_t)len & 63; i--; ) 
      {
        WR_STROBO();
        WR_STROBO();
      }
    } 
    else 
    {
      while(blocks--) 
      {
        i = 16; // 64 pixels/block / 4 pixels/pass
        do 
        {
            writebus(hi);writebus(lo);
            writebus(hi);writebus(lo);
            writebus(hi);writebus(lo);
            writebus(hi);writebus(lo);
        } while(--i);
      }
      for(i = (uint8_t)len & 63; i--;)
      {
        writebus(hi);
        writebus(lo);
      } 
    }
    ILI9325_CS_PIN = 1;    
  }
  
void ILI9325_DrawFastHLine(int16_t x, int16_t y, int16_t length,uint16_t color)
{
  int16_t x2;

  // Initial off-screen clipping
  if((length <= 0     ) ||
      (y      <  0     ) || ( y                  >= _height) ||
      (x      >= _width) || ((x2 = (x+length-1)) <  0      )) return;

  if(x < 0) {        // Clip left
    length += x;
    x       = 0;
  }
  if(x2 >= _width) { // Clip right
    x2      = _width - 1;
    length  = x2 - x + 1;
  }

  setAddrWindow(x, y, x2, y);
  ILI9325_flood(color, length);
  setAddrWindow(0, 0, _width - 1, _height - 1);
}

void ILI9325_DrawFastVLine(int16_t x, int16_t y, int16_t length,uint16_t color)
{
  int16_t y2;

  // Initial off-screen clipping
  if((length <= 0      ) ||
      (x      <  0      ) || ( x                  >= _width) ||
      (y      >= _height) || ((y2 = (y+length-1)) <  0     )) return;
  if(y < 0) {         // Clip top
    length += y;
    y       = 0;
  }
  if(y2 >= _height) { // Clip bottom
    y2      = _height - 1;
    length  = y2 - y + 1;
  }

  setAddrWindow(x, y, x, y2);
  ILI9325_flood(color, length);
  setAddrWindow(0, 0, _width - 1, _height - 1);
}

void ILI9325_FillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, 
    uint16_t fillcolor) {
  int16_t  x2, y2;

  // Initial off-screen clipping
  if( (w            <= 0     ) ||  (h             <= 0      ) ||
      (x1           >= _width) ||  (y1            >= _height) ||
      ((x2 = x1+w-1) <  0     ) || ((y2  = y1+h-1) <  0      )) return;
  if(x1 < 0) { // Clip left
    w += x1;
    x1 = 0;
  }
  if(y1 < 0) { // Clip top
    h += y1;
    y1 = 0;
  }
  if(x2 >= _width) { // Clip right
    x2 = _width - 1;
    w  = x2 - x1 + 1;
  }
  if(y2 >= _height) { // Clip bottom
    y2 = _height - 1;
    h  = y2 - y1 + 1;
  }

  setAddrWindow(x1, y1, x2, y2);
  ILI9325_flood(fillcolor, (uint32_t)w * (uint32_t)h);
  setAddrWindow(0, 0, _width - 1, _height - 1);
}

// Bresenham's algorithm - thx wikpedia
void ILI9325_WriteLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color) 
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) 
  {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) 
  {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) 
  {
    ystep = 1;
  } 
  else 
  {
    ystep = -1;
  }

  for (; x0<=x1; x0++) 
  {
    if (steep) 
    {
        ILI9325_DrawPixel(y0, x0, color);
    } 
    else 
    {
        ILI9325_DrawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) 
    {
        y0 += ystep;
        err += dx;
    }
  }
}

void ILI9325_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color)
{
// Update in subclasses if desired!
  if(x0 == x1)
  {
      if(y0 > y1) _swap_int16_t(y0, y1);
      ILI9325_DrawFastVLine(x0, y0, y1 - y0 + 1, color);
  }
  else if(y0 == y1)
  {
      if(x0 > x1) _swap_int16_t(x0, x1);
      ILI9325_DrawFastHLine(x0, y0, x1 - x0 + 1, color);
  }
  else
  {
      ILI9325_WriteLine(x0, y0, x1, y1, color);
  }
}

// Draw a circle outline
void ILI9325_DrawCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  ILI9325_DrawPixel(x0  , y0+r, color);
  ILI9325_DrawPixel(x0  , y0-r, color);
  ILI9325_DrawPixel(x0+r, y0  , color);
  ILI9325_DrawPixel(x0-r, y0  , color);

  while (x<y) 
  {
      if (f >= 0) 
      {
          y--;
          ddF_y += 2;
          f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x;

      ILI9325_DrawPixel(x0 + x, y0 + y, color);
      ILI9325_DrawPixel(x0 - x, y0 + y, color);
      ILI9325_DrawPixel(x0 + x, y0 - y, color);
      ILI9325_DrawPixel(x0 - x, y0 - y, color);
      ILI9325_DrawPixel(x0 + y, y0 + x, color);
      ILI9325_DrawPixel(x0 - y, y0 + x, color);
      ILI9325_DrawPixel(x0 + y, y0 - x, color);
      ILI9325_DrawPixel(x0 - y, y0 - x, color);
  }
}

void ILI9325_FillScreen(uint16_t color)
{
      
      // For the 932X, a full-screen address window is already the default
      // state, just need to set the address pointer to the top-left corner.
      // Although we could fill in any direction, the code uses the current
      // screen Rotation because some users find it disconcerting when a
      // fill does not occur top-to-bottom.
      uint16_t x, y;
      switch(Rotation)
       {
        default: x = 0            ; y = 0            ; break;
        case 1 : x = ILI9325_PIXELWIDTH  - 1; y = 0            ; break;
        case 2 : x = ILI9325_PIXELWIDTH  - 1; y = ILI9325_PIXELHEIGHT - 1; break;
        case 3 : x = 0            ; y = ILI9325_PIXELHEIGHT - 1; break;
      }
      writeregister(0x0020, x);
      writeregister(0x0021, y);
  
    ILI9325_flood(color, (long)ILI9325_PIXELWIDTH * (long)ILI9325_PIXELHEIGHT);
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
      pushColor(image[i]);
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
    uint16_t px = x,py = y;
    
    while (c[0] != 0) 
    {
        ILI9325_DrawChar(px, py, c[0], color, size);
        px += size*6;
        c++;
        if ((px >= _width)||(*c=='\n')) 
        {
            py += 10*size;
            px = x;
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
void ILI9325_SetRotation(uint8_t x)
{   
    Rotation = x & 0x03;
    uint16_t t;
    switch(Rotation) 
    {
        default: t = 0x1030; _width = ILI9325_PIXELWIDTH; _height = ILI9325_PIXELHEIGHT; break;
        case 1 : t = 0x1028; _width = ILI9325_PIXELHEIGHT; _height = ILI9325_PIXELWIDTH; break;
        case 2 : t = 0x1000; _width = ILI9325_PIXELWIDTH; _height = ILI9325_PIXELHEIGHT; break;
        case 3 : t = 0x1018; _width = ILI9325_PIXELHEIGHT; _height = ILI9325_PIXELWIDTH; break;
    }
    writeregister(0x0003, t ); // MADCTL
    // For 932X, init default full-screen address window:
    setAddrWindow(0, 0, ILI9325_PIXELWIDTH - 1, ILI9325_PIXELHEIGHT - 1); // CS_IDLE happens here
}    
