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
#include "main.h"
#include "ssd1306.h"

OLED_HandleTypeDef OLED;

/**
 * @brief for testing purposes
 *
 */
void OLED_display_welcome(void)
{
    ssd1306_SetCursor(20, 5);
    ssd1306_Fill(White);
    ssd1306_WriteString("Welcome!", Font_11x18, Black);
    ssd1306_UpdateScreen();
}

/**
 * @brief for testing purposes
 *
 */
void OLED_display_off(void)
{
    ssd1306_SetCursor(20, 5);
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();
}

/**
 * @brief Collect messages, and then determine priority
 *
 * Message          Pritority
 * Warning              3
 * Notification         2
 * Tx,Rx                1
 * State,Positions      0
 * @todo link to current state, position
 *
 */
void OLED_get_priority(void)
{
    OLED.Positions = "Z: 0.2mm A:120";

    if (OLED.Rx)
    {
        OLED.Positions = "123";
    }

    if (OLED.Warning != NULL)
    {
        OLED.priority = 3;
    }
    else if (OLED.Notification != NULL)
    {
        OLED.priority = 2;
    }
    else if (strlen((char *)OLED.Rx) + strlen((char *)OLED.Tx) != 0)
    {
        OLED.priority = 1;
    }
    else
    {
        OLED.priority = 0;
    }
}

/**
 * @brief  Display message based on priority level,
 * clear all message buffer, update priotiry_old,
 * @todo Better way to clear message buffer
 *
 */
void OLED_display_message(void)
{

    switch (OLED.priority)
    {
    case 3:; // Warning
        ssd1306_Fill(White);
        ssd1306_SetCursor(20, 10);
        ssd1306_WriteString(OLED.Warning, Font_11x18, Black);
        OLED.Warning = NULL;
        break;
    case 2:; // Notification
        ssd1306_Fill(Black);
        ssd1306_SetCursor(20, 10);
        ssd1306_WriteString(OLED.Notification, Font_11x18, White);
        OLED.Notification = NULL;
        break;
    case 1:; // Tx Rx
        ssd1306_Fill(Black);
        // First row: Tx
        char Txbuf[20];
        strcpy(Txbuf, "Tx: ");
        // strcat(Txbuf, OLED.Tx);
        strcat(Txbuf, (char *)OLED.Tx);
        ssd1306_SetCursor(0, 0);
        ssd1306_WriteString(Txbuf, Font_7x10, White);
        memset(OLED.Tx, '\0', sizeof(OLED.Tx));
        // Second row: Rx
        char Rxbuf[20];
        strcpy(Rxbuf, "Rx:");
        strcat(Rxbuf, (char *)OLED.Rx);
        ssd1306_SetCursor(0, 20);
        ssd1306_WriteString(Rxbuf, Font_7x10, White);
        memset(OLED.Rx, '\0', sizeof(OLED.Rx));
        break;
    case 0:; // State Positions
        ssd1306_Fill(Black);
        // First row: State
        ssd1306_SetCursor(30, 0);

        char *str = NULL;
        switch (ROBOT_STATE)
        {
        case STATE_RESET:;
            str = "RESET";
            break;
        case STATE_IDLE:;
            str = "Idle";
            break;
        case STATE_MOVE:;
            str = "Move";
            break;
        case STATE_ERROR:;
            str = "Error";
            break;
        }
        ssd1306_WriteString(str, Font_7x10, White);
        // Second row: Positions
        ssd1306_SetCursor(0, 20);
        ssd1306_WriteString(OLED.Positions, Font_7x10, White);
        OLED.Positions = NULL;
        break;
    }
    ssd1306_UpdateScreen();

    // clear message buffer
    // Update priority_old
    OLED.priority_old = OLED.priority;
}