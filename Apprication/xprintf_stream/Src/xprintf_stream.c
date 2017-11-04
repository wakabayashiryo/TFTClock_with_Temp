#include "xprintf_stream.h"

UART_HandleTypeDef huart2;
uint32_t UART_Ready = 1;

static uint8_t *pointBuff;
static uint8_t FrontBuff[MAXBUFFERSIZE];
static uint32_t BufferSize;
static uint32_t wp = 0;

static void xStream_Write_Buff(uint8_t c)
{
  *(pointBuff+wp) = c;

  wp++;

  if(wp>(BufferSize-1))
    xStream_fflush();
}
static inline void xStream_Clear_Buff(uint32_t size)
{
  for(uint32_t cp = 0;cp<size;cp++)
      *(pointBuff+cp) = '\0';
}

void xStream_Setbuf(uint8_t *pBuff,uint32_t size)
{
  pointBuff = pBuff;
  BufferSize = size;

  xdev_out(xStream_Write_Buff);
}

void xStream_fflush(void)
{
  uint32_t stln = strlen((char*)pointBuff);

  HAL_StatusTypeDef CheckStatus;

  memcpy(FrontBuff,pointBuff,stln);

  if(stln>0)
  {
    UART_Ready = 0;

    CheckStatus = HAL_UART_Transmit_DMA(&huart2,FrontBuff,stln);
    if(CheckStatus==HAL_ERROR)
      _Error_Handler(__FILE__, __LINE__);

    while(!UART_Ready);

    xStream_Clear_Buff(stln);
    wp = 0;
  }  
  else
  {
    return;
  }
}

void HAL_UART_TxCpltCallback (UART_HandleTypeDef *huart)
{
  UART_Ready = 1;
}