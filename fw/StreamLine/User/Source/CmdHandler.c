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

static int CmdHandler_ControlDataOutput (uint8_t *pUpdate, uint8_t update)
{
    if (pUpdate == NULL || (update != HANDLER_ENABLE && update != HANDLER_DISABLE))
    {
        return -1;
    }

    switch (update)
    {
        case HANDLER_ENABLE:
        {
            *pUpdate = HANDLER_ENABLE;
        }
        break;

        case HANDLER_DISABLE:
        {
            *pUpdate = HANDLER_DISABLE;
        }
        break;
    }

    return 0;
}

static void CmdHandler_SendAckToHost (uint8_t id, uint8_t number, uint8_t update, int status)
{
    memset(&pTxAns, 0, sizeof(pTxAns));

    pTxAns[0] = id;
    pTxAns[1] = ',';
    pTxAns[2] = number;
    pTxAns[3] = ',';
    pTxAns[4] = update;
    pTxAns[5] = ',';

    if (status)
    {
        pTxAns[6] = 'E';
    }

    else
    {
        pTxAns[6] = 'O';
    }

    pTxAns[7] = '\r';
    pTxAns[8] = '\n';

    HAL_UART_Transmit_IT(&huart2, (uint8_t*)&pTxAns, sizeof(pTxAns));
}

int CmdHandler_ParseCommand (uint8_t *pData, size_t size)
{
    if (pData == NULL || size != RECORD_SIZE)
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

    int status = -1;

    switch (id)
    {
        case HANDLER_ADC:
        {
            switch (number)
            {
                case HANDLER_ADC_0:
                {
                    status = CmdHandler_ControlDataOutput(&sharedStreamData.adc0.upd, update);
                    CmdHandler_SendAckToHost(id, number, update, status);
                }
                break;

                case HANDLER_ADC_1:
                {
                    status = CmdHandler_ControlDataOutput(&sharedStreamData.adc1.upd, update);
                    CmdHandler_SendAckToHost(id, number, update, status);
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
