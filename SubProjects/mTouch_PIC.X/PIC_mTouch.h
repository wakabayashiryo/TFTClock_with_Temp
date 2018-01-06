/* 
 * File:   mTouch.h
 * Author: evaota
 *
 * Created on 2017/04/15, 11:06
 */

#ifndef PIC_MTOUCH_H
#define	PIC_MTOUCH_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "main.h"
    
typedef struct
{
    uint8_t  CPS_Channel;
    int16_t  Threshould_Diff;
    uint16_t CPS_Data;
    uint16_t CPS_PreData;
    uint8_t  Check_Interval;
}CPS_CHANNELCONFIG;
    
#define CPS_SELECT_CHANNEL(x) CPSCON1 = (uint8_t)x
#define CPS_ENABLE()   CPSCON0 |= (0x01<<7);       //CPS module is enabled
#define CPS_DISABLE()  CPSCON0 &= ~(0x01<<7);      //CPS module is disabled

typedef void (*mTouch_Init_PORTTypedef)(void);

void mTouch_Init(mTouch_Init_PORTTypedef mTouch_PortInit,CPS_CHANNELCONFIG *cps_config,uint8_t num_of_channel);

void mTouch_Scan(void);
void mTouch_Count1ms(void);
uint8_t mTouch_Check(uint8_t elements);

void mTouch_Display_Config(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MTOUCH_H */

