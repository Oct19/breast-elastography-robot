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

char USB_Receive_Buf[64];
char USB_Transmit_Buf[128];

void usb_serial_update(void)
{

    memset(USB_Transmit_Buf, 0, sizeof(USB_Transmit_Buf)); // clear previous message

    char *message = "Hello";
    strcat(USB_Transmit_Buf, message);

    if (USB_Serial_Echo && strlen((char *)USB_Receive_Buf) != 0)
    {
        strcat(USB_Transmit_Buf, " ");
        strcat(USB_Transmit_Buf, USB_Receive_Buf);
    }

    strcat(USB_Transmit_Buf, "\n\r");
    CDC_Transmit_FS((uint8_t *)USB_Transmit_Buf, strlen(USB_Transmit_Buf));
}