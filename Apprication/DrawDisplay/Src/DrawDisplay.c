#include "DrawDisplay.h"
#include "stdarg.h"
#include "stm32f4xx_hal_rtc.h"

static uint16_t BackColor = ILI9325_BLACK;
static uint16_t ProcessCount_Envir = PROCESSTIME_ENVIR-1;

static uint8_t PreviousDate;
static uint8_t PreviousSeconds;
static uint8_t PreviousMinutes;

RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef stime;
RTC_DateTypeDef sdate;

static const char* MonthStr[]={
  "Jan.","Feb.","Mar.","Apr.","May","June","July","Aug.","Sep.","Oct.","Nov.","Dec.",
};

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
