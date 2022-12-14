/*!
	@file   tsk_uartparser.c
	@brief  <brief description here>

	@t.odo	-
	@dev    io.d (devcoons.com)
*/
/******************************************************************************
 * Preprocessor Definitions & Macros
 ******************************************************************************/

#define UART_DMA_BUFFER_SIZE 32
#define PARSER_MESSAGE_LIST_SIZE 4
#define PARSER_MESSAGE_SIZE 16

/******************************************************************************
 * Includes
 ******************************************************************************/

#include "main.h"
#include "FreeRTOS.h"
#include "usart.h"

/******************************************************************************
 * Enumerations, structures & Variables
 ******************************************************************************/

static uint8_t buffer[UART_DMA_BUFFER_SIZE];
static uint8_t msg_list[PARSER_MESSAGE_LIST_SIZE][PARSER_MESSAGE_SIZE];

/******************************************************************************
 * Declaration | Static Functions
 ******************************************************************************/

/******************************************************************************
 * Definition  | Static Functions
 ******************************************************************************/

/******************************************************************************
 * Definition  | Public Functions
 ******************************************************************************/

void UARTParser(void *arguments)
{
	size_t dma_head = 0, dma_tail = 0;
	size_t cur_msg_sz = 0;
	size_t cur_msg = 0;
	uint8_t found = 0;

	HAL_UART_Receive_DMA(&huart1, buffer, UART_DMA_BUFFER_SIZE);

	for (;;)
	{
		do
		{
			__disable_irq();
			dma_tail = UART_DMA_BUFFER_SIZE - huart1.hdmarx->Instance->CNDTR;
			__enable_irq();

			if (dma_tail != dma_head)
			{
				if (dma_head < dma_tail)
				{
					for (register size_t i = dma_head; i < dma_tail; i++)
					{
						found = (found == 0 && buffer[i] == '\r')	? 1
								: (found == 1 && buffer[i] == '\n') ? 2
																	: 0;
						msg_list[cur_msg][cur_msg_sz++] = buffer[i];

						if (found == 2)
						{
							cur_msg = cur_msg == PARSER_MESSAGE_LIST_SIZE - 1 ? 0 : cur_msg + 1;
							memset(msg_list[cur_msg], 0, PARSER_MESSAGE_SIZE);
							cur_msg_sz = 0;
						}
					}
				}
				else
				{
					for (register size_t i = dma_head; i < UART_DMA_BUFFER_SIZE; i++)
					{
						found = (found == 0 && buffer[i] == '\r')	? 1
								: (found == 1 && buffer[i] == '\n') ? 2
																	: 0;
						msg_list[cur_msg][cur_msg_sz++] = buffer[i];

						if (found == 2)
						{
							cur_msg = cur_msg == PARSER_MESSAGE_LIST_SIZE - 1 ? 0 : cur_msg + 1;
							memset(msg_list[cur_msg], 0, PARSER_MESSAGE_SIZE);
							cur_msg_sz = 0;
						}
					}
					for (register size_t i = 0; i < dma_tail; i++)
					{
						found = (found == 0 && buffer[i] == '\r')	? 1
								: (found == 1 && buffer[i] == '\n') ? 2
																	: 0;

						msg_list[cur_msg][cur_msg_sz++] = buffer[i];

						if (found == 2)
						{
							cur_msg = cur_msg == PARSER_MESSAGE_LIST_SIZE - 1 ? 0 : cur_msg + 1;
							memset(msg_list[cur_msg], 0, PARSER_MESSAGE_SIZE);
							cur_msg_sz = 0;
						}
					}
				}
				dma_head = dma_tail;
			}
		} while (dma_head != (UART_DMA_BUFFER_SIZE - huart1.hdmarx->Instance->CNDTR));

		HAL_UART_Transmit(&huart1, msg_list[0], sizeof(msg_list[0]), 100);
		osDelay(25);
	}
}

/******************************************************************************
 * EOF - NO CODE AFTER THIS LINE
 ******************************************************************************/
