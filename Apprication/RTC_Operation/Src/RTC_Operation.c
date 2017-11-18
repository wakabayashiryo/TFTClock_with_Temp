#include "RTC_Operation.h"

const char *weekday_char[] = {
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday",
  "Sunday"
};

/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
  void RTC_Set_Calendar(RTC_HandleTypeDef *hrtc,RTC_DateTypeDef *sDate,RTC_TimeTypeDef *sTime)
  { 
      if(HAL_RTC_SetDate(hrtc,sDate,RTC_FORMAT_BIN) != HAL_OK)
      {
        /* Initialization Error */
        Error_Handler();
      }
    
      if (HAL_RTC_SetTime(hrtc, sTime, RTC_FORMAT_BIN) != HAL_OK)
      {
        /* Initialization Error */
        Error_Handler();
      }
    
      /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
      HAL_RTCEx_BKUPWrite(hrtc, RTC_BKP_DR1, 0x32F2);
  }
  
  void RTC_Get_Calendar(RTC_HandleTypeDef *hrtc,RTC_DateTypeDef *sDate,RTC_TimeTypeDef *sTime)
  {  
    /* Get the RTC current Time */
    if(HAL_RTC_GetTime(hrtc, sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
    /* Get the RTC current Date */
    if(HAL_RTC_GetDate(hrtc, sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
  }

  const char* RTC_Get_WeekDay_Char(RTC_DateTypeDef *sDate)
  {
    if(!IS_RTC_WEEKDAY(sDate->WeekDay))
      return "Invalid Weekday number";
    return weekday_char[sDate->WeekDay-1];
  }
  /**
    * @brief  Display the current time and date.
    * @param  showtime : pointer to buffer
    * @param  showdate : pointer to buffer
    * @retval None
    */
  void RTC_Show_Calendar(RTC_HandleTypeDef *hrtc,RTC_DateTypeDef *sDate,RTC_TimeTypeDef *sTime)
  {
    /* Get the RTC current Time & Date*/
    RTC_Get_Calendar(hrtc,sDate,sTime);

    /* Display date Format : mm-dd-yy */
    xprintf("Date:%4d-%02d-%02d[%s] ",2000 + sDate->Year,sDate->Month,sDate->Date,RTC_Get_WeekDay_Char(sDate));
    /* Display time Format : hh:mm:ss */
    xprintf("Time:%2d:%2d:%2d\n", sTime->Hours, sTime->Minutes, sTime->Seconds);
    
    xStream_fflush();
  }