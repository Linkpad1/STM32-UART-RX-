/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>


sensor_status_handle_TypeDef sensor_handle = { 0,0,0,0,0,0,0,0,0,0 };

// Remove Key Characters from received bytes leaving just raw Data.
void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

void removeString (char text[], int index, int rm_length)
{
    int i;

    for ( i = 0; i < index; ++i )
        if ( text[i] == '\0' )
            return;

    for ( ; i < index + rm_length; ++i )
        if ( text[i] == '\0' ) {
            text[index] = '\0';
            return;
        }

    do {
        text[i - rm_length] = text[i];
    } while ( text[i++] != '\0' );
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
UART_HandleTypeDef UartHandle;
__IO ITStatus UartReady = RESET;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM10_Init(void);
/* USER CODE BEGIN PFP */


char buffer[11] = {0};
uint8_t reception_complete = 0;
bool Rx1 = false;
bool Rx2 = false;
bool Rx3 = false;
bool Rx4 = false;
bool Rx5 = false;
bool Rx6 = false;
bool Rx7 = false;
bool Rx8 = false;
bool Rx9 = false;
bool Rx10 = false;

//Timers
uint8_t Timer2Int = 0; // 1 second timer
uint8_t Timer3Int = 0; // 10 second timer
uint16_t Timer10 = 0; // 1 Second Timeout Timer
uint16_t RXcount = 0; // RX packet count
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



void RXdata(void)
{
	if((reception_complete == 1) && (Timer10 < 500 ))
	{
	 // Sensor 1
	 if (strncmp(buffer, "SENS01", 6) == 0)
		  {
		  removeString (buffer, 0, 6);
		  sensor_handle.Sensor1= atoi(buffer);
		  Rx1 = true;
		  }

	 // Sensor 2
	 if (strncmp(buffer, "SENS02", 6) == 0)
		 {
		  // Subtract the 6 start chars from string
		  removeString (buffer, 0, 6);
		  sensor_handle.Sensor2= atoi(buffer);
		  Rx2 = true;
	     }

	 // Sensor 3
	 if (strncmp(buffer, "SENS03", 6) == 0)
		 {
		  // Subtract the 6 start chars from string
		  removeString (buffer, 0, 6);
		  sensor_handle.Sensor3= atoi(buffer);
		  Rx3 = true;
	     }

	 // Sensor 4
	 if (strncmp(buffer, "SENS04", 6) == 0)
		 {
		  // Subtract the 6 start chars from string
		  removeString (buffer, 0, 6);
		  sensor_handle.Sensor4= atoi(buffer);
		  Rx4 = true;
	     }

	 // Sensor 5
	 if (strncmp(buffer, "SENS05", 6) == 0)
		 {
		  // Subtract the 6 start chars from string
		  removeString (buffer, 0, 6);
		  sensor_handle.Sensor5= atoi(buffer);
		  Rx5 = true;
	     }

	 // Sensor 6
	 if (strncmp(buffer, "SENS06", 6) == 0)
		 {
		  // Subtract the 6 start chars from string
		  removeString (buffer, 0, 6);
		  sensor_handle.Sensor6= atoi(buffer);
		  Rx6 = true;
	     }

	 // Sensor 7
	 if (strncmp(buffer, "SENS07", 6) == 0)
		 {
		  // Subtract the 6 start chars from string
		  removeString (buffer, 0, 6);
		  sensor_handle.Sensor7= atoi(buffer);
		  Rx7 = true;
	     }

	 // Sensor 8
	 if (strncmp(buffer, "SENS08", 6) == 0)
		 {
		  // Subtract the 6 start chars from string
		  removeString (buffer, 0, 6);
		  sensor_handle.Sensor8= atoi(buffer);
		  Rx8 = true;
	     }

	 // Sensor 9
	if (strncmp(buffer, "SENS09", 6) == 0)
		 {
		 // Subtract the 6 start chars from string
		 removeString (buffer, 0, 6);
		 sensor_handle.Sensor9= atoi(buffer);
		 Rx9 = true;
		 }

	// Sensor 10
	if (strncmp(buffer, "SENS10", 6) == 0)
		 {
		 // Subtract the 6 start chars from string
		 removeString (buffer, 0, 6);
		 sensor_handle.Sensor10= atoi(buffer);
		 Rx10 = true;
		 }
	// if all packets receive have failed reset the UART irq
	if ((Rx1==0) & (Rx2==0) & (Rx3==0) & (Rx4==0) & (Rx5==0) && (Rx6==0) && (Rx7==0) && (Rx8==0) && (Rx9==0) && (Rx10==0))
	{
	HAL_UART_Transmit_IT(&huart2,  "Missed RX", sizeof"Missed RX");
	HAL_Delay(1);
	__disable_irq();
	HAL_UART_AbortReceive_IT(&huart3);
	huart3.RxState =  HAL_UART_STATE_READY;
	__enable_irq();
	}
	// reset RX packet flags
	 Rx1 = false;
	 Rx2 = false;
	 Rx3 = false;
	 Rx4 = false;
	 Rx5 = false;
	 Rx6 = false;
	 Rx7 = false;
	 Rx8 = false;
	 Rx9 = false;
	 Rx10 = false;
	}
 reception_complete = 0;
}


void SensorStatus (void)
{
	if(Timer2Int ==1)
	{
		{
		char buffer1[200] = {0};
		snprintf(buffer1, sizeof(buffer1), "S1 %d \n, S2 %d \n, S3 %d \n, S4 %d \n, S5 %d \n, S6 %d \n, S7 %d \n, S8 %d \n, S9 %d \n, S10 %d \n", sensor_handle.Sensor1,sensor_handle.Sensor2,sensor_handle.Sensor3,sensor_handle.Sensor4,sensor_handle.Sensor5,sensor_handle.Sensor6,sensor_handle.Sensor7,sensor_handle.Sensor8,sensor_handle.Sensor9,sensor_handle.Sensor10);
	    HAL_UART_Transmit_IT(&huart2,  buffer1, strlen(buffer1));
	}

	Timer2Int = 0;
	}

}

void ErrorHandler(void)
{
	Timer10 = __HAL_TIM_GET_COUNTER(&htim10);
	if (Timer10 > 500)
	{
		HAL_UART_Transmit(&huart2,  "No Connection", sizeof"No Connection", 100);
		sensor_handle.Sensor1 = 0;
		sensor_handle.Sensor2 = 0;
		sensor_handle.Sensor3 = 0;
		sensor_handle.Sensor4 = 0;
		sensor_handle.Sensor5 = 0;
		sensor_handle.Sensor6 = 0;
		sensor_handle.Sensor7 = 0;
		sensor_handle.Sensor8 = 0;
		sensor_handle.Sensor9 = 0;
		sensor_handle.Sensor10 = 0;
	}

}




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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_DMA(&huart3, &buffer, 11);
	 HAL_TIM_Base_Start_IT(&htim2); // start 1 second timer
	 HAL_TIM_Base_Start_IT(&htim3); // start 10 second timer



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	   HAL_TIM_Base_Start(&htim10); // start 10 second timer
	   HAL_UART_Receive_DMA(&huart3, &buffer, 11);

	   // UART Print RX Status
	   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);


	 RXdata();
	 SensorStatus();
	 ErrorHandler();

	 // reset UART flag
	 UartReady = RESET;
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 16000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1001-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 16000;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10001-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 16000;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 1001-1;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_SET);

  /*Configure GPIO pins : PE2 PE3 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  //if(huart->Instance == huart2.Instance)
  //{
  HAL_UART_Receive_DMA(&huart3, &buffer, 11);
  HAL_UART_Transmit_IT(&huart2,  &buffer, 11);

 /* Set transmission flag: trasfer complete*/
  reception_complete = 1;
  RXcount++;
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
  /* Set transmission flag: trasfer complete*/
  UartReady = SET;

  TIM10->CNT &= 0x0; // restart the RX Timeout Counter [0.5S]
 // }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if (htim->Instance == TIM2) // 1 second timer
	 {
		 Timer2Int = 1;
	 }

	 if (htim->Instance == TIM3) // 10 second timer
		 {
			 Timer3Int = 1;

		 }

 }

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
