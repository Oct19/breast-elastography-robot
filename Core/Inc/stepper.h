/**
 * @file    switch.h
 * @author  Cai Dingkun <caidingkun@outlook.com>
 * @brief   This file contains all the function prototypes for
 *          the stepper.c file (Limit switch part number: FC-SPX307Z)
 * @version 0.1
 * @date    2022-11-10
 *
 * @copyright Copyright (c) 2022
 *
 * ****************************************************************
 * Stepper Motor
 * Part number: 28HS4401-65N2-50
 * Current rating: 0.7A/phase
 * Step angle: 1.8 degrees
 * Lead screw increment: 0.01mm/step
 * **************************************************************
 * Stepper Motor Driver
 * Part number: UM242 (TB6600 family)
 * Half motor current during idel state
 * Voltage high: >3.5V
 * ENA: at least 5μs before DIR signal
 * DIR: at least 2μs before PUL signal
 * PUL: Both high and low signal should be longer than 2μs
 */
#ifndef __switch_H__
#define __switch_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "robot.h"
#include "stdbool.h"


#define SWITCH_POS (HAL_GPIO_ReadPin(SWITCH_POS_GPIO_Port,SWITCH_POS_Pin))
#define SWITCH_NEG (HAL_GPIO_ReadPin(SWITCH_NEG_GPIO_Port, SWITCH_NEG_Pin))

#define MOT_OFF HAL_GPIO_WritePin(ENA1_GPIO_Port, ENA1_Pin, RESET)
#define MOT_ON HAL_GPIO_WritePin(ENA1_GPIO_Port, ENA1_Pin, SET)

#define DIR_CW HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, SET)
#define DIR_CCW HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, RESET)

void Stepper_Init(void);

void step_simplest(void);
void step_constantSpeed(int steps, uint8_t direction, uint8_t delay);
void step_simpleAccel(int steps);
void step_constantAccel();

#ifdef __cplusplus
}
#endif
#endif /*__ switch_H__ */
