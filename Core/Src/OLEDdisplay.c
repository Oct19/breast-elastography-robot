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

#include "rtc.h"
#include "string.h"
#include "stdio.h"
#include "ssd1306.h"

void OLED_display_time(void)
{
    char timestamp[30];
    RTC_datetime(timestamp);

    char date[10], time[10];
    memcpy(date,timestamp,10*sizeof(char));
    memcpy(time,&timestamp[10],10*sizeof(char));

    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    char retVal;
    retVal = ssd1306_WriteString(date, Font_7x10, White);
    ssd1306_SetCursor(0, 10);
    retVal = ssd1306_WriteString(time, Font_7x10, White);

    ssd1306_UpdateScreen();
}

void OLED_display_welcome(void)
{
    ssd1306_SetCursor(20, 5);
    ssd1306_Fill(White);
    ssd1306_WriteString("Welcome!", Font_11x18, Black);
    ssd1306_UpdateScreen();
}