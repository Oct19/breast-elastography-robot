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

#ifdef _forceSensor_USE_ADC
uint32_t adc_val;
HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, 100); // wait for the conversion to complete
adc_val = HAL_ADC_GetValue(&hadc1);
sprintf(OLED.Positions, "%lu", adc_val);
HAL_ADC_Stop(&hadc1);

#endif /* _forceSensor_USE_ADC */

#ifdef _forceSensor_USE_RS485
/**
 * @brief Force sensor enable UART DMA data collection
 *
 */

/* Request reading for 9 channels, including CRC */
#ifdef _forceSensor_VALUE_LONG
uint8_t FS_Tx_Buf[8] = {0x01, 0x03, 0x03, 0x00, 0x00, 0x12, 0xC5, 0x83};
#endif
#ifdef _forceSensor_VALUE_FLOAT
uint8_t FS_Tx_Buf[8] = {0x01, 0x03, 0x01, 0x00, 0x00, 0x12, 0xC4, 0x3B};
#endif
#endif
