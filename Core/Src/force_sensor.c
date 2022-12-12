/**
 * @file force_sensor.c
 * @author Cai Dingkun <caidingkun@outlook.com>
 * @brief Code for force sensor and multi-channel transmitter
 * @version 0.1
 * @date 2022-12-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "main.h"

extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;

#ifdef _FS_USE_ADC




#endif /* _FS_USE_ADC */

#ifdef _FS_USE_RS485
/**
 * @brief Force sensor enable UART DMA data collection
 *
 */

/* Request reading for 9 channels, including CRC */
#ifdef _FS_VALUE_LONG
uint8_t FS_Tx_Buf[8] = {0x01, 0x03, 0x03, 0x00, 0x00, 0x12, 0xC5, 0x83};
#endif
#ifdef _FS_VALUE_FLOAT
uint8_t FS_Tx_Buf[8] = {0x01, 0x03, 0x01, 0x00, 0x00, 0x12, 0xC4, 0x3B};
#endif
#endif
