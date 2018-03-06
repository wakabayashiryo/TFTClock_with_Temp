/*
 * File:   main.c
 * Author: evaota
 *
 * Created on 2017/12/17, 11:48
 */

#include "main.h"

void Basic_Init(void);
void I2C_Init_Port(void);
void mTouch_Init_Port(void);


void main(void)
{
    uint16_t value_ch1 = 0;
    uint16_t value_ch2 = 0;
    uint8_t Register[5];
    
    Basic_Init();
    
    I2C1_Init(I2C_Init_Port,0x56,I2C_MODE_SLAVE,I2C_SPEED_STANDARD);
            
    Timer0_Init(TMR0_PRESCALER64,125);//set interrupt time is 1ms
    
    CPS_CHANNELCONFIG cps_conf[2];
    cps_conf[0].CPS_Channel = 0;
    cps_conf[0].Threshould_Diff = 500;     
    cps_conf[1].CPS_Channel = 3;
    cps_conf[1].Threshould_Diff = 500;     
    mTouch_Init(mTouch_Init_Port,cps_conf,2);
    
    __delay_ms(100);
    
    while(1)
    {        
        I2C_Receive(I2C1_MODULE,0x00,Register,5);
        
        switch(Register[0])
        {
            case 0x33:
                cps_conf[0].Threshould_Diff = (uint16_t)((Register[1]<<8)|Register[2]);
                cps_conf[1].Threshould_Diff = (uint16_t)((Register[3]<<8)|Register[4]);
            break;
            
            case 0x44:    
                value_ch1 = mTouch_Get_RawValue(0);
                value_ch2 = mTouch_Get_RawValue(1);

                Register[1] = (value_ch1&0xFF00)>>8;
                Register[2] = (value_ch1&0x00FF);
                Register[3] = (value_ch2&0xFF00)>>8;
                Register[4] = (value_ch2&0x00FF);

                I2C_Transmit(I2C1_MODULE,0x56,Register,5);
            break;
                
            default:
                break;
        }

        if(mTouch_Check(0)||mTouch_Check(1))
            LATA5 = 0;
        else 
            LATA5 = 1;
    }
}

void Basic_Init(void)
{
    OSCCON = 0xF0;      //PLL ON: 
                        //internal Oscilltor is 32MHz
                        //clock source is internal one.
    
    TRISA = 0x00;       //All PORTA is output 
    ANSELA = 0x00;      //All PORTA  is digital
    WPUA = 0x00;        
    
    /*alternate pin fuction control*/
    APFCON = 0x00;     //no alteration
    
    PORTA = 0x00;       //clear
    
}

void interrupt Handler(void)
{
    static uint16_t Interval = 0,led=0;
    
    I2C_Interrupt();
    
    if(Timer0_CheckFlag())
    {
        mTouch_Count1ms();   

        if(++Interval>30)//Scanning Timer1 ever 30ms
        {
            Interval = 0;
            mTouch_Scan();
        }
    }
}


void I2C_Init_Port(void)
{
    TRISA |= (3<<1);
    ANSELA &= ~(3<<1);
}

void mTouch_Init_Port(void)
{
    TRISA   |= 0x11;        //RA0 is used as Channel0,1 of CPS.
    ANSELA  |= 0x09;        //RA4 is set as AN3
}
