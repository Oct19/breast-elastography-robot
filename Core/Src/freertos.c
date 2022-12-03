/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "OLEDdisplay.h"
#include "usb_serial.h"
#include "usbd_cdc_if.h"
#include "stepper.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for blink */
osThreadId_t blinkHandle;
const osThreadAttr_t blink_attributes = {
  .name = "blink",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for display */
osThreadId_t displayHandle;
const osThreadAttr_t display_attributes = {
  .name = "display",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for USBserial */
osThreadId_t USBserialHandle;
const osThreadAttr_t USBserial_attributes = {
  .name = "USBserial",
  .stack_size = 220 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Stepper */
osThreadId_t StepperHandle;
const osThreadAttr_t Stepper_attributes = {
  .name = "Stepper",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for onceTimer */
osTimerId_t onceTimerHandle;
const osTimerAttr_t onceTimer_attributes = {
  .name = "onceTimer"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartBlink(void *argument);
void StartDisplay(void *argument);
void StartUSBserial(void *argument);
void StartStepper(void *argument);
void Callback01(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of onceTimer */
  onceTimerHandle = osTimerNew(Callback01, osTimerOnce, NULL, &onceTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of blink */
  blinkHandle = osThreadNew(StartBlink, NULL, &blink_attributes);

  /* creation of display */
  displayHandle = osThreadNew(StartDisplay, NULL, &display_attributes);

  /* creation of USBserial */
  USBserialHandle = osThreadNew(StartUSBserial, NULL, &USBserial_attributes);

  /* creation of Stepper */
  StepperHandle = osThreadNew(StartStepper, NULL, &Stepper_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartBlink */
/**
 * @brief  Function implementing the blink thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartBlink */
void StartBlink(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartBlink */
  /* Infinite loop */
  for (;;)
  {
    // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    osDelay(50);
  }
  /* USER CODE END StartBlink */
}

/* USER CODE BEGIN Header_StartDisplay */
/**
 * @brief Function implementing the display thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDisplay */
void StartDisplay(void *argument)
{
  /* USER CODE BEGIN StartDisplay */
  /* Infinite loop */
  for (;;)
  {
    OLED_get_priority();
    if (OLED.priority >= OLED.priority_old)
    {
      OLED_display_message();
      if (OLED.priority >= OLED.priority_old)
        osTimerStart(onceTimerHandle, 1000);
    }
    osDelay(100);
  }
  /* USER CODE END StartDisplay */
}

/* USER CODE BEGIN Header_StartUSBserial */
/**
 * @brief Function implementing the USBserial thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartUSBserial */
void StartUSBserial(void *argument)
{
  /* USER CODE BEGIN StartUSBserial */
  /* Infinite loop */
  for (;;)
  {
    USB_Transmit_Hello();
    osDelay(500);
  }
  /* USER CODE END StartUSBserial */
}

/* USER CODE BEGIN Header_StartStepper */
/**
 * @brief Function implementing the Stepper thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartStepper */
void StartStepper(void *argument)
{
  /* USER CODE BEGIN StartStepper */
  
  enable_driver();
  /* Infinite loop */
  for (;;)
  {
    for (int y = 0; y < 8; y = y + 1) // 8 times
    {
      step_test(100, 1, 1); // 25 steps (45 degrees) CCV
      osDelay(500);
    }
  }
  /* USER CODE END StartStepper */
}

/* Callback01 function */
void Callback01(void *argument)
{
  /* USER CODE BEGIN Callback01 */
  OLED.priority_old = 0;
  /* USER CODE END Callback01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

