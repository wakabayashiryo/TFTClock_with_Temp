#include "PIC_I2C.h"

I2Cx_Configuration I2C1_Config;

int8_t I2C_Transmit(I2Cx_Module I2Cx,uint8_t address,uint8_t *pData,uint8_t size)
{
    int8_t result;
    
     switch(I2Cx)
    {
        case I2C1_MODULE:
            result = I2C1_Transmit(address,pData,size);
        break;
    }
    return result;
}

int8_t I2C_Receive(I2Cx_Module I2Cx,uint8_t address,uint8_t *pData,uint8_t size)
{
    int8_t result;
    
    switch(I2Cx)
    {
        case I2C1_MODULE:
             result = I2C1_Receive(address,pData,size);
        break;
    }
    return result;
}

void I2C_Interrupt(void)
{
    I2C1_Interrupt();
}