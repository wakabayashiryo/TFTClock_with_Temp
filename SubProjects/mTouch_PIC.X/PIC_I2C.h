/*
 * File:   PIC_I2C.h
 * Author: evaota
 *
 * Created on 2017/03/5, 14:24
 */
 
#ifndef PIC_I2C_H
#define	PIC_I2C_H

#include "main.h"

#define I2C1_DONOT_USE 0

#define SLAVE_RECEIVE_MAX 8

typedef enum
{
    I2C_MODE_MASTER,
    I2C_MODE_SLAVE
}I2Cx_Mode;

typedef enum
{
    I2C_SPEED_STANDARD,
    I2C_SPEED_HIGH,
}I2Cx_Speed;

typedef enum
{
    I2C1_MODULE,
    I2C2_MODULE
}I2Cx_Module;

typedef struct 
{
    uint8_t SlaveTransmitBuff[SLAVE_RECEIVE_MAX];
    uint8_t SlaveReceiveBuff[SLAVE_RECEIVE_MAX];
    uint8_t *TransmitPoint;
    uint8_t *ReceivePoint;
    I2Cx_Mode Mode;
}I2Cx_Configuration;


typedef void (*I2C1_Init_PORTTypedef)(void);
typedef void (*I2C2_Init_PORTTypedef)(void);

void I2C1_Init(I2C1_Init_PORTTypedef I2C1_Init_PORT,uint8_t address,I2Cx_Mode mode,I2Cx_Speed Speed);
void I2C1_Interrupt(void);
int8_t I2C1_Transmit(uint8_t address,uint8_t *pData,uint8_t size);
int8_t I2C1_Receive(uint8_t address,uint8_t *pData,uint8_t size);

int8_t I2C_Transmit(I2Cx_Module I2Cx,uint8_t address,uint8_t *pData,uint8_t size);
int8_t I2C_Receive(I2Cx_Module I2Cx,uint8_t address,uint8_t *pData,uint8_t size);
void I2C_Interrupt(void);

#endif	/* I2C_H */

