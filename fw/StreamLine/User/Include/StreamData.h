#ifndef __STREAMDATA_H
#define __STREAMDATA_H

#include <stdio.h>

typedef struct
{
    uint8_t  id;
    uint8_t  sep1;
    uint8_t  number;
    uint8_t  sep2;
    uint8_t  data[5]; // 0.000 - 3.300
    uint8_t  end[2];
} adcData;

typedef struct
{
    uint8_t id;
    uint8_t sep1;
    uint8_t number;
    uint8_t sep2;
    uint8_t data;   // 0 - off, 1 - on
    uint8_t end[2];
} gpioData;

typedef struct
{
    uint8_t id;
    uint8_t sep1;
    uint8_t number;
    uint8_t sep2;
    uint8_t data[10]; // -30.000000000 - 30.000000000
    uint8_t end[2];
} tempData;

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
} dataStruct;
#pragma pack(pop)

#endif
