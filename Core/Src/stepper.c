

#include "robot.h"

void Stepper_Init(void)
{
  MOT_OFF;
  HAL_TIM_PWM_Start_IT(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start_IT(&htim1,TIM_CHANNEL_2);
}
