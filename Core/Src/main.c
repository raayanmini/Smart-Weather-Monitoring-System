/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim9;

UART_HandleTypeDef huart6;

osThreadId task1Handle;
osThreadId task2Handle;
osThreadId task4Handle;
osThreadId task3Handle;
osSemaphoreId myBinarySem01Handle;
osSemaphoreId myBinarySem02Handle;
/* USER CODE BEGIN PV */
uint8_t dht_buffer[5]={0};
char dtValue[12];
int8_t convCpltFlag;
int8_t config_flag=0;
volatile int8_t wifi_status,wifi_sym_prnt_flag;
uint8_t Time_Date[7];
char EEPROM_WR_BUFF[20];
char EEPROM_RD_BUFF[20];
char str[12];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM9_Init(void);
static void MX_SPI1_Init(void);
void ReadRTC_WriteLCD_1Sec(void const * argument);
void ReadDHT11_5Sec(void const * argument);
void WiFi_Tx(void const * argument);
void Check_InternetStatus(void const * argument);

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
  MX_USART6_UART_Init();
  MX_RTC_Init();
  MX_I2C1_Init();
  MX_TIM9_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  devInit();				//it contains all device initializations
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of myBinarySem01 */
  osSemaphoreDef(myBinarySem01);
  myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

  /* definition and creation of myBinarySem02 */
  osSemaphoreDef(myBinarySem02);
  myBinarySem02Handle = osSemaphoreCreate(osSemaphore(myBinarySem02), 1);

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
  /* definition and creation of task1 */
  osThreadDef(task1, ReadRTC_WriteLCD_1Sec, osPriorityNormal, 0, 128);
  task1Handle = osThreadCreate(osThread(task1), NULL);

  /* definition and creation of task2 */
  osThreadDef(task2, ReadDHT11_5Sec, osPriorityAboveNormal, 0, 128);
  task2Handle = osThreadCreate(osThread(task2), NULL);

  /* definition and creation of task4 */
  osThreadDef(task4, WiFi_Tx, osPriorityNormal, 0, 128);
  task4Handle = osThreadCreate(osThread(task4), NULL);

  /* definition and creation of task3 */
  osThreadDef(task3, Check_InternetStatus, osPriorityNormal, 0, 1024);
  task3Handle = osThreadCreate(osThread(task3), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 32-1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 65535;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */
  TIM9->CR1|=CEN; //enables the counter peripheral
  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RED_LED_Pin|GREEN_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PA0 PA1 CS_Pin DHT11_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|CS_Pin|DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RED_LED_Pin GREEN_LED_Pin */
  GPIO_InitStruct.Pin = RED_LED_Pin|GREEN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ENT_SW_INTR_Pin */
  GPIO_InitStruct.Pin = ENT_SW_INTR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ENT_SW_INTR_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	config_flag=SET;
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_ReadRTC_WriteLCD_1Sec */
/**
  * @brief  Function implementing the task1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ReadRTC_WriteLCD_1Sec */
void ReadRTC_WriteLCD_1Sec(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	  /* PRINTING DATE & TIME*/
	HAL_I2C_Mem_Read(&hi2c1,(0x68<<1),0x00,1, (uint8_t *)Time_Date,7,1000);
	snprintf(dtValue,12,"%02X:%02X:%02X",Time_Date[2],Time_Date[1],Time_Date[0]);
	ST7735_WriteString(100,3,dtValue,Font_7x10,ST7735_ORANGE,ST7735_BLACK);
	snprintf(dtValue,12,"%02X/%02X/%02X",Time_Date[4],Time_Date[5],Time_Date[6]);
	ST7735_WriteString(3,3,dtValue,Font_7x10,ST7735_ORANGE,ST7735_BLACK);

	/* PRINTING TEMPERATURE VALUE*/
	if(convCpltFlag==SET)
	{
		/**** printing temperature value ****/
		snprintf(str,11,"%02d.%01d C",dht_buffer[2],dht_buffer[3]);
		ST7735_WriteString(7,64,str,Font_11x18,ST7735_RED,ST7735_WHITE);
		drawCircle(57,68, 3, ST7735_RED);

		/**** printing humidity value ****/
		snprintf(str,12,"%02d.%01d%%RH",dht_buffer[0],dht_buffer[1]);
		ST7735_WriteString(7,104,str,Font_11x18,ST7735_RED,ST7735_WHITE);
		convCpltFlag=RESET;
	}
	/* PRINTING WIFI STATUS */
	if(wifi_sym_prnt_flag==SET)
	{
		if(wifi_status==STATUS_ERROR)
		{
			HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON red LED
			HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF green LED
			draw_wifi_status(71, 4,0, ST7735_WHITE);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET); //set 13th bit postion to turn OFF red LED
			HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET); //clear 13th bit postion to turn ON green LED
			draw_wifi_status(71, 4,1, ST7735_GREEN);
		}
		wifi_sym_prnt_flag=RESET;
	}
	osDelay(200);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_ReadDHT11_5Sec */
