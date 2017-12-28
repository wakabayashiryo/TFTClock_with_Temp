#include "DrawDisplay.h"

static uint16_t BackColor = ILI9325_BLACK;
static uint16_t ProcessCount_Clock = 0;
static uint16_t ProcessCount_Envir  = 0;

RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef stime;
RTC_DateTypeDef sdate;

static void Clear_StringSpace(uint16_t x,uint16_t y,int8_t *str,uint16_t size)
{
  uint16_t len = strlen((char*)str);

  ILI9325_FillRect(x,y,size*len*6,size*len*8,BackColor);
}

void Display_Set_BackColor(uint16_t color)
{
  BackColor = color;
}

void Dispaly_DigitalClock(void)
{
  char ClockString[6];
  char EnvirString[2][6];

  if((++ProcessCount_Clock)>PROCESSTIME_CLOCK)
  {
    ProcessCount_Clock = 0;

    RTC_Get_Calendar(&hrtc,&sdate,&stime);

    sprintf(ClockString,"%2d:%02d",stime.Hours,stime.Minutes);
    Clear_StringSpace(0,120,(int8_t *)ClockString,9);
    ILI9325_DrawString(0,120,(int8_t *)ClockString,ILI9325_BLUE,9);
  }

  if((++ProcessCount_Envir)>PROCESSTIME_ENVIR)
  {
    ProcessCount_Envir = 0;

    SHT31_Read_Data();

    sprintf(EnvirString[0],"%2.1f",SHT31_Get_Temperature());
    Clear_StringSpace(0,0,(int8_t *)EnvirString[0],5);
    ILI9325_DrawString(0,0,(int8_t *)EnvirString[0],ILI9325_BLUE,5);

    sprintf(EnvirString[1],"%2f",SHT31_Get_Humidity());
    Clear_StringSpace(160,0,(int8_t *)EnvirString[1],5);
    ILI9325_DrawString(160,0,(int8_t *)EnvirString[1],ILI9325_BLUE,5);
    
  }
}
