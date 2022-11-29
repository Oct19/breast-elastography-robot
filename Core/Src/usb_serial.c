/**
 * @file    usb_serial.c
 * @author  Cai Dingkun <caidingkun@outlook.com>
 * @brief   This file provides code for the configuration
 *          of USB serial port. Default Baud rate: 9600, Line ending: CRLF
 * @version 0.1
 * @date    2022-11-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "usb_serial.h"

#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "OLEDdisplay.h"

char USB_Receive_Buf[64];
char USB_Transmit_Buf[64];

void USB_Transmit_Hello(void)
{
    memset(USB_Transmit_Buf, 0, sizeof(USB_Transmit_Buf)); // clear previous message

    // Print Hello message for 10 seconds
    int t = HAL_GetTick();
    char message[10];
    if (t < 10000)
    {
        sprintf(message, "Hi %d\n\r", t);
    }
    else
    {
        memset(message, '\0', sizeof(message));
    }

    memcpy(USB_Transmit_Buf, message, strlen(message));
    CDC_Transmit_FS((uint8_t *)USB_Transmit_Buf, strlen(USB_Transmit_Buf));

    // Update OLED message
    memset(OLED.Tx, '\0', sizeof(OLED.Tx));
    memcpy(OLED.Tx, USB_Transmit_Buf, strlen((char *)USB_Transmit_Buf));
}

/**
 * @brief Actions after receiving USB serial message
 *
 */
void USB_Receive(uint8_t *Buf, uint32_t *Len)
{
    // Update OLED message
    memset(OLED.Rx, '\0', sizeof(OLED.Rx));
    memcpy(OLED.Rx, Buf, strlen((char *)Buf));

    if (USB_Serial_Echo)
    {
        CDC_Transmit_FS(Buf, (uint8_t)*Len);
    }
}