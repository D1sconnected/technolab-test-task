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
    memset(&pTxAns, '-', sizeof(pTxAns));

    pTxAns[0] = id;
    pTxAns[1] = ',';
    pTxAns[2] = number;
    pTxAns[3] = ',';
    pTxAns[4] = update;
    pTxAns[5] = ',';

    if (status)
    {
        pTxAns[6]  = 'E';
        pTxAns[7]  = 'R';
        pTxAns[8]  = 'R';
        pTxAns[9]  = 'O';
        pTxAns[10] = 'R';
    }

    else
    {
        pTxAns[6]  = 'O';
        pTxAns[7]  = 'K';
        pTxAns[8]  = 0;
        pTxAns[9]  = 0;
        pTxAns[10] = 0;
    }

    pTxAns[11] = '\r';
    pTxAns[12] = '\n';
    pTxAns[61] = '\r';
    pTxAns[62] = '\n';
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
                    status = CmdHandler_ControlDataOutput(&sharedStreamData.led0.upd, update);
                    if (!status)
                    {
                        CmdHandler_ToogleLed(LED_BLUE_GPIO_Port, LED_BLUE_Pin, value);
                    }
                    CmdHandler_SendAckToHost(id, number, update, status);
                    return 0;
                }
                break;

                case HANDLER_LED_RED:
                {
                    status = CmdHandler_ControlDataOutput(&sharedStreamData.led1.upd, update);
                    if (!status)
                    {
                        CmdHandler_ToogleLed(LED_RED_GPIO_Port, LED_RED_Pin, value);
                    }
                    CmdHandler_SendAckToHost(id, number, update, status);
                    return 0;
                }
                break;

                case HANDLER_LED_ORANGE:
                {
                    status = CmdHandler_ControlDataOutput(&sharedStreamData.led2.upd, update);
                    if (!status)
                    {
                        CmdHandler_ToogleLed(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin, value);
                    }
                    CmdHandler_SendAckToHost(id, number, update, status);
                    return 0;
                }
                break;

                case HANDLER_LED_GREEN:
                {
                    status = CmdHandler_ControlDataOutput(&sharedStreamData.led3.upd, update);
                    if (!status)
                    {
                        CmdHandler_ToogleLed(LED_GREEN_GPIO_Port, LED_GREEN_Pin, value);
                    }
                    CmdHandler_SendAckToHost(id, number, update, status);
                    return 0;
                }
                break;
            }
        }
        break;

        case HANDLER_TEMPERATURE:
        {
            status = CmdHandler_ControlDataOutput(&sharedStreamData.tmp0.upd, update);
            CmdHandler_SendAckToHost(id, number, update, status);
        }
        break;

        case HANDLER_BTN:
        {
            status = CmdHandler_ControlDataOutput(&sharedStreamData.btn0.upd, update);
            CmdHandler_SendAckToHost(id, number, update, status);
        }
        break;

        case HANDLER_HOLD:
        {
            status = CmdHandler_ControlDataOutput(&sharedStreamData.hld0.upd, update);
            CmdHandler_SendAckToHost(id, number, update, status);
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
