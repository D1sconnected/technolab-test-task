#include "CmdHandler.h"

static int CmdHandler_ToogleLed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t enable)
{
    if (GPIOx == NULL)
    {
        return -1;
    }

    // Implemented due inverted logic on demo board
    if (enable)
    {
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
    }

    else
    {
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
    }

    return 0;
}

int Handler_ReadLed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *pData)
{
    if (GPIOx == NULL)
    {
        return -1;
    }

    GPIO_PinState state = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

    // Implemented due inverted logic on demo board
    if (state)
    {
        *pData = '0';
    }
    else
    {
        *pData = '1';
    }

    return 0;
}

int CmdHandler_ParseCommand (uint8_t *pData, size_t size)
{
    if (pData == NULL || size < 7)
    {
        return -1;
    }

    static uint8_t id     = 0;
    static uint8_t number = 0;
    static uint8_t update = 0;
    static uint8_t value  = 0;

    // Parse command
    id     = pData[HANDLER_ID_INDEX];
    number = pData[HANDLER_NUMBER_INDEX];
    update = pData[HANDLER_UPDATE_INDEX];
    value = (uint8_t)atoi(&pData[HANDLER_VALUE_INDEX]);

    switch (id)
    {
        case HANDLER_ADC:
        {
            switch (number)
            {
                case HANDLER_ADC_0:
                {

                }
                break;

                case HANDLER_ADC_1:
                {

                }
                break;
            }
        }
        break;

        case HANDLER_LED:
        {
            switch (number)
            {
                case HANDLER_LED_BLUE:
                {
                    CmdHandler_ToogleLed(LED_BLUE_GPIO_Port, LED_BLUE_Pin, value);
                    return 0;
                }
                break;

                case HANDLER_LED_RED:
                {
                    CmdHandler_ToogleLed(LED_RED_GPIO_Port, LED_RED_Pin, value);
                    return 0;
                }
                break;

                case HANDLER_LED_ORANGE:
                {
                    CmdHandler_ToogleLed(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin, value);
                    return 0;
                }
                break;

                case HANDLER_LED_GREEN:
                {
                    CmdHandler_ToogleLed(LED_GREEN_GPIO_Port, LED_GREEN_Pin, value);
                    return 0;
                }
                break;
            }
        }
        break;

        case HANDLER_TEMPERATURE:
        {

        }
        break;

        case HANDLER_BTN:
        {

        }
        break;

        case HANDLER_FTHREADS:
        {

        }
        break;

        default:
        {

        }
        break;
    }

    return -1;
}
