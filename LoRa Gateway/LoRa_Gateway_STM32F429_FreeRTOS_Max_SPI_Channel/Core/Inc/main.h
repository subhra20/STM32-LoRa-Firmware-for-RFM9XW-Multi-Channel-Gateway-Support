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

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NSS_2_Pin GPIO_PIN_3
#define NSS_2_GPIO_Port GPIOE
#define RST_2_Pin GPIO_PIN_4
#define RST_2_GPIO_Port GPIOE
#define User_Btn_Pin GPIO_PIN_13
#define User_Btn_GPIO_Port GPIOC
#define DIO0_2_Pin GPIO_PIN_1
#define DIO0_2_GPIO_Port GPIOF
#define NSS_1_Pin GPIO_PIN_6
#define NSS_1_GPIO_Port GPIOF
#define RST_1_Pin GPIO_PIN_7
#define RST_1_GPIO_Port GPIOF
#define NSS_Pin GPIO_PIN_0
#define NSS_GPIO_Port GPIOC
#define RST_Pin GPIO_PIN_1
#define RST_GPIO_Port GPIOC
#define DIO0_0_Pin GPIO_PIN_2
#define DIO0_0_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_14
#define LED3_GPIO_Port GPIOB
#define DIO0_1_Pin GPIO_PIN_15
#define DIO0_1_GPIO_Port GPIOA
#define RST_3_Pin GPIO_PIN_9
#define RST_3_GPIO_Port GPIOG
#define DIO0_3_Pin GPIO_PIN_10
#define DIO0_3_GPIO_Port GPIOG
#define NSS_3_Pin GPIO_PIN_15
#define NSS_3_GPIO_Port GPIOG
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
