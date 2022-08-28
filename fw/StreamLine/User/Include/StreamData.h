#ifndef __STREAMDATA_H
#define __STREAMDATA_H

#include <stdio.h>

/* Data template
uint8_t id[4] = 'ADC0'; ID + NUMBER
uint8_t sep1  = ',';
uint8_t upd   = 'E' or 'D'
uint8_t sep2  = ',';
uint8_t data[depends on type]
uint8_t end[2] = '\r' + '\n'
*/
#pragma pack(push, 1)
typedef struct
{
    uint8_t  id[4];
    uint8_t  sep1;
    uint8_t  upd;
    uint8_t  sep2;
    uint8_t  data[5];
    uint8_t  end[2];
} adcData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    uint8_t id[4];
    uint8_t sep1;
    uint8_t upd;
    uint8_t sep2;
    uint8_t data[3]; // 'ON\0' or 'OFF'
    uint8_t end[2];
} gpioData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    uint8_t id[4];
    uint8_t sep1;
    uint8_t upd;
    uint8_t sep2;
    uint8_t data[10]; // ToDo: increase
    uint8_t end[2];
} tempData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    adcData   adc0;
    adcData   adc1;
    gpioData  led0;
    gpioData  led1;
    gpioData  led2;
    gpioData  led3;
    gpioData  btn0;
    gpioData  hld0;
    tempData  tmp0;
    uint8_t   thrd[1024];
    uint8_t   end[50];
} dataStruct;
#pragma pack(pop)

#endif
