/**
  ******************************************************************************
  * File Name          : RTC_Operation.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_OPERATION_H
#define __RTC_OPERATION_H
  /* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_rtc.h"
#include "main.h"

/* USER CODE BEGIN Includes */
void RTC_Set_Calendar(RTC_HandleTypeDef *hrtc,RTC_DateTypeDef *sDate,RTC_TimeTypeDef *sTime);
void RTC_Get_Calendar(RTC_HandleTypeDef *hrtc,RTC_DateTypeDef *sDate,RTC_TimeTypeDef *sTime);  
const char* RTC_Get_WeekDay_Char(RTC_DateTypeDef *sDate);
void RTC_Show_Calendar(RTC_HandleTypeDef *hrtc,RTC_DateTypeDef *sDate,RTC_TimeTypeDef *sTime);
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#endif /* RTC_Operation_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
