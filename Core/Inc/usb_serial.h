/**
 * @file    usb_serial.h
 * @author  Cai Dingkun <caidingkun@outlook.com>
 * @brief   This file contains all the function prototypes for
 *          the usb_serial.c file
 * @version 0.1
 * @date    2022-11-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __USB_SERIAL_H__
#define __USB_SERIAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define USB_Serial_Timestamp        1       // yyyy-mm-dd hh:mm:ss
#define USB_Serial_Echo             1       // Repeat last received message

/**
 * @brief Storing recieved message through USB serial port
 * 
 */
extern char USB_Receive_Buf[64];

/**
 * @brief Stroing message to be transmited through USB serial port
 * 
 */
extern char USB_Transmit_Buf[128];

/**
 * @brief Combine message, and transmit through USB serial port
 * 
 */
void usb_serial_update(void);

#ifdef __cplusplus
}
#endif
#endif /*__ USB_SERIAL_H__ */
