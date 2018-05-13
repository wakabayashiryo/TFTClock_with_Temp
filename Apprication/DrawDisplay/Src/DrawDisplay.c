#include "DrawDisplay.h"
#include "stdarg.h"
#include "stm32f4xx_hal_rtc.h"

static uint16_t BackColor = ILI9325_BLACK;
static uint16_t ProcessCount_Envir = PROCESSTIME_ENVIR-1;

static uint8_t PreviousDate;
static uint8_t PreviousSeconds;
static uint8_t PreviousMinutes;
static uint8_t PreviousHours;

static const char* MonthStr[]={
  "Jan.","Feb.","Mar.","Apr.","May","June","July","Aug.","Sep.","Oct.","Nov.","Dec.",
};
static const char* DateStr[]={
  "Mon.","Tue.","Wed.","Thu.","Fri.","Sat.","Sun.",
};

static const float AnalogXY[] = 
{
 0.00000,-1.00000,
 0.10453,-0.99452,
 0.20791,-0.97815,
 0.30902,-0.95106,
 0.40674,-0.91355,
 0.50000,-0.86603,
 0.58779,-0.80902,
 0.66913,-0.74314,
 0.74314,-0.66913,
 0.80902,-0.58779,
 0.86603,-0.50000,
 0.91355,-0.40674,
 0.95106,-0.30902,
 0.97815,-0.20791,
 0.99452,-0.10453,
 1.00000,-0.00000,
 0.99452, 0.10453,
 0.97815, 0.20791,
 0.95106, 0.30902,
 0.91355, 0.40674,
 0.86603, 0.50000,
 0.80902, 0.58779,
 0.74314, 0.66913,
 0.66913, 0.74314,
 0.58779, 0.80902,
 0.50000, 0.86603,
 0.40674, 0.91355,
 0.30902, 0.95106,
 0.20791, 0.97815,
 0.10453, 0.99452,
 0.00000, 1.00000,
-0.10453, 0.99452,
-0.20791, 0.97815,
-0.30902, 0.95106,
-0.40674, 0.91355,
-0.50000, 0.86603,
-0.58779, 0.80902,
-0.66913, 0.74314,
-0.74314, 0.66913,
-0.80902, 0.58779,
-0.86603, 0.50000,
-0.91355, 0.40674,
-0.95106, 0.30902,
-0.97815, 0.20791,
-0.99452, 0.10453,
-1.00000, 0.00000,
-0.99452,-0.10453,
-0.97815,-0.20791,
-0.95106,-0.30902,
-0.91355,-0.40674,
-0.86603,-0.50000,
-0.80902,-0.58779,
-0.74314,-0.66913,
-0.66913,-0.74314,
-0.58779,-0.80902,
-0.50000,-0.86603,
-0.40674,-0.91355,
-0.30902,-0.95106,
-0.20791,-0.97815,
-0.10453,-0.99452,
 0.00000,-1.00000
};

RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef stime;
RTC_DateTypeDef sdate;

void Display_DrawString(uint16_t x,uint16_t y,uint16_t color,uint16_t size,const char *args, ...)
{
	va_list ap;
	char str[30];
	
	va_start(ap,args);
	vsnprintf(str,30,args,ap);

  uint16_t len = strlen((char*)str);  
  
  ILI9325_FillRect(x,y,size*len*6,size*7,BackColor);
  ILI9325_DrawString(x,y,(int8_t *)str,color,size);
  
	va_end(ap);
}

void Display_Set_BackColor(uint16_t color)
{
  BackColor = color;
}

void Display_Reset_PreviousDatas(void)
{
  PreviousDate = 0;
  PreviousSeconds = 0;
  PreviousMinutes = 0;
  PreviousHours = 0;
  ProcessCount_Envir = PROCESSTIME_ENVIR-1;
}

#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }

void Display_DrawHand(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t thickness,uint16_t color) 
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

  if(y0 < y1) ystep = 1;
  else         ystep = -1;

  for (; x0<=x1; x0++) 
  {
    if (steep) 
        ILI9325_FillCircle(y0, x0, thickness,color);
    else 
        ILI9325_FillCircle(x0, y0, thickness,color);
    err -= dy;
    if (err < 0) 
    {
        y0 += ystep;
        err += dx;
    }
  }
}

static void Draw_HourHand(uint16_t x,uint16_t y,uint16_t r,uint8_t time,uint8_t thickness,uint16_t color)
{
  uint16_t x2,y2;

  x2 = x+(int16_t)((float)r*AnalogXY[time*2  ]);
  y2 = y+(int16_t)((float)r*AnalogXY[time*2+1]);  

  Display_DrawHand(x,y,x2,y2,thickness,color);
}

static void Plot_SecondCircle(uint16_t x,uint16_t y,uint8_t r,uint8_t time,uint16_t color)
{
  uint16_t x2,y2;

  x2 = x+(int16_t)((float)r*AnalogXY[time*2  ]);
  y2 = y+(int16_t)((float)r*AnalogXY[time*2+1]);  
  
  ILI9325_FillCircle(x2, y2, 2,color);
}

