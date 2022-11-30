/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

  /* Private includes ----------------------------------------------------------*/
  /* USER CODE BEGIN Includes */

  /* USER CODE END Includes */

  /* Exported types ------------------------------------------------------------*/
  /* USER CODE BEGIN ET */

  typedef enum _ROBOT_STATE_HandleTypeDef
  {
    STATE_RESET,  // 0
    STATE_IDLE,  // 1
    STATE_MOVE,  // 2
    STATE_ERROR, // 3
  } ROBOT_STATE_HandleTypeDef;

  extern ROBOT_STATE_HandleTypeDef ROBOT_STATE;

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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define ENA_Pin GPIO_PIN_12
#define ENA_GPIO_Port GPIOB
#define DIR_Pin GPIO_PIN_13
#define DIR_GPIO_Port GPIOB
#define PUL_Pin GPIO_PIN_14
#define PUL_GPIO_Port GPIOB
#define Z_RESET_Pin GPIO_PIN_3
#define Z_RESET_GPIO_Port GPIOB
#define Z_RESET_EXTI_IRQn EXTI3_IRQn
  /* USER CODE BEGIN Private defines */

  /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
