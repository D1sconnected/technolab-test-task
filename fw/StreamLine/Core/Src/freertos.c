/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define V25 0.76
#define AVG_SLOPE .0025
#define VSENSE 3.3/4096
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
char pRxBuf[1024] = {0};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId CmdHandlerTaskHandle;
osThreadId StreamDataTaskHandle;
osThreadId ReadGpioTaskHandle;
osThreadId ReadAdcTaskHandle;
osThreadId ReadTempTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask_CmdHandler(void const * argument);
void StartTask_StreamData(void const * argument);
void StartTask_ReadGpio(void const * argument);
void StartTask_ReadAdc(void const * argument);
void StartTask_ReadTemp(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of CmdHandlerTask */
  osThreadDef(CmdHandlerTask, StartTask_CmdHandler, osPriorityNormal, 0, 128);
  CmdHandlerTaskHandle = osThreadCreate(osThread(CmdHandlerTask), NULL);

  /* definition and creation of StreamDataTask */
  osThreadDef(StreamDataTask, StartTask_StreamData, osPriorityHigh, 0, 128);
  StreamDataTaskHandle = osThreadCreate(osThread(StreamDataTask), NULL);

  /* definition and creation of ReadGpioTask */
  osThreadDef(ReadGpioTask, StartTask_ReadGpio, osPriorityNormal, 0, 128);
  ReadGpioTaskHandle = osThreadCreate(osThread(ReadGpioTask), NULL);

  /* definition and creation of ReadAdcTask */
  osThreadDef(ReadAdcTask, StartTask_ReadAdc, osPriorityNormal, 0, 128);
  ReadAdcTaskHandle = osThreadCreate(osThread(ReadAdcTask), NULL);

  /* definition and creation of ReadTempTask */
  osThreadDef(ReadTempTask, StartTask_ReadTemp, osPriorityNormal, 0, 128);
  ReadTempTaskHandle = osThreadCreate(osThread(ReadTempTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
//    snprintf(pRxBuf, sizeof("-----------------------------------------\n\r"), "%s", "-----------------------------------------\n\r");
    /* Infinite loop */
    for(;;)
    {
//        //vTaskList(&pRxBuf[43]);
//        //HAL_UART_Transmit_IT(&huart2, (uint8_t*)pRxBuf, sizeof(pRxBuf));
//        int status = 0;
//        memset(pRxBuf, 0, sizeof(pRxBuf));
//        status = CircularBuffer_Get(pCirBuf, pRxBuf);
//        if (status)
//        {
//            HAL_UART_Transmit_IT(&huart2, (uint8_t*)pRxBuf, sizeof(pRxBuf));
//        }

        osDelay(1000);
    }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask_CmdHandler */
/**
* @brief Function implementing the CmdHandlerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_CmdHandler */
void StartTask_CmdHandler(void const * argument)
{
  /* USER CODE BEGIN StartTask_CmdHandler */
  /* Infinite loop */
  for(;;)
  {
      // Get command
      uint8_t pTemp[6] = {0};
      int status = CircularBuffer_Get(pCirBuf, pTemp);

      // If exist -> parse command -> enable/disable stuff
      if (status)
      {
          CmdHandler_ParseCommand(pTemp, sizeof(pTemp));
      }
      osDelay(1000);
  }
  /* USER CODE END StartTask_CmdHandler */
}

/* USER CODE BEGIN Header_StartTask_StreamData */
/**
* @brief Function implementing the StreamDataTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_StreamData */
void StartTask_StreamData(void const * argument)
{
  /* USER CODE BEGIN StartTask_StreamData */
  /* Infinite loop */
  for(;;)
  {
      lockData = true;
      vTaskList(sharedStreamData.thrd);
      HAL_UART_Transmit_IT(&huart2, (uint8_t*)&sharedStreamData, sizeof(sharedStreamData));
      osDelay(1000);
  }
  /* USER CODE END StartTask_StreamData */
}

/* USER CODE BEGIN Header_StartTask_ReadGpio */
/**
* @brief Function implementing the ReadGpioTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_ReadGpio */
void StartTask_ReadGpio(void const * argument)
{
  /* USER CODE BEGIN StartTask_ReadGpio */
  /* Infinite loop */
  for(;;)
  {
      if (!lockData)
      {
          Handler_ReadLed(LED_BLUE_GPIO_Port, LED_BLUE_Pin, &sharedStreamData.led0.data);
          Handler_ReadLed(LED_RED_GPIO_Port, LED_RED_Pin, &sharedStreamData.led1.data);
          Handler_ReadLed(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin, &sharedStreamData.led2.data);
          Handler_ReadLed(LED_GREEN_GPIO_Port, LED_GREEN_Pin, &sharedStreamData.led3.data);
      }
      osDelay(100);
  }
  /* USER CODE END StartTask_ReadGpio */
}

/* USER CODE BEGIN Header_StartTask_ReadAdc */
/**
* @brief Function implementing the ReadAdcTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_ReadAdc */
void StartTask_ReadAdc(void const * argument)
{
  /* USER CODE BEGIN StartTask_ReadAdc */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask_ReadAdc */
}

/* USER CODE BEGIN Header_StartTask_ReadTemp */
/**
* @brief Function implementing the ReadTempTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_ReadTemp */
void StartTask_ReadTemp(void const * argument)
{
  /* USER CODE BEGIN StartTask_ReadTemp */
    static uint16_t readValue;
    static float tCelsius;
  /* Infinite loop */
  for(;;)
  {
      if (!lockData)
      {
          HAL_ADC_Start(&hadc1);
          HAL_ADC_PollForConversion(&hadc1,1000);
          readValue = HAL_ADC_GetValue(&hadc1);
          tCelsius = ((VSENSE*readValue - V25) / AVG_SLOPE) + 25;
          HAL_ADC_Stop(&hadc1);
          //snprintf((char*)&sharedStreamData.tmp0.data[0], sizeof(sharedStreamData.tmp0.data), "%f", tCelsius);
      }
      osDelay(250);
  }
  /* USER CODE END StartTask_ReadTemp */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
