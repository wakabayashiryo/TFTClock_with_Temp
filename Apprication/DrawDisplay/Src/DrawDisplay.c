#include "DrawDisplay.h"
#include "stm32f4xx_hal_rtc.h"

static uint16_t BackColor = ILI9325_BLACK;
static uint16_t ProcessCount_Envir = PROCESSTIME_ENVIR-1;

static uint8_t PreviousMinutes;
static uint8_t PreviousSeconds;
static uint8_t PreviousDate;

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

  char Datestring[25];
  char ClockString[6];
  char TempString[10];
  char HumidString[10];

  RTC_Get_Calendar(&hrtc,&sdate,&stime);

  if(sdate.Date!=PreviousDate)
  {
    PreviousDate = sdate.Date;

    sprintf(Datestring,"%4d-%02d-%02d[%s] ",2000 + sdate.Year,sdate.Month,sdate.Date,RTC_Get_WeekDay_Char(&sdate));
    Clear_StringSpace(CLCOKDATE_X,CLCOKDATE_Y,(int8_t *)Datestring,2);
    ILI9325_DrawString(CLCOKDATE_X,CLCOKDATE_Y,(int8_t *)Datestring,ILI9325_Color565(0,188,212),2);
  }

  if(stime.Minutes!=PreviousMinutes)
  {
    PreviousMinutes = stime.Minutes;

    sprintf(ClockString,"%2d:%02d",stime.Hours,stime.Minutes);
    Clear_StringSpace(CLCOKMAIN_X,CLCOKMAIN_Y,(int8_t *)ClockString,9);
    ILI9325_DrawString(CLCOKMAIN_X,CLCOKMAIN_Y,(int8_t *)ClockString,ILI9325_Color565(96,125,139),9);
  }
  if(stime.Seconds!=PreviousSeconds)
  {
    PreviousSeconds = stime.Seconds;
    
    sprintf(ClockString,"%02d",stime.Seconds);
    Clear_StringSpace(CLOCKSECOND_X,CLOCKSECOND_Y,(int8_t *)ClockString,5);
    ILI9325_DrawString(CLOCKSECOND_X,CLOCKSECOND_Y,(int8_t *)ClockString,ILI9325_Color565(96,125,139),4);
  }


  if((++ProcessCount_Envir)>PROCESSTIME_ENVIR)
  {
    ProcessCount_Envir = 0;

    // SHT31_Read_Data();

//     temp = SHT31_Get_Temperature();
//     humid = SHT31_Get_Humidity();
  
//     sprintf(TempString,"%hd.%1d",(int16_t)temp,((uint16_t)(temp*10)%10));
//     Clear_StringSpvoid Display_DigitalClock(void)
// {ace(TEMP_X,TEMP_Y,(int8_t *)TempString,3);
//     ILI9325_DrawString(TEMP_X,TEMP_Y,(int8_t *)TempString,ILI9325_BLUE,3);

//     sprintf(HumidString,"%hd.%1d",(int16_t)humid,((uint16_t)(humid*10)%10));
//     Clear_StringSpace(HUMI_X,HUMI_Y,(int8_t *)HumidString,3);
//     ILI9325_DrawString(HUMI_X,HUMI_Y,(int8_t *)HumidString,ILI9325_BLUE,3);
    
  }

}