void Display_AnalogClock(void)
{
  float temp,humid;

  RTC_Get_Calendar(&hrtc,&sdate,&stime);

  if(sdate.Date!=PreviousDate)
  {
    PreviousDate = sdate.Date;

    Display_DrawString(90,220,ILI9325_Color565(0,188,212),2,"%4s %d %d",MonthStr[sdate.Month],sdate.Date,2000 + sdate.Year);
  }

  if(stime.Seconds!=PreviousSeconds)
  { 
    Plot_SecondCircle(160,105,103,PreviousSeconds,BackColor);
    
    Plot_SecondCircle(160,105,103,stime.Seconds,ILI9325_RED);

    PreviousSeconds = stime.Seconds;
  }
  
  if(stime.Minutes!=PreviousMinutes)
  { 
    Draw_HourHand(160,105,60,(PreviousHours<13)? PreviousHours*5:PreviousHours*2.5,1,BackColor);

    Draw_HourHand(160,105,100,PreviousMinutes,1,BackColor);

    PreviousHours = stime.Hours;
    PreviousMinutes = stime.Minutes;

    Display_DrawString(190,102,ILI9325_Color565(0,188,212),2,"%s ",DateStr[sdate.WeekDay-1]);

    Draw_HourHand(160,105,60,(stime.Hours<13)? stime.Hours*5:stime.Hours*2.5,1,ILI9325_GREEN);

    Draw_HourHand(160,105,100,stime.Minutes,1,ILI9325_BLUE);
  }

  if((++ProcessCount_Envir)>PROCESSTIME_ENVIR)
  {
    ProcessCount_Envir = 0;

    SHT31_Read_Data();

    temp = SHT31_Get_Temperature();
    humid = SHT31_Get_Humidity();
    
    Display_DrawString(20,186,ILI9325_BLUE,4,"%2hd",(uint16_t)temp);
    ILI9325_DrawCircle(72,202,2,ILI9325_BLUE);
    Display_DrawString(76,200,ILI9325_BLUE,2,"C");
  
    Display_DrawString(238,186,ILI9325_BLUE,4,"%2hd",(uint16_t)humid);
    Display_DrawString(294,200,ILI9325_BLUE,2,"%%");
  }

}

void Display_DigitalClock(void)
{
  float temp,humid;

  RTC_Get_Calendar(&hrtc,&sdate,&stime);

  if(sdate.Date!=PreviousDate)
  {
    PreviousDate = sdate.Date;

    Display_DrawString(50,200,ILI9325_Color565(0,188,212),2,"%s ",RTC_Get_WeekDay_Char(&sdate));
    Display_DrawString(180,90,ILI9325_Color565(0,188,212),3,"%-4s%02d",MonthStr[sdate.Month],sdate.Date);
    Display_DrawString(187,37,ILI9325_Color565(0,188,212),4,"%4d",2000+sdate.Year);
  }

  if(stime.Minutes!=PreviousMinutes)
  {
    PreviousMinutes = stime.Minutes;

    Display_DrawString(35,37,ILI9325_Color565(96,125,139),8,"%2d",stime.Hours);
    Display_DrawString(35,117,ILI9325_Color565(96,125,139),8,"%02d",stime.Minutes);
  }

  if(stime.Seconds!=PreviousSeconds)
  {
    PreviousSeconds = stime.Seconds;
    
    Display_DrawString(131,145,ILI9325_Color565(96,125,139),4,"%02d",stime.Seconds);
  }


  if((++ProcessCount_Envir)>PROCESSTIME_ENVIR)
  {
    ProcessCount_Envir = 0;

    SHT31_Read_Data();

    temp = SHT31_Get_Temperature();
    humid = SHT31_Get_Humidity();

    Display_DrawString(213,153,ILI9325_BLUE,4,"%2hd",(uint16_t)temp);
    ILI9325_DrawCircle(265,169,2,ILI9325_BLUE);
    Display_DrawString(269,167,ILI9325_BLUE,2,"C");
  
    Display_DrawString(213,186,ILI9325_BLUE,4,"%2hd",(uint16_t)humid);
    Display_DrawString(269,200,ILI9325_BLUE,2,"%%");
  }

}

static uint8_t BackLight_Blight = 0; 

void Display_Set_Blightless(uint8_t Blight)
{
  if(Blight>7)Blight = 7;
  BackLight_Blight = Blight;
}

void Display_Process_BackLight(void)
{
  static uint8_t count = 0;
  
  switch(count)
  {
    case 1:
      if(BackLight_Blight&0x01)
        Display_Set_BackLight();
      else
        Display_Reset_BackLight();
    break;
    case 2:
      if(BackLight_Blight&0x02)
        Display_Set_BackLight();
      else
        Display_Reset_BackLight();
    break;
    case 4:
      if(BackLight_Blight&0x04)
        Display_Set_BackLight();
      else
        Display_Reset_BackLight();
    break;
    default:
    break;
  }
  
  if(++count>7)count = 0;
}
