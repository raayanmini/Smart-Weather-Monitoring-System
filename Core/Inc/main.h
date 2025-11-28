/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735.h"
#include "WIFI.h"
#include <stdio.h>
#include "dht11.h"
#include "GFX_FUNCTIONS.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define SUCCESS			0
#define FAIL 			1
#define WIFI_NOT_FOUND	-1
#define PASSWORD_ERROR	-2
#define STATUS_ERROR	3
#define RESET			-10
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void devInit(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CS_Pin GPIO_PIN_4
#define CS_GPIO_Port GPIOA
#define RED_LED_Pin GPIO_PIN_13
#define RED_LED_GPIO_Port GPIOB
#define GREEN_LED_Pin GPIO_PIN_14
#define GREEN_LED_GPIO_Port GPIOB
#define DHT11_Pin GPIO_PIN_8
#define DHT11_GPIO_Port GPIOA
#define ENT_SW_INTR_Pin GPIO_PIN_10
#define ENT_SW_INTR_GPIO_Port GPIOC
#define ENT_SW_INTR_EXTI_IRQn EXTI15_10_IRQn

/* USER CODE BEGIN Private defines */
#define RTC_ADDR	(0x68<<1)
#define EEPROM_ADDR	(0x50<<1)
#define CEN			BIT(0)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
