/**
  ******************************************************************************
  * File Name          : main.h
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
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "SHT31.h"
#include "DrawDisplay.h"
#include "xprintf.h"
#include "ILI9325_Driver.h"
#include "xprintf_stream.h"
#include "RTC_Operation.h"
#include "TouchSense.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define BackLight_Pin GPIO_PIN_0
#define BackLight_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA

#define LCD_RST_Pin GPIO_PIN_1
#define LCD_RST_GPIO_Port GPIOC
#define LCD_RD_Pin GPIO_PIN_0
#define LCD_RD_GPIO_Port GPIOA
#define LCD_WR_Pin GPIO_PIN_1
#define LCD_WR_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_4
#define LCD_RS_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_0
#define LCD_CS_GPIO_Port GPIOB

#define LCD_D0_Pin GPIO_PIN_9
#define LCD_D0_GPIO_Port GPIOA
#define LCD_D1_Pin GPIO_PIN_7
#define LCD_D1_GPIO_Port GPIOC
#define LCD_D2_Pin GPIO_PIN_10
#define LCD_D2_GPIO_Port GPIOA
#define LCD_D3_Pin GPIO_PIN_3
#define LCD_D3_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_5
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_4
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_10
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_8
#define LCD_D7_GPIO_Port GPIOA

#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SD_CS_Pin GPIO_PIN_6
#define SD_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

typedef enum
{
  SCREEN_SAVER  = 0,
  DIGITAL_CLOCK = 1,
  ANALOG_CLOCK  = 2,
  SETTING       = 3,
  ADJ_TIME      = 4,
}Operational_States;

typedef struct
{
  Operational_States state;
  Operational_States state_temp;
  
  struct
  {
    uint8_t saver_switch;
    float saver_minutes;
    uint32_t saver_timer;
  }Backlight;

  struct
  {
    uint8_t beep_switch;
    uint32_t beep_timer;
  }Beep;
  uint16_t pt1;
  uint16_t pt2;
}user_config;

#define _DETECT_TOUCH 10

#define SOUND_BEEP_ms(t)    uconf.Beep.beep_timer = t 
#define SCREENSAVER_RESET() uconf.Backlight.saver_timer = uconf.Backlight.saver_minutes*60.0*1000.0

#define SOUND_FREQ 4000

#define PWM_PERIOD (uint32_t)(1000000/SOUND_FREQ)

#define Buzzer_Start()              if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK){Error_Handler();}
#define Buzzer_ON()                 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,PWM_PERIOD>>1)
#define Buzzer_OFF()                __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0)
#define Buzzer_Set_Frequency(freq)  __HAL_TIM_SET_AUTORELOAD(&htim2, (uint32_t)(1000000/freq))

/* USER CODE END Private defines */
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
