/* 
 * File:   Timer0.h
 * Author: evaota
 *
 * Created on 2017/08/25, 12:21
 */

#ifndef PIC_TIMER0_H
#define	PIC_TIMER0_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "main.h" 
    
typedef enum
{
    TMR0_PRESCALER2 = 0,
    TMR0_PRESCALER4 = 1,
    TMR0_PRESCALER8 = 2,
    TMR0_PRESCALER16 = 3,
    TMR0_PRESCALER32 = 4,
    TMR0_PRESCALER64 = 5,
    TMR0_PRESCALER128 = 6,
    TMR0_PRESCALER256 = 7,
}TMR0_PRESCALER_VAL;
    
void Timer0_Init(TMR0_PRESCALER_VAL pre_val,uint8_t reload_val);
uint8_t Timer0_CheckFlag(void);


#ifdef	__cplusplus
}
#endif

#endif	/* TIMER0_H */
