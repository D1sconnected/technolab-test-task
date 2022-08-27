#ifndef __CMDHANDLER_H
#define __CMDHANDLER_H

#include "main.h"
#include <stdio.h>

// format defines
#define HANDLER_ID_INDEX     0
#define HANDLER_NUMBER_INDEX 2
#define HANDLER_ENABLE_INDEX 4

// id defines
#define HANDLER_ADC 'A'
#define HANDLER_LED 'L'
#define HANDLER_TEMPERATURE 'T'
#define HANDLER_BTN 'H'
#define HANDLER_FTHREADS 'F'

// service defines
#define HANDLER_ADC_0 '0'
#define HANDLER_ADC_1 '1'

#define HANDLER_LED_BLUE    '0'
#define HANDLER_LED_RED     '1'
#define HANDLER_LED_ORANGE  '2'
#define HANDLER_LED_GREEN   '3'

int CmdHandler_ParseCommand (uint8_t *pData, size_t size);

#endif
