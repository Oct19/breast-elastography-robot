/**
 * @file limit_switch.h
 * @author your name (you@domain.com)
 * @brief Limit switch configuration
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022
 *
 * Part number: FC-SP 307Z
 * Connection:
 * COLOR    NAME    CONNECTION
 * BROWN    5-24VDC 5V
 * BLUE     GND     GND
 * BLACK    NPN.NO  STM32:LIMIT_SWITCH
 * WHITE    NPN.NC  OPEN
 * Note:
 * For NO output, high voltage is detected
 * 
 */

#define PRESSED 0
#define LIMIT_SWITCH_STATUS HAL_GPIO_ReadPin(LIMIT_SWITCH_GPIO_Port, LIMIT_SWITCH_Pin)

// TODO: Update Angle limit switch connection
//                                 ^            +---------+--------+
//                         +---------------+    |    Z    |    A   |
//                         |      USB      |    |  LIMIT  |  LIMIT |
//                         |     SERIAL    |    | SWITCH  | SWITCH |
//                         |               |    |         |        |
//                         |               |    |         |        |
//                         |     STM32     |    |         |        |
//                         |    F103C8T6   |    | NO  NC  | NO  NC |
//                         |               |    +---------+--------+
//                         |               |       |         |   
//                         |               |       |         |   
//                         |            PB3|<------+         |   
//                         |               |                 |   
//                         |            XXX|<----------------+
//                         +---------------+                 
  