#ifndef __CMDHANDLER_H
#define __CMDHANDLER_H

#include "main.h"
#include "usart.h"
#include <stdio.h>

// format defines
#define HANDLER_ID_INDEX     0
#define HANDLER_NUMBER_INDEX 3
#define HANDLER_UPDATE_INDEX 5
#define HANDLER_DATA_INDEX   7

#define HANDLER_ENABLE      'E'
#define HANDLER_DISABLE     'D'

// id defines
#define HANDLER_ADC0        "ADC0"
#define HANDLER_ADC1        "ADC1"
#define HANDLER_LED0        "LED0"
#define HANDLER_LED1        "LED1"
#define HANDLER_LED2        "LED2"
#define HANDLER_LED3        "LED3"
#define HANDLER_TMP0        "TMP0"
#define HANDLER_BTN0        "BTN0"
#define HANDLER_HLD0        "HLD0"
#define HANDLER_FTH0        "FTH0"

#define HANDLER_ADC_0 '0'
#define HANDLER_ADC_1 '1'

#define HANDLER_LED_BLUE    '0'
#define HANDLER_LED_RED     '1'
#define HANDLER_LED_ORANGE  '2'
#define HANDLER_LED_GREEN   '3'

#define HANDLER_JOY_UP      '0'

#define HANDLER_JOY_UP_HOLD '0'

#define HANDLER_TEMP_0      '0'

int Handler_ReadLed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *pData); // ToDo: rename/refactor
int CmdHandler_ParseCommand (uint8_t *pData, size_t size);

#endif
