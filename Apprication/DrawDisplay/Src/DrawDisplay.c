#include "DrawDisplay.h"
#include "stdarg.h"
#include "stm32f4xx_hal_rtc.h"

static uint16_t BackColor = ILI9325_BLACK;
static uint16_t ProcessCount_Envir = PROCESSTIME_ENVIR-1;

static uint8_t PreviousMinutes;
static uint8_t PreviousSeconds;
static uint8_t PreviousDate;

static const float AnalogXY[] = 
{
 1.000, 0.000,
 0.995,  0.105,
 0.978,  0.208,
 0.951,  0.309,
 0.914,  0.407,
 0.866,  0.500,
 0.809,  0.588,
 0.743,  0.669,
 0.669,  0.743,
 0.588,  0.809,
 0.500,  0.866,
 0.407,  0.914,
 0.309,  0.951,
 0.208,  0.978,
 0.105,  0.995,
 0.000,  1.000,
-0.105,  0.995,
-0.208,  0.978,
-0.309,  0.951,
-0.407,  0.914,
-0.500,  0.866,
-0.588,  0.809,
-0.669,  0.743,
-0.743,  0.669,
-0.809,  0.588,
-0.866,  0.500,
-0.914,  0.407,
-0.951,  0.309,
-0.978,  0.208,
-0.995,  0.105,
-1.000,  0.000,
-0.995, -0.105,
-0.978, -0.208,
-0.951, -0.309,
-0.914, -0.407,
-0.866, -0.500,
-0.809, -0.588,
-0.743, -0.669,
-0.669, -0.743,
-0.588, -0.809,
-0.500, -0.866,
-0.407, -0.914,
-0.309, -0.951,
-0.208, -0.978,
-0.105, -0.995,
-0.000, -1.000,
 0.105, -0.995,
 0.208, -0.978,
 0.309, -0.951,
 0.407, -0.914,
 0.500, -0.866,
 0.588, -0.809,
 0.669, -0.743,
 0.743, -0.669,
 0.809, -0.588,
 0.866, -0.500,
 0.914, -0.407,
 0.951, -0.309,
 0.978, -0.208,
 0.995, -0.105,
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

void Display_DigitalClock(void)
{
  float temp,humid;

  RTC_Get_Calendar(&hrtc,&sdate,&stime);

  if(sdate.Date!=PreviousDate)
  {
    PreviousDate = sdate.Date;

    Display_DrawString(CLCOKDATE_X,CLCOKDATE_Y,ILI9325_Color565(0,188,212),2,"%4d-%02d-%02d[%s] ",2000 + sdate.Year,sdate.Month,sdate.Date,RTC_Get_WeekDay_Char(&sdate));
  }

  if(stime.Minutes!=PreviousMinutes)
  {
    PreviousMinutes = stime.Minutes;

    Display_DrawString(CLCOKMAIN_X,CLCOKMAIN_Y,ILI9325_Color565(96,125,139),9,"%2d:%02d",stime.Hours,stime.Minutes);
  }
  if(stime.Seconds!=PreviousSeconds)
  {
    PreviousSeconds = stime.Seconds;
    
    Display_DrawString(CLOCKSECOND_X,CLOCKSECOND_Y,ILI9325_Color565(96,125,139),5,"%02d",stime.Seconds);
  }


  if((++ProcessCount_Envir)>PROCESSTIME_ENVIR)
  {
    ProcessCount_Envir = 0;

    // SHT31_Read_Data();

    temp = SHT31_Get_Temperature();
    humid = SHT31_Get_Humidity();
  
    Display_DrawString(TEMP_X,TEMP_Y,ILI9325_BLUE,3,"%hd.%1d",(int16_t)temp,((uint16_t)(temp*10)%10));

    Display_DrawString(HUMI_X,HUMI_Y,ILI9325_BLUE,3,"%hd.%1d",(int16_t)humid,((uint16_t)(humid*10)%10));
  }

}

void Calcu_HourHand(uint16_t x,uint16_t y,uint16_t r,uint8_t color)
{
  uint16_t x2,y2;

  x2 = x+(int16_t)((float)r*AnalogXY[stime.Seconds*2  ]);
  y2 = y+(int16_t)((float)r*AnalogXY[stime.Seconds*2+1]);  

  ILI9325_DrawLine(x,y,x2,y2,color);
}

void Display_AnalogClock(void)
{
  float temp,humid;

  RTC_Get_Calendar(&hrtc,&sdate,&stime);

  if(sdate.Date!=PreviousDate)
  {
    PreviousDate = sdate.Date;

    Display_DrawString(CLCOKDATE_X,CLCOKDATE_Y,ILI9325_Color565(0,188,212),2,"%4d-%02d-%02d[%s] ",2000 + sdate.Year,sdate.Month,sdate.Date,RTC_Get_WeekDay_Char(&sdate));
  }

  Calcu_HourHand(160,120,120,ILI9325_BLACK);

  ILI9325_DrawCircle(160,120,120,ILI9325_BLACK);
  ILI9325_DrawCircle(160,120,119,ILI9325_BLACK);

  if((++ProcessCount_Envir)>PROCESSTIME_ENVIR)
  {
    ProcessCount_Envir = 0;

    // SHT31_Read_Data();

    temp = SHT31_Get_Temperature();
    humid = SHT31_Get_Humidity();
    
    Display_DrawString(TEMP_X,TEMP_Y,ILI9325_BLUE,3,"%hd.%1d",(int16_t)temp,((uint16_t)(temp*10)%10));

    Display_DrawString(HUMI_X,HUMI_Y,ILI9325_BLUE,3,"%hd.%1d",(int16_t)humid,((uint16_t)(humid*10)%10));
  }

}
