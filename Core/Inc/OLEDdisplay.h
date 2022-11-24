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
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

void OLED_display_time(void);
void OLED_display_welcome(void);

#ifdef __cplusplus
}
#endif

#endif /* __DISPLAY_H__ */