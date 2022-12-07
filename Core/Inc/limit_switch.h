/**
 * @file limit_switch.h
 * @author Cai Dingkun <caidingkun@outlook.com>
 * @brief Limit switch configuration
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022
 *
 * Part number: FC-SP 307Z
 * For NO output, high voltage is detected
 * 
 */

#define PRESSED 0
#define LIMIT_SWITCH_STATUS HAL_GPIO_ReadPin(LIMIT_SWITCH_GPIO_Port, LIMIT_SWITCH_Pin)