/**
* @brief Function implementing the task2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ReadDHT11_5Sec */
void ReadDHT11_5Sec(void const * argument)
{
  /* USER CODE BEGIN ReadDHT11_5Sec */
  /* Infinite loop */
  for(;;)
  {
	  if(check_DHT_response(TIM9)==SUCCESS)
	  	{
	  		if(getHT(TIM9,dht_buffer,5)==SUCCESS)
	  		{
	  			convCpltFlag=SET;					/* Conversion Complete flag*/
	  		}
	  	}
		osSemaphoreRelease(myBinarySem01Handle);	/* Semaphore release indicates temperature value is ready for uploading*/
		osDelay(5000);
  }
  /* USER CODE END ReadDHT11_5Sec */
}

/* USER CODE BEGIN Header_WiFi_Tx */
/**
* @brief Function implementing the task4 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_WiFi_Tx */
void WiFi_Tx(void const * argument)
{
  /* USER CODE BEGIN WiFi_Tx */
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreWait(myBinarySem02Handle,osWaitForever);	/*Semaphore waiting to upload temperature */
	  wifi_transmit(dht_buffer);
	  wifi_status=RESET;
  }
  /* USER CODE END WiFi_Tx */
}

/* USER CODE BEGIN Header_Check_InternetStatus */
/**
* @brief Function implementing the task3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Check_InternetStatus */
void Check_InternetStatus(void const * argument)
{
  /* USER CODE BEGIN Check_InternetStatus */
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreWait(myBinarySem01Handle,osWaitForever);	/* Semaphore waiting for temperature value */
	  wifi_status=checkInt();
	  wifi_sym_prnt_flag=SET;
	  if(wifi_status==SUCCESS)
	  {
		osSemaphoreRelease(myBinarySem02Handle);			/* Semaphore release to upload temperature*/
	  }
	  else if(wifi_status==STATUS_ERROR)
	  {
		//Writing TO EEPROM
		sprintf(EEPROM_WR_BUFF, "%d%d%d%d%d", Time_Date[2]-6*(Time_Date[2]>>4),Time_Date[1]-6*(Time_Date[1]>>4),Time_Date[0]-6*(Time_Date[0]>>4),(int)dht_buffer[0],(int)dht_buffer[2]);
		HAL_I2C_Mem_Write(&hi2c1,(0x50<<1),0,1,(uint8_t *)EEPROM_WR_BUFF,15,1000);

		//Reading From EEPROM (For debugging purpose)
		HAL_Delay(100);
		HAL_I2C_Mem_Read(&hi2c1,(0x50<<1),0,1,(uint8_t *)EEPROM_RD_BUFF,15,2000);
	  }
  }
  /* USER CODE END Check_InternetStatus */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
