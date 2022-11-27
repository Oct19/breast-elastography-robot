/**
 * @file    display.c
 * @author  Dingkun <https://github.com/oct19>
 * @brief   This file provides code for the configuration
 *          of the OLED display
 * @version 0.1
 * @date 2022-11-09
 *
 * @copyright Copyright (c) 2022
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 */

/* Includes ---------------------------------------------------------------- */
#include "OLEDdisplay.h"

#include "string.h"
#include "stdio.h"
#include "ssd1306.h"

void OLED_display_welcome(void)
{
    ssd1306_SetCursor(20, 5);
    ssd1306_Fill(White);
    ssd1306_WriteString("Welcome!", Font_11x18, Black);
    ssd1306_UpdateScreen();
}

void OLED_display_off(void)
{
    ssd1306_SetCursor(20, 5);
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();
}