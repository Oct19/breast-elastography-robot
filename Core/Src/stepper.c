/**
 * @file    switch.c
 * @author  Cai Dingkun <caidingkun@outlook.com>
 * @brief   This file provides code for the configuration
 *          of limit switch FC-SPX307Z
 * @version 0.1
 * @date    2022-11-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "stepper.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

void step (int steps, uint8_t direction, uint16_t delay)
{
  int x;
  if (direction == 0)
    HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
  for(x=0; x<steps; x=x+1)
  {
    HAL_GPIO_WritePin(PUL_GPIO_Port, PUL_Pin, GPIO_PIN_SET);
    osDelay(delay);
    HAL_GPIO_WritePin(PUL_GPIO_Port, PUL_Pin, GPIO_PIN_RESET);
    osDelay(delay);
  }
}