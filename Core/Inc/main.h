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
extern "C" {
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
    STATE_RESET, // 0 ignore Rx, homing procedure
    STATE_IDLE,  // 1 stepper off
    STATE_MOVE,  // 2 stepper destination != current position
    STATE_ERROR, // 3 cant homing,
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
#define ENA0_Pin GPIO_PIN_12
#define ENA0_GPIO_Port GPIOB
#define DIR0_Pin GPIO_PIN_13
#define DIR0_GPIO_Port GPIOB
#define PUL0_Pin GPIO_PIN_14
#define PUL0_GPIO_Port GPIOB
#define ENA1_Pin GPIO_PIN_8
#define ENA1_GPIO_Port GPIOA
#define DIR1_Pin GPIO_PIN_9
#define DIR1_GPIO_Port GPIOA
#define PUL1_Pin GPIO_PIN_10
#define PUL1_GPIO_Port GPIOA
#define Z_RESET_Pin GPIO_PIN_3
#define Z_RESET_GPIO_Port GPIOB
#define Z_RESET_EXTI_IRQn EXTI3_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
