#include "TouchSense.h"

I2C_HandleTypeDef hi2c1;

int8_t TouchSense_Set_Configuration(uint16_t th1,uint16_t th2,uint16_t dur)
{
    uint8_t TransmitDatas[7];

    TransmitDatas[0] = F1822_CONFIG;  
    
    TransmitDatas[1] =  th1&0x00FF;     //Threshold of channel 0
    TransmitDatas[2] = (th1&0xFF00)>>8;

    TransmitDatas[3] =  th2&0x00FF;     //Threshold of channel 1
    TransmitDatas[4] = (th2&0xFF00)>>8;

    TransmitDatas[5] =  dur&0x00FF;     //Duration 
    TransmitDatas[6] = (dur&0xFF00)>>8;
    
    if(HAL_I2C_Master_Transmit_DMA(&hi2c1,TOUCH_ADDRESS,TransmitDatas,7)==HAL_ERROR)
        return -1;    
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);
        
    return 0;
}

int8_t TouchSense_Read_Status(void)
{
    uint8_t ReadRegister = F1822_STATUS;
    uint8_t Status = 0x00;

    if(HAL_I2C_Master_Transmit_DMA(&hi2c1,TOUCH_ADDRESS,&ReadRegister,1)==HAL_ERROR)
        return -1;    
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);

    if(HAL_I2C_Master_Receive_DMA(&hi2c1,TOUCH_ADDRESS,&Status,1)==HAL_ERROR)
        return 1;
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);
        
    return Status;
}
