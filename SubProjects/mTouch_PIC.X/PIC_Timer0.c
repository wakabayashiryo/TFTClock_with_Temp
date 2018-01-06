#include "PIC_Timer0.h"

static uint8_t TMR0_ReloadTemp;

void Timer0_Init(TMR0_PRESCALER_VAL pre_val,uint8_t reload_val)
{
    OPTION_REG &= ~(1<<5);  //Clock source is internal clock(FOSC/4)
    OPTION_REG &= ~(1<<3);  //Assign Prescaler to Timer0 module
    OPTION_REG &= ~(7<<0);  //Clear Prescaler value
    OPTION_REG |=  ((uint8_t)pre_val<<0);  //Set prescaler value
    
    TMR0_ReloadTemp = 0xFF - reload_val;
    TMR0 = TMR0_ReloadTemp;   //Clear 
    
    TMR0IF = 0;
    TMR0IE = 1;
    PEIE = 1;
    GIE = 1;
}

uint8_t Timer0_CheckFlag(void)
{
    if(TMR0IE&&TMR0IF)
    {
        TMR0 = TMR0_ReloadTemp;
        TMR0IF = 0;
        return 1;
    }
    return 0;
}
