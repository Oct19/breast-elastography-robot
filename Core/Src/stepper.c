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
#include "math.h"

#define HOME_POS (uint32_t)(1)

/****************************************************************
 * Functions below are simple, using osDelay
 ****************************************************************
 */

/**
 * @brief Toggle Pulse pin with osDelay
 */
void step_simplest(void)
{
  HAL_GPIO_TogglePin(PUL0_GPIO_Port, PUL0_Pin);
  osDelay(1);
}

/**
 * @brief
 *
 * @param steps number of steps
 * @param direction 0/1 CCL/CW
 * @param delay milliseconds delay between each pulse. 1 ms is ok
 */
void step_constantSpeed(int steps, uint8_t direction, uint8_t delay)
{
  HAL_GPIO_WritePin(DIR0_GPIO_Port, DIR0_Pin, direction);
  for (int i = 0; i < steps; i++)
  {
    HAL_GPIO_WritePin(PUL0_GPIO_Port, PUL0_Pin, SET);
    osDelay(delay);
    HAL_GPIO_WritePin(PUL0_GPIO_Port, PUL0_Pin, RESET);
    osDelay(delay);
  }
  osDelay(1000);
}

/**
 * @brief Changing Step speed via changing osDelay delay
 * Interval change is linear, while speed change is not linear
 *
 * @param steps Number of steps
 */
void step_simpleAccel(int steps)
{
  int lowSpeed = 30;
  int highSpeed = 1;
  int change = 1;

  int rampUpStop = (lowSpeed - highSpeed) / change;
  if (rampUpStop > steps / 2)
    rampUpStop = steps / 2; // movement finished before reach highSpeed
  int rampDownStart = steps - rampUpStop;

  int delay = lowSpeed;
  for (int i = 0; i < steps; i++)
  {
    HAL_GPIO_WritePin(PUL0_GPIO_Port, PUL0_Pin, SET);
    osDelay(delay);
    HAL_GPIO_WritePin(PUL0_GPIO_Port, PUL0_Pin, RESET);
    osDelay(delay);

    if (i < rampUpStop)
      delay -= change;
    else if (i > rampDownStart)
      delay += change;
  }
  osDelay(1000);
}

/**
 * @brief Interstep delay with Taylor series approximation
 *
 * For detailed calculation, refer to documentation:
 * AVR446: Linear speed control of stepper motor,
 * 2.3.1 Exact calculations of the inter-step delay
 *
 * @param steps number of steps
 */
void step_constantAccel()
{
  /* Calculate delay for each step */
  int steps = 100;
  int delays[steps];
  int spr = 200;                  // steps per round
  float angle = 2 * pi / spr;     // angle per step (rad)
  float accel = 30;               // rad/s2
  float counterFreequency = 1000; // Hz
  float c0 = counterFreequency * sqrt(2 * angle / accel) * 0.67703;
  float lastDelay = 0;
  int highSpeed = 1; // minimum osDelay 1 milliseconds
  for (int i = 0; i < steps; i++)
  {
    float d = c0;
    if (i > 0)
    {
      d = lastDelay - 2 * lastDelay / (4 * i - 1);
      angle++;
    }
    if ((int)d <= highSpeed)
    {
      d = highSpeed;
      angle++;
    }
    delays[i] = d;
    lastDelay = d;
  }
  /* Acceleration */
  for (int i = 0; i < steps; i++)
  {
    HAL_GPIO_WritePin(PUL0_GPIO_Port, PUL0_Pin, RESET);
    osDelay(delays[i]);
    HAL_GPIO_WritePin(PUL0_GPIO_Port, PUL0_Pin, SET);
    osDelay(delays[i]);
  }
  /* Deceleration */
  for (int i = 0; i < steps; i++)
  {
    HAL_GPIO_WritePin(PUL0_GPIO_Port, PUL0_Pin, RESET);
    osDelay(delays[steps - i - 1]);
    HAL_GPIO_WritePin(PUL0_GPIO_Port, PUL0_Pin, SET);
    osDelay(delays[steps - i - 1]);
  }
  osDelay(1000);
}

/****************************************************************
 * Functions below are basic, use interrupts
 * Stepper GPIO output pin label handled by STM32MX:
 * ENA: Connects all motor ENA-
 * PUL: PUL1 for steppers[1], PULn for steppers[n]
 * DIR: DIR1 for steppers[1], DIRn for steppers[n]
 * ********************************
 * Timer configuration in SMT32MX:
 * Clock source: internal clock
 * Prescaler: HCLK(MHz) - 1 (for STM32F103 = 72-1)
 * Counter period: handled by function below
 * ****************************************************************
 */

/**
 * @brief Inside HAL_TIM_PeriodElapsedCallback
 *
 */
void step_ISR()
{
  // HAL_TIM_Base_Start_IT(&htim1);
  //  TIM1->CCR3 = 50;
}

void resetStepperInfo(stepperInfo si)
{
  si.n = 0;
  si.d = 0;
  si.di = 0;
  si.stepCount = 0;
  si.rampUpStepCount = 0;
  si.totalSteps = 0;
  si.stepPosition = 0;
  si.movementDone = false;
}

void resetStepper(volatile stepperInfo si)
{
  si.c0 = si.acceleration;
  si.d = si.c0;
  si.di = si.d;
  si.stepCount = 0;
  si.n = 0;
  si.rampUpStepCount = 0;
  si.movementDone = false;
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

/**
 * @brief Generate S-curve for stepper motor speed:
 *
 * When FStart < FStop, curve goes up;
 * When FStart > FStop, curve goes down;
 * When FStart = FStop, curve is flat horizontal line
 *
 * @param len number of samples
 * @param FStart
 * @param FStop
 * @param flexible define curvature, larger means curvy. recommend 4-6
 * @param index
 * @return float
 */
float motorPower_PowerSLine(float len, float FStart, float FStop, float flexible, uint8_t index)
{
  float deno;
  float melo;
  float num;
  float Fcurrent;

  if (index > len)
    index = len;
  num = len / 2;
  melo = flexible * (index - num) / num;
  deno = 1.0 / (1 + expf(-melo));
  Fcurrent = FStart - (FStart - FStop) * deno;

  return Fcurrent;
}