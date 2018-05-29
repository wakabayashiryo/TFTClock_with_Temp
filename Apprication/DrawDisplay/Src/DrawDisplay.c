#include "DrawDisplay.h"
#include "stdarg.h"
#include "stm32f4xx_hal_rtc.h"

static uint16_t BackColor = ILI9325_BLACK;
static uint16_t ProcessCount_Envir = PROCESSTIME_ENVIR-1;

static uint8_t PreviousDate     = 0;
static uint8_t PreviousSeconds  = 0;
static uint8_t PreviousMinutes  = 0;
static uint8_t PreviousHours    = 0;

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

    Draw_HourHand(160,105,60,(stime.Hours<13)? stime.Hours*5:(stime.Hours-12)*5,1,ILI9325_GREEN);

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
    Display_DrawString(180,90,ILI9325_Color565(0,188,212),3,"%-4s%2d",MonthStr[sdate.Month],sdate.Date);
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
 
void Draw_Adjusing_pointer(uint8_t stat)
{
  const uint16_t point_pos[][2] = {
    {15,55},{15,125},{25,195},{160,45},{160,90},{160,90},
  };
  if(stat>0)
    Display_DrawString(point_pos[stat-1][0],point_pos[stat-1][1],ILI9325_WHITE,3,">");
  
  Display_DrawString(point_pos[stat][0],point_pos[stat][1],ILI9325_RED,3,">");
}

void Display_Adjust_Time(user_config *uc)
{
  static AdjustTime_States adjt = STORE_TIME;
  static RTC_TimeTypeDef stime_temp;
  static RTC_DateTypeDef sdate_temp;

  switch((uint8_t)adjt)
  {
    case STORE_TIME:
     
      RTC_Get_Calendar(&hrtc,&sdate_temp,&stime_temp);

      Draw_Adjusing_pointer(adjt);

      Display_DrawString(187,37,ILI9325_Color565(0,188,212),4,"%4d",2000+sdate_temp.Year);
      Display_DrawString(180,90,ILI9325_Color565(0,188,212),3,"%-4s%2d",MonthStr[sdate_temp.Month],sdate_temp.Date);
      Display_DrawString(50,200,ILI9325_Color565(0,188,212),2,"%s ",RTC_Get_WeekDay_Char(&sdate_temp));
      Display_DrawString(35,37,ILI9325_Color565(96,125,139),8,"%2d",stime_temp.Hours);
      Display_DrawString(35,117,ILI9325_Color565(96,125,139),8,"%02d",stime_temp.Minutes);
    
      adjt = ADJUST_HOUR;
  break;
    
    case ADJUST_HOUR:
      if(uc->pt2 > _DETECT_TOUCH)
      { if(++stime_temp.Hours>23)stime_temp.Hours=0;
        Display_DrawString(35,37,ILI9325_Color565(96,125,139),8,"%2d",stime_temp.Hours);
      }
    break;
    
    case ADJUST_MINU:
      if(uc->pt2 > _DETECT_TOUCH)
      { if(++stime_temp.Minutes>59)stime_temp.Minutes=0;
      Display_DrawString(35,117,ILI9325_Color565(96,125,139),8,"%02d",stime_temp.Minutes);}
    break;
    
    case ADJUST_WEEK:
      if(uc->pt2 > _DETECT_TOUCH)
      { if(++sdate_temp.WeekDay>7)sdate_temp.WeekDay=1;
      Display_DrawString(50,200,ILI9325_Color565(0,188,212),2,"%s ",RTC_Get_WeekDay_Char(&sdate_temp));}
    break;
    
    case ADJUST_YEAR:
      if(uc->pt2 > _DETECT_TOUCH)
      { if(++sdate_temp.Year>100)sdate_temp.Year=0;
      Display_DrawString(187,37,ILI9325_Color565(0,188,212),4,"%4d",2000+sdate_temp.Year);}
    break;
    
    case ADJUST_MONT:
      if(uc->pt2 > _DETECT_TOUCH)
      { if(++sdate_temp.Month>12)sdate_temp.Month=1;
      Display_DrawString(180,90,ILI9325_Color565(0,188,212),3,"%-4s%2d",MonthStr[sdate_temp.Month],sdate_temp.Date);}
    break;
    
    case ADJUST_DATE:
      if(uc->pt2 > _DETECT_TOUCH)
      { if(++sdate_temp.Date>31)sdate_temp.Date=0;
      Display_DrawString(180,90,ILI9325_Color565(0,188,212),3,"%-4s%2d",MonthStr[sdate_temp.Month],sdate_temp.Date);}
    break;
    
    default:
    break;
  }

  if(uc->pt1 > _DETECT_TOUCH)
  {
    Draw_Adjusing_pointer(adjt);

    if(++adjt>6)
    {
      adjt = STORE_TIME;  
      uc->state = SETTING;
      stime_temp.Seconds = 0;
      RTC_Set_Calendar(&hrtc,&sdate_temp,&stime_temp);

      ILI9325_FillScreen(ILI9325_WHITE);
      Display_Reset_PreviousDatas();
    }
  } 
}
 
