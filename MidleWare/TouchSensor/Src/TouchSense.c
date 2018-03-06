#include "TouchSense.h"

I2C_HandleTypeDef hi2c1;
static uint8_t ReceiveDatas[5];

int8_t TouchSense_Set_Configuration(uint16_t th1,uint16_t th2)
{
    uint8_t TransmitDatas[5];

    TransmitDatas[0] = F1822_CONFIG;  
    
    TransmitDatas[1] = (th1&0xFF00)>>8;      //Threshold of channel 0
    TransmitDatas[2] =  th1&0x00FF;

    TransmitDatas[3] =  (th2&0xFF00)>>8;     //Threshold of channel 1
    TransmitDatas[4] =  th2&0x00FF;

    if(HAL_I2C_Master_Transmit_DMA(&hi2c1,TOUCH_ADDRESS,TransmitDatas,5)==HAL_ERROR)
        return -1;    
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);
        
    return 0;
}

int8_t TouchSense_Read_Value(void)
{
    uint8_t ReadRegister = F1822_VALUE;

    if(HAL_I2C_Master_Transmit_DMA(&hi2c1,TOUCH_ADDRESS,&ReadRegister,1)==HAL_ERROR)
        return -1;    
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);

    if(HAL_I2C_Master_Receive_DMA(&hi2c1,TOUCH_ADDRESS,ReceiveDatas,5)==HAL_ERROR)
        return 1;
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);
    
    return 0;
}

uint16_t TouchSense_Get_Value1(void)
{
    if(ReceiveDatas[0]!=0x44)return 1;
    return (ReceiveDatas[1]<<8)|ReceiveDatas[2];
}

uint16_t TouchSense_Get_Value2(void)
{
    if(ReceiveDatas[0]!=0x44)return 1;
    return (ReceiveDatas[3]<<8)|ReceiveDatas[4];
}