/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f7xx_hal.h"

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

// stupid cubemx cannot mark MPU_Config as non static
// and I need wrapper to export it out of module
void generated_MPU_Config(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED4_Pin GPIO_PIN_8
#define LED4_GPIO_Port GPIOI
#define JOYCTR_Pin GPIO_PIN_11
#define JOYCTR_GPIO_Port GPIOI
#define JOYCTR_EXTI_IRQn EXTI15_10_IRQn
#define LED3_Pin GPIO_PIN_4
#define LED3_GPIO_Port GPIOH
#define JOYA_Pin GPIO_PIN_2
#define JOYA_GPIO_Port GPIOG
#define JOYA_EXTI_IRQn EXTI2_IRQn
#define JOYB_Pin GPIO_PIN_3
#define JOYB_GPIO_Port GPIOG
#define JOYB_EXTI_IRQn EXTI3_IRQn
#define JOYC_Pin GPIO_PIN_4
#define JOYC_GPIO_Port GPIOD
#define JOYC_EXTI_IRQn EXTI4_IRQn
#define JOYD_Pin GPIO_PIN_5
#define JOYD_GPIO_Port GPIOD
#define JOYD_EXTI_IRQn EXTI9_5_IRQn
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
