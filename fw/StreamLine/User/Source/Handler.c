#include "Handler.h"

static int Handler_ToogleLed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *pData)
{
    if (GPIOx == NULL || pData == NULL)
    {
        return -1;
    }

    // Implemented due inverted logic on demo board
    if (memcmp((char*)pData, (char*)HANDLER_ON, (size_t)STREAM_LED_DATA_LEN - 1) == 0)
    {
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
        return 0;
    }

    else if (memcmp((char*)pData, (char*)HANDLER_OFF, (size_t)STREAM_LED_DATA_LEN) == 0)
    {
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
        return 0;
    }

    return 1;
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
        memcpy(pData, (char*)HANDLER_OFF, STREAM_LED_DATA_LEN);
    }
    else
    {
        memcpy(pData, (char*)HANDLER_ON, STREAM_LED_DATA_LEN);
    }

    return 0;
}

static int Handler_ControlDataOutput (uint8_t *pUpdate, uint8_t update)
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

static void Handler_SendAckToHost (uint8_t *pId, uint8_t update, int status)
{
    memset(pTxAns, '-', sizeof(pTxAns));

    memcpy(pTxAns, pId, STREAM_ID_LEN);
    pTxAns[4] = ',';
    pTxAns[5] = update;
    pTxAns[6] = ',';

    if (status)
    {
        memcpy(&pTxAns[7], (char*)HANDLER_ERROR, HANDLER_ACK_LEN);
    }

    else
    {
        memcpy(&pTxAns[7], (char*)HANDLER_OK, HANDLER_ACK_LEN);
    }

    pTxAns[12] = '\r';
    pTxAns[13] = '\n';
    pTxAns[61] = '\r';
    pTxAns[62] = '\n';
    HAL_UART_Transmit_IT(&huart2, (uint8_t*)&pTxAns, sizeof(pTxAns));
}

int Handler_ParseCommand (uint8_t *pCommand, size_t size)
{
    if (pCommand == NULL || size != RECORD_SIZE)
    {
        return -1;
    }

    // Parse command
    uint8_t *pId   = &pCommand[HANDLER_ID_INDEX];
    uint8_t number = pCommand[HANDLER_NUMBER_INDEX];
    uint8_t update = pCommand[HANDLER_UPDATE_INDEX];
    uint8_t *pData = &pCommand[HANDLER_DATA_INDEX];

    int status = -1;

    switch (pId[0])
    {
        case 'A':
        {
            switch (number)
            {
                case HANDLER_ADC_0:
                {
                    status = Handler_ControlDataOutput(&sharedStreamData.adc0.upd, update);
                    Handler_SendAckToHost(pId, update, status);
                }
                break;

                case HANDLER_ADC_1:
                {
                    status = Handler_ControlDataOutput(&sharedStreamData.adc1.upd, update);
                    Handler_SendAckToHost(pId, update, status);
                }
                break;
            }
        }
        break;

        case 'L':
        {
            switch (number)
            {
                case HANDLER_LED_BLUE:
                {
                    status = Handler_ControlDataOutput(&sharedStreamData.led0.upd, update);
                    if (!status)
                    {
                        Handler_ToogleLed(LED_BLUE_GPIO_Port, LED_BLUE_Pin, pData);
                    }
                    Handler_SendAckToHost(pId, update, status);
                    return 0;
                }
                break;

                case HANDLER_LED_RED:
                {
                    status = Handler_ControlDataOutput(&sharedStreamData.led1.upd, update);
                    if (!status)
                    {
                        Handler_ToogleLed(LED_RED_GPIO_Port, LED_RED_Pin, pData);
                    }
                    Handler_SendAckToHost(pId, update, status);
                    return 0;
                }
                break;

                case HANDLER_LED_ORANGE:
                {
                    status = Handler_ControlDataOutput(&sharedStreamData.led2.upd, update);
                    if (!status)
                    {
                        Handler_ToogleLed(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin, pData);
                    }
                    Handler_SendAckToHost(pId, update, status);
                    return 0;
                }
                break;

                case HANDLER_LED_GREEN:
                {
                    status = Handler_ControlDataOutput(&sharedStreamData.led3.upd, update);
                    if (!status)
                    {
                        Handler_ToogleLed(LED_GREEN_GPIO_Port, LED_GREEN_Pin, pData);
                    }
                    Handler_SendAckToHost(pId, update, status);
                    return 0;
                }
                break;
            }
        }
        break;

        case 'T':
        {
            status = Handler_ControlDataOutput(&sharedStreamData.tmp0.upd, update);
            Handler_SendAckToHost(pId, update, status);
        }
        break;

        case 'B':
        {
            status = Handler_ControlDataOutput(&sharedStreamData.btn0.upd, update);
            Handler_SendAckToHost(pId, update, status);
        }
        break;

        case 'H':
        {
            status = Handler_ControlDataOutput(&sharedStreamData.hld0.upd, update);
            Handler_SendAckToHost(pId, update, status);
        }
        break;

        default:
        {
            // ToDo: Send Unsuported
        }
        break;
    }

    return -1;
}
