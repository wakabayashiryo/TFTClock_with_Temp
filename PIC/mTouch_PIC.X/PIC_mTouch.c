#include "PIC_mTouch.h"

static CPS_CHANNELCONFIG *config_ptr;
static uint8_t using_channel;

void mTouch_Init(mTouch_Init_PORTTypedef mTouch_PortInit,CPS_CHANNELCONFIG *cps_config,uint8_t num_of_channel)
{
    mTouch_PortInit();
    
    config_ptr = cps_config;
    using_channel = num_of_channel; 
   
    /*Timer1 Configration*/
    T1CON |= (0x01<<2);     //Not synchronize with system clock
    T1CON &= ~(0x01<<3);    //LP Oscirator disable
    T1CON |= (0x00<<4);     //Prescaler Rate is 1
    T1CON |= (0x03<<6);     //Clock source is CPS
    
    T1GCON = 0x00;       
    
    TMR1 = 0x0000;
    
    /*CPS Configration*/
    CPSCON0 |= (0x03<<2);       //SPS range is 18uA
    CPSCON0 |= (0x00<<6);       //CPS use Reference Voltage 
    CPS_DISABLE();
    CPS_SELECT_CHANNEL(0x00);
    
    T1CON |= (0x01<<0);         //Timer1 Enable
    
}

void mTouch_Scan(void)
{
    static uint8_t elements = 0;
    
    config_ptr[elements].CPS_PreData = config_ptr[elements].CPS_Data;

    CPS_DISABLE();
    
    config_ptr[elements].CPS_Data = TMR1;
    TMR1 = 0x0000;
    
    CPS_ENABLE();

    if(++elements>=using_channel)elements = 0;    
    
    CPS_SELECT_CHANNEL(config_ptr[elements].CPS_Channel);
}

void mTouch_Count1ms(void)
{    
    for(uint8_t elements=0;elements<using_channel;elements++)
    {
        if(config_ptr[elements].Check_Interval)
            config_ptr[elements].Check_Interval--;
    }
}

uint8_t mTouch_Check(uint8_t elements)
{
    if(using_channel<=elements)return 0;

    int16_t DifferValue = (int16_t)(config_ptr[elements].CPS_PreData-config_ptr[elements].CPS_Data);

    if((DifferValue>config_ptr[elements].Threshould_Diff)&&!config_ptr[elements].Check_Interval)
    {
        config_ptr[elements].Check_Interval = 100;
        return 1;
    }
    return 0;
}

void mTouch_Display_Config(void)
{
    for(uint8_t elements =0; elements<using_channel;elements++)
    {
        printf("Channel:%d ",config_ptr[elements].CPS_Channel);
        printf("Diff=%000005d",(int16_t)(config_ptr[elements].CPS_PreData-config_ptr[elements].CPS_Data));
    }
    printf("\n");
}