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

char USB_Transmit_Buf[64];

void USB_Transmit_Hello(void)
{

    // Print Hello message for 10 seconds
    char message[12];
    memset(message, '\0', sizeof(message));
    int t = HAL_GetTick();
    if (t < 20000)
    {
        strcpy(message, "Hello ");
        char t_str[5];
        itoa(t, t_str, 10); // convert to string [buf]
        strcat(message, t_str);
        strcat(message, "\n\r");
    }

    memset(USB_Transmit_Buf, 0, sizeof(USB_Transmit_Buf)); // clear previous message
    memcpy(USB_Transmit_Buf, message, strlen(message));
    CDC_Transmit_FS((uint8_t *)USB_Transmit_Buf, strlen(USB_Transmit_Buf));

    // Update OLED message
    memset(OLED.Tx, '\0', sizeof(OLED.Tx));
    memcpy(OLED.Tx, USB_Transmit_Buf, strlen(USB_Transmit_Buf));
}

/**
 * @brief Actions after receiving USB serial message
 *
 */
void USB_Receive(uint8_t *Buf, uint32_t *Len)
{
    // Echo back
    if (USB_Serial_Echo)
        CDC_Transmit_FS(Buf, (uint8_t)*Len);

    // Update OLED message
    memset(OLED.Rx, '\0', sizeof(OLED.Rx));
    memcpy(OLED.Rx, Buf, strlen((char *)Buf));

    // Read command with delim
    // This will swap space with NULL character '\0'
    // char delim[] = " ";
    // char *ptr = strtok(Buf, delim);

    // while (ptr != NULL)
    // {
    //     printf("'%s'\n", ptr);
    //     ptr = strtok(NULL, delim);
    // }
}
