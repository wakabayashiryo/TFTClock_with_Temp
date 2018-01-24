#include "SHT31.h"

static uint8_t Data_array[6];

I2C_HandleTypeDef hi2c1;

static int8_t SHT31_Write_Command(uint16_t data)
{
    int8_t result = 0;
    uint8_t data_arr[2];
    
    data_arr[0] = (data&0xFF00)>>8;
    data_arr[1] = data&0x00FF;
    if(HAL_I2C_Master_Transmit_DMA(&hi2c1,SHT31_ADDRESS,data_arr,2)==HAL_ERROR)
        result = 1;
        
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);
    
    return result;
}

int8_t SHT31_Init(void)
{
    int8_t result = 0;
    
    result += SHT31_Write_Command(SHT31_SOFTRESET);
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);
        
    result += SHT31_Write_Command(SHT31_CLEARSTATUS);
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);
    
    result += SHT31_Write_Command(SHT31_HEATERDIS);
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);
    
    return result;
}

int8_t SHT31_Read_Data(void)
{
    int8_t result = 0;    

    SHT31_Write_Command(SHT31_MEAS_HIGHREP_STRETCH);//Start Mesurement Teperature and Humidity

    if(HAL_I2C_Master_Receive_DMA(&hi2c1,SHT31_ADDRESS,Data_array,6)==HAL_ERROR)
        result = 1;
    
    while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);
        
    return result;
}

float SHT31_Get_Temperature(void)
{
    uint16_t temp16 = (Data_array[0]<<8)|Data_array[1];
    return ((float)temp16/65535.f)*175.f-45.f;
}

float SHT31_Get_Humidity(void)
{
    uint16_t humid16 = (Data_array[3]<<8)|Data_array[4];
    return (float)humid16/65535.f*100.f;
}