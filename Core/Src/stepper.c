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

#define HOME_POS (uint32_t)(1)

/*
 * Internal function prototypes
 */
void StepperSetSettings();
void StepperActivateCoil(StepperMotor *mot, uint8_t stage);
void StepperReset(StepperMotor *mot);

void step_test(int steps, uint8_t direction, uint16_t delay)
{
  int x;
  if (direction == 0)
    HAL_GPIO_WritePin(DIR0_GPIO_Port, DIR0_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(DIR0_GPIO_Port, DIR0_Pin, GPIO_PIN_RESET);
  for (x = 0; x < steps; x = x + 1)
  {
    HAL_GPIO_WritePin(PUL0_GPIO_Port, PUL0_Pin, GPIO_PIN_SET);
    osDelay(delay);
    HAL_GPIO_WritePin(PUL0_GPIO_Port, PUL0_Pin, GPIO_PIN_RESET);
    osDelay(delay);
  }
}

// void StepperInit()
// {
//   int i;

//   p_Motor0 = &Motor[0];
//   p_Motor1 = &Motor[1];

//   StepperSetSettings();

//   // Enable Clocks to all ports
//   SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

//   // Setup GPIO ports as outputs with drive strength enabled
//   GPIO_PDDR_REG(p_Motor0->settings.Pt) = 0;
//   GPIO_PDDR_REG(p_Motor1->settings.Pt) = 0;
//   for (i = 0; i < NUM_PINS; i++)
//   {
//     PORTE_PCR1 = 0;
//     // Motor 1
//     GPIO_PDDR_REG(p_Motor0->settings.Pt) |= (1 << p_Motor0->settings.Pin[i]);
//     PORT_PCR_REG(p_Motor0->settings.Port, p_Motor0->settings.Pin[i]) = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
//     // Motor 2
//     GPIO_PDDR_REG(p_Motor1->settings.Pt) |= (1 << p_Motor1->settings.Pin[i]);
//     PORT_PCR_REG(p_Motor1->settings.Port, p_Motor1->settings.Pin[i]) = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
//   }
// }

// void StepperSetSettings()
// {
//   /* Reset both motor statuses */
//   StepperReset(p_Motor0);
//   StepperReset(p_Motor1);

//   /* Assign motor controller Port and Pin */
//   p_Motor0->settings.ENA_PORT = ENA0_GPIO_Port;
//   p_Motor0->settings.ENA_PIN = ENA0_Pin;
//   p_Motor0->settings.DIR_PORT = DIR0_GPIO_Port;
//   p_Motor0->settings.DIR_PIN = DIR0_Pin;
//   p_Motor0->settings.PUL_PORT = PUL0_GPIO_Port;
//   p_Motor0->settings.PUL_PIN = PUL0_Pin;

//   p_Motor1->settings.ENA_PORT = ENA1_GPIO_Port;
//   p_Motor1->settings.ENA_PIN = ENA1_Pin;
//   p_Motor1->settings.DIR_PORT = DIR1_GPIO_Port;
//   p_Motor1->settings.DIR_PIN = DIR1_Pin;
//   p_Motor1->settings.PUL_PORT = PUL1_GPIO_Port;
//   p_Motor1->settings.PUL_PIN = PUL1_Pin;
// }

// /*
//  * Reset the stepper motor to 0 and stop
//  */
// void StepperReset(StepperMotor *mot)
// {
//   /* Enable both stepper motors */
//   mot->status.enabled = 0;
//   mot->status.enabled = 0;
//   mot->status.current_position = HOME_POS;
//   mot->status.desired_position = HOME_POS;
//   mot->status.speed = 0;
//   mot->status.spinning = 0;
//   /* Re-enable the motor */
//   mot->status.enabled = 1;
//   mot->status.enabled = 1;

//   /* Set to default speed */
//   mot->status.speed = 10;
// }
