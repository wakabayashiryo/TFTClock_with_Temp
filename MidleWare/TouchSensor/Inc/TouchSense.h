/* 
 * File:   TouchSensor.h
 * Author: evaota
 *
 * Created on 2017/12/15, 21:09
 */

#ifndef TOUCHSENSE_H_
#define	TOUCHSENSE_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "main.h"

#define TOUCH_ADDRESS (0x56<<1)

#define F1822_CONFIG   0x33
#define F1822_VALUE    0x44

int8_t TouchSense_Set_Configuration(uint16_t th1,uint16_t th2);
int8_t TouchSense_Read_Value(void);
uint16_t TouchSense_Get_Value1(void);
uint16_t TouchSense_Get_Value2(void);


#ifdef	__cplusplus
}
#endif

#endif	/* TOUCHSENSE_H_ */

