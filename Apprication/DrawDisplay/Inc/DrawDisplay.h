/**
  ******************************************************************************
  * File Name          : DrawDisplay.h
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
#ifndef __DRAWDISPLAY_H
#define __DRAWDISPLAY_H
  /* Includes ------------------------------------------------------------------*/
#include "main.h"

#define PROCESSTIME_ENVIR 30000 //ms

typedef enum
{
  STORE_TIME      = 0,
  ADJUST_HOUR     = 1,
  ADJUST_MINU     = 2,
  ADJUST_WEEK     = 3,
  ADJUST_YEAR     = 4,
  ADJUST_MONT     = 5,
  ADJUST_DATE     = 6
}AdjustTime_States;

typedef enum
{
  DROW_TEXT           = 0,
  SETTING_BEEP        = 1,
  SETTING_SAVER       = 2,
  SETTING_SAVER_TIME  = 3,
}Setting_States;

/* USER CODE BEGIN Includes */

#define Display_Set_BackLight()   GPIOC->ODR &= ~(1<<0)
#define Display_Reset_BackLight() GPIOC->ODR |=  (1<<0)

void Display_Reset_PreviousDatas(void);
void Display_Set_BackColor(uint16_t color);

void Display_DigitalClock(void);
void Display_AnalogClock(void);

void Display_Adjust_Time();
void Display_user_Setting();
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#endif /* __DRAWDISPLAY_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
