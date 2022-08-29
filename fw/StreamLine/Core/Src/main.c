/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
volatile CircularBuffer pCirBuf;
volatile char pTxAns[63] = {0}; // 13 for data & 50 for footer
volatile dataStruct     sharedStreamData = {0};
volatile dataStruct     txStream = {0};
volatile uint32_t adcVals[3] = {0};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */

  pCirBuf  = CircularBuffer_Create(RECORD_COUNT, RECORD_SIZE);
  HAL_UART_Receive_IT (&huart2, &gTermByte, sizeof(uint8_t));
  HAL_ADC_Start_DMA(&hadc1,adcVals,3);

  // Init dataStruct
  memcpy(sharedStreamData.adc0.id, (char*)HANDLER_ADC0, sizeof(sharedStreamData.adc0.id));
  sharedStreamData.adc0.sep1    = ',';
  sharedStreamData.adc0.upd     = HANDLER_ENABLE;
  sharedStreamData.adc0.sep2    = ',';
  memset(sharedStreamData.adc0.data, '0', sizeof(sharedStreamData.adc0.data));
  sharedStreamData.adc0.end[0]  = '\n';
  sharedStreamData.adc0.end[1]  = '\r';

  memcpy(sharedStreamData.adc1.id, (char*)HANDLER_ADC1, sizeof(sharedStreamData.adc1.id));
  sharedStreamData.adc1.sep1    = ',';
  sharedStreamData.adc1.upd     = HANDLER_ENABLE;
  sharedStreamData.adc1.sep2    = ',';
  memset(sharedStreamData.adc1.data, '0', sizeof(sharedStreamData.adc1.data));
  sharedStreamData.adc1.end[0]  = '\n';
  sharedStreamData.adc1.end[1]  = '\r';

  memcpy(sharedStreamData.led0.id, (char*)HANDLER_LED0, sizeof(sharedStreamData.led0.id));
  sharedStreamData.led0.sep1   = ',';
  sharedStreamData.led0.upd    = HANDLER_ENABLE;
  sharedStreamData.led0.sep2   = ',';
  memcpy(sharedStreamData.led0.data, (char*)HANDLER_ON, sizeof(sharedStreamData.led0.data));
  sharedStreamData.led0.end[0] = '\n';
  sharedStreamData.led0.end[1] = '\r';

  memcpy(sharedStreamData.led1.id, (char*)HANDLER_LED1, sizeof(sharedStreamData.led1.id));
  sharedStreamData.led1.sep1   = ',';
  sharedStreamData.led1.upd    = HANDLER_ENABLE;
  sharedStreamData.led1.sep2   = ',';
  memcpy(sharedStreamData.led1.data, (char*)HANDLER_ON, sizeof(sharedStreamData.led1.data));
  sharedStreamData.led1.end[0] = '\n';
  sharedStreamData.led1.end[1] = '\r';

  memcpy(sharedStreamData.led2.id, (char*)HANDLER_LED2, sizeof(sharedStreamData.led2.id));
  sharedStreamData.led2.sep1   = ',';
  sharedStreamData.led2.upd    = HANDLER_ENABLE;
  sharedStreamData.led2.sep2   = ',';
  memcpy(sharedStreamData.led2.data, (char*)HANDLER_ON, sizeof(sharedStreamData.led2.data));
  sharedStreamData.led2.end[0] = '\n';
  sharedStreamData.led2.end[1] = '\r';

  memcpy(sharedStreamData.led3.id, (char*)HANDLER_LED3, sizeof(sharedStreamData.led3.id));
  sharedStreamData.led3.sep1   = ',';
  sharedStreamData.led3.upd    = HANDLER_ENABLE;
  sharedStreamData.led3.sep2   = ',';
  memcpy(sharedStreamData.led3.data, (char*)HANDLER_ON, sizeof(sharedStreamData.led3.data));
  sharedStreamData.led3.end[0] = '\n';
  sharedStreamData.led3.end[1] = '\r';

  memcpy(sharedStreamData.btn0.id, (char*)HANDLER_BTN0, sizeof(sharedStreamData.btn0.id));
  sharedStreamData.btn0.sep1   = ',';
  sharedStreamData.btn0.upd    = HANDLER_ENABLE;
  sharedStreamData.btn0.sep2   = ',';
  memcpy(sharedStreamData.btn0.data, (char*)HANDLER_OFF, sizeof(sharedStreamData.btn0.data));
  sharedStreamData.btn0.end[0] = '\n';
  sharedStreamData.btn0.end[1] = '\r';

  memcpy(sharedStreamData.hld0.id, (char*)HANDLER_HLD0, sizeof(sharedStreamData.hld0.id));
  sharedStreamData.hld0.sep1   = ',';
  sharedStreamData.hld0.upd    = HANDLER_ENABLE;
  sharedStreamData.hld0.sep2   = ',';
  memcpy(sharedStreamData.hld0.data, (char*)HANDLER_OFF, sizeof(sharedStreamData.hld0.data));
  sharedStreamData.hld0.end[0] = '\n';
  sharedStreamData.hld0.end[1] = '\r';

  memcpy(sharedStreamData.tmp0.id, (char*)HANDLER_TMP0, sizeof(sharedStreamData.tmp0.data));
  sharedStreamData.tmp0.sep1    = ',';
  sharedStreamData.tmp0.upd     = HANDLER_ENABLE;
  sharedStreamData.tmp0.sep2    = ',';
  memset(sharedStreamData.tmp0.data, '0', sizeof(sharedStreamData.tmp0.data));
  sharedStreamData.tmp0.end[0]  = '\n';
  sharedStreamData.tmp0.end[1]  = '\r';

  memset(sharedStreamData.end, '-', sizeof(sharedStreamData.end));
  sharedStreamData.end[48] = '\n';
  sharedStreamData.end[49] = '\r';

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
