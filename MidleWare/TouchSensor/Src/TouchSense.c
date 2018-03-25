#include "TouchSense.h"

I2C_HandleTypeDef hi2c1;
static uint8_t ReceiveDatas[5];
static SensorValue sval[2];

#define TouchSense_Get_Value1() (uint16_t)((ReceiveDatas[1]<<8)|ReceiveDatas[2])
#define TouchSense_Get_Value2() (uint16_t)((ReceiveDatas[3]<<8)|ReceiveDatas[4])

int8_t TouchSense_Set_Configuration(uint16_t th1,uint16_t th2)
{
    uint8_t TransmitDatas[5];

    sval[0].threshould = th1;
    sval[1].threshould = th2;

    TransmitDatas[0] = F1822_CONFIG;  
    
    TransmitDatas[1] = (th1&0xFF00)>>8;      //Threshold of channel 0
    TransmitDatas[2] =  th1&0x00FF;

    TransmitDatas[3] =  (th2&0xFF00)>>8;     //Threshold of channel 1
    TransmitDatas[4] =  th2&0x00FF;

    if(HAL_I2C_Master_Transmit_DMA(&hi2c1,TOUCH_ADDRESS,TransmitDatas,5)==HAL_ERROR)
        return -1;    
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);
    
    HAL_Delay(100);

    /*Get Regular Value for comparing difference*/
    for(uint32_t i=0;i<32;i++)
    {
        TouchSense_Read_Value();
        sval[0].regular += TouchSense_Get_Value1();
        sval[1].regular += TouchSense_Get_Value2();
        HAL_Delay(1);
    } 
    sval[0].regular = sval[0].regular>>6;
    sval[1].regular = sval[1].regular>>6;

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

void TouchSense_Count_Touching(void)
{
    static uint16_t count=0;
    if(++count>SCANRATE)
    {
        count = 0;
        TouchSense_Read_Value();
        
        if(TouchSense_Get_Value1()>(sval[0].threshould+sval[0].regular))
        {
            sval[0].TouchCount_ms += SCANRATE;
        }   
        else
        {
            sval[0].StoreTime = sval[0].TouchCount_ms;
            sval[0].TouchCount_ms = 0;
        }
        
        if(TouchSense_Get_Value2()>(sval[1].threshould+sval[1].regular))
        {
            sval[1].TouchCount_ms += SCANRATE;
        }   
        else
        {
            sval[1].StoreTime = sval[1].TouchCount_ms;
            sval[1].TouchCount_ms = 0;
        }
    }
}

uint16_t TouchSense_Get_TouchTime(uint8_t ch)
{
    if(ch>1)return 0;
    uint16_t time_temp;

    time_temp = sval[ch].StoreTime;
    sval[ch].StoreTime = 0;

    return time_temp;
}