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
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask_CmdHandler(void const * argument);
void StartTask_StreamData(void const * argument);

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
  osThreadDef(StreamDataTask, StartTask_StreamData, osPriorityNormal, 0, 128);
  StreamDataTaskHandle = osThreadCreate(osThread(StreamDataTask), NULL);

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
      vTaskList(pRxBuf);
      HAL_UART_Transmit_IT(&huart2, (uint8_t*)pRxBuf, sizeof(pRxBuf));
      osDelay(1000);
  }
  /* USER CODE END StartTask_StreamData */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
