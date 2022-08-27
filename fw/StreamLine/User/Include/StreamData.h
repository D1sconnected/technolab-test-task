#ifndef __STREAMDATA_H
#define __STREAMDATA_H

#include <stdio.h>

typedef struct
{
    uint8_t  id;
    uint8_t  number;
    uint8_t  data[5]; // 0.000 - 3.300
    uint8_t  end;
} adcData;

typedef struct
{
    uint8_t id;
    uint8_t number;
    uint8_t value;   // 0 - off, 1 - on
    uint8_t end;
} gpioData;

typedef struct
{
    uint8_t id;
    uint8_t number;
    uint8_t data[10]; // -30.000000000 - 30.000000000
    uint8_t end;
} tempData;

typedef struct
{
    uint8_t id;
    uint8_t number;
    uint8_t data[1024];
    uint8_t end;
} fthrData;

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
    fthrData  thr0;
} dataStruct;

#endif
