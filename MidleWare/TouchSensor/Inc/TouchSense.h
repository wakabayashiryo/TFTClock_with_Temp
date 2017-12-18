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

#define F1822_CONFIG 0x33
#define F1822_STATUS   0x44

int8_t TouchSense_Set_Configuration(uint16_t th1,uint16_t th2,uint16_t dur);
int8_t TouchSense_Read_Status(void);


#ifdef	__cplusplus
}
#endif

#endif	/* TOUCHSENSE_H_ */

