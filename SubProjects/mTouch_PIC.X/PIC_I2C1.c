#include "PIC_I2C.h"

//Check idle status use R_nW,SEN,RSEN,PEN,RCEN,ACKEN 
#define I2C1_IdleCheck() while((SSPCON2&0x1F)|(SSPSTAT&0x05))

extern I2Cx_Configuration I2C1_Config;

void I2C1_Init(I2C1_Init_PORTTypedef I2C1_Init_PORT,uint8_t address,I2Cx_Mode mode,I2Cx_Speed Speed)
{
    I2C1_Init_PORT();                       //Intialize Port for I2C
    I2C1_Config.Mode = mode;                //Save I2C1 mode

    switch(mode)
    {
        case I2C_MODE_MASTER:
            SSPCON1 |= (0x08<<0);          //I2C mode is master
            if(Speed==I2C_SPEED_STANDARD)
            {
                SSPSTAT |= (1<<7);         //Slew rate control disabled for standard speed mode
                SSPADD = 0x4F;             //Set baud rate(100kHz)
            }
            else if(Speed==I2C_SPEED_HIGH)
            {
                SSPSTAT &= ~(1<<7);        //Slew rate control enabled for high speed mode 
                SSPADD = 0x13;             //Set baud rate(400kHz)
            }
            SSPCON1bits.SSPEN = 1;         //Enable I2C
        break;
        
        case I2C_MODE_SLAVE:
            SSPSTAT = 0x00;                //Clear all flag
            if(Speed==I2C_SPEED_STANDARD)
                SSPSTAT |= (1<<7);         //Slew rate control disabled for standard speed mode
            else if(Speed==I2C_SPEED_HIGH)
                SSPSTAT &= ~(1<<7);        //Slew rate control enabled for high speed mode 
            
            SSPCON1 |= (0x06<<0);          //I2C slave mode 7bit                
            SSPCON2bits.SEN= 1;            //clock streching is enable
            SSPADD = address << 1;         //Set own address
            SSPMSK = 0xFE;                 //Compared on address bit from 1bit to 7bit
            SSPCON1bits.SSPEN = 1;         //Enable I2C
        break;
    }
    SSP1IE = 1;                             //Enable SSP interrupt
    BCL1IE = 1;                             //Enable Bus collisioni interrupt
    SSP1IF = 0;                             //Clear SSP interrupt flag
    BCL1IF = 0;                             //Clear Bus collisioni interrupt flag

    PEIE = 1;
    GIE = 1;
}

int8_t I2C1_Transmit(uint8_t address,uint8_t *pData,uint8_t size)
{
    int8_t result;

    switch(I2C1_Config.Mode)
    {
        case I2C_MODE_MASTER:
            I2C1_IdleCheck();               //Waitting bus will become idle status 
            SSPCON2bits.SEN = 1;           //Issue Start Condition

            I2C1_IdleCheck();               //Waitting bus will become idle status 
            SSPBUF = address << 1;         //Send Slave address
            while(SSPSTATbits.R_nW);       //Waitting to send address 

            result = SSPCON2bits.ACKSTAT;     //Check acknowledge from slave
            if (!result)                       //if Slave send acknowledge to master
            {
                for(uint8_t i=0;i<size;i++)        //Send data sequence
                {
                    I2C1_IdleCheck();       //Waitting bus will become idle status  
                    SSPBUF = *(pData+i);   //send Data to slave
                    while(SSPSTATbits.R_nW);//Waitting to send data 

                    result = SSPCON2bits.ACKSTAT;//Check acknowledge from slave
                    if (result)                // if master revceived Not acknowledge
                    {
                        result = 2;
                        break;
                    }
                }
            }

            I2C1_IdleCheck();               //Waitting bus will become idle status 
            SSPCON2bits.PEN = 1;           //Issue Stop condition
        break;
        
        case I2C_MODE_SLAVE:
            if((SSPSTATbits.S==0)&&(SSPSTATbits.P==1))
            {
                for(uint8_t i = 0;i<size;i++)
                    I2C1_Config.SlaveTransmitBuff[i] = pData[i];
                result = size;
            }
            else
                result = 0;
        break;
    }
    
    return result;
}

