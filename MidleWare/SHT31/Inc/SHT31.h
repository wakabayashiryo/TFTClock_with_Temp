/* 
 * File:   SHT31.h
 * Author: evaota
 *
 * Created on 2017/08/02, 23:00
 */

#ifndef SHT31_H
#define	SHT31_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "main.h"

#define SHT31_ADDRESS 0x0044<<1

#define SHT31_MEAS_HIGHREP_STRETCH 0x2C06
#define SHT31_MEAS_MEDREP_STRETCH  0x2C0D
#define SHT31_MEAS_LOWREP_STRETCH  0x2C10
#define SHT31_MEAS_HIGHREP         0x2400
#define SHT31_MEAS_MEDREP          0x240B
#define SHT31_MEAS_LOWREP          0x2416
#define SHT31_READSTATUS           0xF32D
#define SHT31_CLEARSTATUS          0x3041
#define SHT31_SOFTRESET            0x30A2
    
#define SHT31_HEATEREN             0x306D
#define SHT31_HEATERDIS            0x3066

int8_t SHT31_Init(void);

int8_t SHT31_Read_Data(void);

float SHT31_Get_Temperature(void);
float SHT31_Get_Humidity(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SHT31_H */

