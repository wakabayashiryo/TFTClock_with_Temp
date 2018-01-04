#include "DrawDisplay.h"
#include "stm32f4xx_hal_rtc.h"

static uint16_t BackColor = ILI9325_BLACK;
static uint16_t ProcessCount_Envir = PROCESSTIME_ENVIR-1;

static uint8_t PreviousMinutes;
static uint8_t PreviousSeconds;

RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef stime;
RTC_DateTypeDef sdate;

static void Clear_StringSpace(uint16_t x,uint16_t y,int8_t *str,uint16_t size)
{
  uint16_t len = strlen((char*)str);

  ILI9325_FillRect(x,y,size*len*6,size*7,BackColor);
}

void Display_Set_BackColor(uint16_t color)
{
  BackColor = color;
}

void Display_DigitalClock(void)
{
  float temp,humid;

  char ClockString[6];
  char TempString[30];
  char HumidString[30];

  RTC_Get_Calendar(&hrtc,&sdate,&stime);

  if(stime.Minutes!=PreviousMinutes)
  {
    PreviousMinutes = stime.Minutes;

    sprintf(ClockString,"%2d:%02d",stime.Hours,stime.Minutes);
    Clear_StringSpace(0,120,(int8_t *)ClockString,9);
    ILI9325_DrawString(0,120,(int8_t *)ClockString,ILI9325_YELLOW,9);
  }
  if(stime.Seconds!=PreviousSeconds)
  {
    PreviousSeconds = stime.Seconds;
    
    sprintf(ClockString,"%02d",stime.Seconds);
    Clear_StringSpace(260,150,(int8_t *)ClockString,5);
    ILI9325_DrawString(260,150,(int8_t *)ClockString,ILI9325_YELLOW,5);
  }


  if((++ProcessCount_Envir)>PROCESSTIME_ENVIR)
  {
    ProcessCount_Envir = 0;

    // SHT31_Read_Data();

    temp = SHT31_Get_Temperature();
    humid = SHT31_Get_Humidity();
  
    sprintf(TempString,"%hd.%1dC",(int16_t)temp,((uint16_t)(temp*100)%10));
    Clear_StringSpace(0,40,(int8_t *)TempString,4);
    ILI9325_DrawString(0,40,(int8_t *)TempString,ILI9325_BLUE,4);

    sprintf(HumidString,"%hd.%1d%%",(int16_t)humid,((uint16_t)(humid*100)%10));
    Clear_StringSpace(200,40,(int8_t *)HumidString,4);
    ILI9325_DrawString(200,40,(int8_t *)HumidString,ILI9325_BLUE,4);
    
  }

}
