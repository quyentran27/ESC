/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void maincpp();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define ZC_B_Pin GPIO_PIN_14
#define ZC_B_GPIO_Port GPIOC
#define ZC_B_EXTI_IRQn EXTI15_10_IRQn
#define ZC_C_Pin GPIO_PIN_15
#define ZC_C_GPIO_Port GPIOC
#define ZC_C_EXTI_IRQn EXTI15_10_IRQn
#define PWM_A_Pin GPIO_PIN_0
#define PWM_A_GPIO_Port GPIOA
#define REV_Pin GPIO_PIN_2
#define REV_GPIO_Port GPIOA
#define OD_A_Pin GPIO_PIN_4
#define OD_A_GPIO_Port GPIOA
#define OD_B_Pin GPIO_PIN_5
#define OD_B_GPIO_Port GPIOA
#define OD_C_Pin GPIO_PIN_6
#define OD_C_GPIO_Port GPIOA
#define PWM_B_Pin GPIO_PIN_0
#define PWM_B_GPIO_Port GPIOB
#define PWM_C_Pin GPIO_PIN_1
#define PWM_C_GPIO_Port GPIOB
#define ZC_A_Pin GPIO_PIN_9
#define ZC_A_GPIO_Port GPIOB
#define ZC_A_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
