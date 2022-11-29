/**
 * @file    display.h
 * @author  Dingkun <https://github.com/oct19>
 * @brief   This file contains all the function prototypes for
*           the display.c file
 * @version 0.1
 * @date    2022-11-09
 * 
 * @copyright Copyright (c) 2022
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OLEDDISPLAY_H__
#define __OLEDDISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/**
 * @brief Content and priosity for OLED display
 * @todo Seperate Positions into Z and angle
 */
typedef struct _OLED_HandleTypeDef{
    char    *Warning;           // priority 3
    char    *Notification;      // priority 2
    uint8_t    Tx[64];                // priority 1
    uint8_t    Rx[64];                // priority 1
    char    *State;             // priority 0
    char    *Positions;         // priority 0
    int     priority_old;
    int     priority;
} OLED_HandleTypeDef;

extern OLED_HandleTypeDef OLED;

void OLED_get_priority(void);
void OLED_display_welcome(void);
void OLED_display_off(void);
void OLED_display_message(void);

#ifdef __cplusplus
}
#endif

#endif /* __OLEDDISPLAY_H__ */