int8_t I2C1_Receive(uint8_t address,uint8_t *pData,uint8_t size)
{
    int8_t result;

    switch(I2C1_Config.Mode)
    {
        case I2C_MODE_MASTER:
            I2C1_IdleCheck();               //Waitting bus will become idle status
            SSPCON2bits.SEN = 1;           //Issue Start Condition

            I2C1_IdleCheck();               //Waitting bus will become idle status
            SSPBUF = ((address << 1)|0x01);//Sned address to slave
            while(SSPSTATbits.R_nW);       //Waitting to send address 
            
            result = SSPCON2bits.ACKSTAT;     //Check acknowledge from slave
            if (!result)                       // if master revceived Not acknowledge
            {
                for (uint8_t i=0;i<size;i++) 
                {
                    I2C1_IdleCheck();       //Waitting bus will become idle status
                    SSPCON2bits.RCEN = 1;  //Enable Receive (wait sendding data from slave)
                    while(SSPCON2bits.RCEN);//wait to receive Data
                    *(pData+i) = SSPBUF;   //Read Data that sent from slave
                    I2C1_IdleCheck();       //Waitting bus will become idle status
                    if (i == size-1) 
                        SSPCON2bits.ACKDT = 1;//acknowledge will send to slave is not acknowledge  
                    else 
                        SSPCON2bits.ACKDT = 0;//acknowledge will send to slave is acknowledge  
                    SSPCON2bits.ACKEN = 1;    //send acknowledge 
                }
            }

            I2C1_IdleCheck();                  //Waitting bus will become idle status
            SSPCON2bits.PEN = 1;              //Issue Start Condition
        break;
        
        case I2C_MODE_SLAVE:
            if((SSPSTATbits.S==0)&&(SSPSTATbits.P==1))
            {
                for(uint8_t i = 0;i<size;i++)
                     pData[i] = I2C1_Config.SlaveReceiveBuff[i];
                result = I2C1_Config.ReceivePoint - I2C1_Config.SlaveReceiveBuff;
            }            
        break;
    }
    
    return result;
}

void I2C1_Interrupt(void) 
{
    if(SSP1IF&&SSP1IE)
    {   //Check Received data is data or address
        if(SSPSTATbits.D_nA)           //Received Data from master
        {
            if(SSPSTATbits.R_nW)    //Read responce
            {
                if(!SSPCON2bits.ACKSTAT)   //If Number of Read Data is 2 over
                {
                    while((SSPCON1bits.CKP)|(SSPSTATbits.BF));
                    SSPBUF = *I2C1_Config.TransmitPoint;       //Set Data to buffer
                    I2C1_Config.TransmitPoint++;            //increment buffer pointer
                    SSPCON1bits.CKP = 1;           //return acknowledge to master
                }
            }
            else        //Write responce
            {
                *I2C1_Config.ReceivePoint = SSPBUF;    //Read Data from buffer
                I2C1_Config.ReceivePoint++;         //increment buffer pointer
                SSPCON1bits.CKP = 1;           //return acknowledge to master
            }
        }   
        else if(!SSPSTATbits.D_nA)         //Received Address from master
        {
            SSPBUF;            //Access buffer
            
            if(SSPSTATbits.R_nW)       //Read resopnce
            {   
                I2C1_Config.TransmitPoint = I2C1_Config.SlaveTransmitBuff;      //Set first buffer pointer

                while((SSPCON1bits.CKP)|(SSPSTATbits.BF));
                SSPBUF = *I2C1_Config.TransmitPoint;           //Set Data to buffer
                I2C1_Config.TransmitPoint++;                //increment buffer pointer
            }
            else            //Write resopnce
            {
                I2C1_Config.ReceivePoint = I2C1_Config.SlaveReceiveBuff;        //Set first buffer pointer
            }
            SSPCON1bits.CKP = 1;           //return acknowledge to master
        } 
        SSP1IF = 0;
    }

    if(BCL1IE&&BCL1IF)      //If occured bus collision
        BCL1IF = 0;
}