void Draw_Setting_pointer(uint8_t stat)
{
  const uint16_t point_pos[][2] = {
    {10,45},{10,85},{10,125},
  };
  if(stat>0)
    Display_DrawString(point_pos[stat-1][0],point_pos[stat-1][1],ILI9325_WHITE,2,">");
  
    Display_DrawString(point_pos[stat][0],point_pos[stat][1],ILI9325_RED,2,">");
}

void Display_user_Setting(user_config *uc)
{
  static Setting_States cs= DROW_TEXT;
  static uint8_t param_point = 0;
  static const float saver_time_param[] = {
    1.0, 3.0 , 5.0, 10.0, 30.0 , 60.0 , 90.0, 
  };

  switch((uint8_t)cs)
  {
    case DROW_TEXT:
      ILI9325_DrawString(35,40,(int8_t *)"Beep Sound",ILI9325_BLACK,3);
      ILI9325_DrawString(255,40,(uc->Beep.beep_switch==1)?(int8_t *)"ON ":(int8_t *)"OFF",ILI9325_BLACK,3);

      ILI9325_DrawString(35,80,(int8_t *)"Screen Saver",ILI9325_BLACK,3);
      ILI9325_DrawString(255,80,(uc->Backlight.saver_switch==1)?(int8_t *)"ON ":(int8_t *)"OFF",ILI9325_BLACK,3);
      
      ILI9325_DrawString(35,120,(int8_t *)"Saver Time",ILI9325_BLACK,3);
      ILI9325_DrawString(280,125,(int8_t *)"min",ILI9325_BLACK,2);
      Display_DrawString(235,120,ILI9325_BLACK,3,"%2d",(uint16_t)uc->Backlight.saver_minutes);

      Draw_Setting_pointer(cs);
      cs = SETTING_BEEP;
    break;

    case SETTING_BEEP:
      if(uc->pt2> _DETECT_TOUCH)
      { 
        uc->Beep.beep_switch = !uc->Beep.beep_switch;
        
        if(uc->Beep.beep_switch)
          Display_DrawString(255,40,ILI9325_BLACK,3,"ON ");  
        else
          Display_DrawString(255,40,ILI9325_BLACK,3,"OFF");
      }
      break;

    case SETTING_SAVER:
      if(uc->pt2> _DETECT_TOUCH)
      { 
        uc->Backlight.saver_switch = !uc->Backlight.saver_switch;
        
        if(uc->Backlight.saver_switch)
          Display_DrawString(255,80,ILI9325_BLACK,3,"ON ");  
        else
          Display_DrawString(255,80,ILI9325_BLACK,3,"OFF");
      }
    break;
    
    case SETTING_SAVER_TIME:
      if(uc->pt2> _DETECT_TOUCH)
      { 
        if(++param_point>6)param_point=0;

        uc->Backlight.saver_minutes = saver_time_param[param_point];
        
        Display_DrawString(235,120,ILI9325_BLACK,3,"%2d",(uint16_t)saver_time_param[param_point]);  
      }
    break;
    
    default:
    break;
  }
  
  if(uc->pt1 > _DETECT_TOUCH)
  {
    Draw_Setting_pointer(cs);    
    
    if(++cs>3)
    {
      cs = DROW_TEXT;  
      uc->state = DIGITAL_CLOCK;

      ILI9325_FillScreen(ILI9325_WHITE);
      Display_Reset_PreviousDatas();
    }
  }
}