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
 * **************************************************************
 * Limit switch
 * Part number: FC-SPX307Z
 * Connection:
 * COLOR    NAME    CONNECTION
 * BROWN    5-24VDC 5V
 * BLUE     GND     GND
 * BLACK    NPN.NO  STM32:LIMIT_SWITCH
 * WHITE    NPN.NC  OPEN
 * Note:
 * For NO output, high voltage is detected
 * ****************************************************************
 * Stepper Motor
 * Part number: 28HS4401-65N2-50
 * Connection:
 * COLOR    NAME    CONNECTION
 * BLACK    A+      A+
 * GREEN    A-      A-
 * RED      B+      B+
 * BLUE     B-      B-
 * Note:
 * Current rating: 0.7A/phase
 * Step angle: 1.8 degrees
 * Lead screw increment: 0.01mm/step
 * **************************************************************
 * Stepper Motor Driver
 * Part number: UM242 (TB6600 family)
 * Connection:
 * NAME     CONNECTION      NOTE
 * V+,V-    24VDC POWER     8-36VDC
 * A+-      MOTOR           POLARITY MATTERS
 * B+-      MOTOR           POLARITY MATTERS
 * PUL-     STM32:PUL       PULSE, STEP INPUT
 * DIR-     STM32:DIR       DIRECTION INPUT
 * ENA-     STM32:ENA       ENABLE
 * PUL+     5V
 * DIR+     5V
 * ENA+     5V              
 * Note:
 * Connection of ENA+- is optional
 * Configure SW1-8 before use
 * Half motor current during idel state
 * Voltage high: >3.5V
 * ENA signal should be 5μs before DIR signal
 * DIR signal should be 2μs before PUL signal
 */
#ifndef __switch_H__
#define __switch_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

// Limit switch
#define PRESSED                 0
#define LIMIT_SWITCH_STATUS     HAL_GPIO_ReadPin(LIMIT_SWITCH_GPIO_Port, LIMIT_SWITCH_Pin)





#ifdef __cplusplus
}
#endif
#endif /*__ switch_H__ */
