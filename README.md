# breast-elastography-robot

Hardware:

- STM32F103C8T6 board (Bluepill)
- ST-Link V2
- OLED Display SSD1306, width 128, height 32

## Bug

- If migrate private include from FreeRTOS.c to main.h, USBD_CDC_ItfTypeDef is not defined

- When message too long, OLED stuck

## TODO

- System block diagram
- Implement stepper code from iforce2d
- Serial Tx is currently not interrupt but a task
- Update USB related to template repo
- Update OLED_HandleTypeDef
- OLED: Tx Rx last for how long? If one of them keep updating, the other one should also stay
- For task display: in each loop, if limit switch interrupt happened after get_priority, the immediate display function erased notification message before passing on to the next loop. Tmeporay fix: clear message buffer at coorsponding switch case insead of clear all buffer at the end of the function. Same priority level messages still can miss

## Future features

- [ ] G code
- [ ] RoboDK
- [ ] Realtime stepper control
- [ ] USB data transmission

## Fixed Issue

- On board LED not working after RTC enabled: Need to disable RTC output
- FreeRTOS osDelay cause Hard Fault: increase task stack size
- OLED interrupt display conflict with display task, result in broken display. Change to constant refresh rate without interrupt(interrupt message does not show at once, but on the next screen refresh
- Sometimes PlatformIO need to build twice to build successfully, or after Clean All
- sprintf takes half of the flash size, because it brings the whole math implementation for floats

## References

- STM32 project template <https://github.com/Oct19/Bluepill-CubeMX-PlatformIO-Template>
- STM32 Driver for OLED Display SSD1306 <https://github.com/afiskon/stm32-ssd1306>
- STM32 timer interrupt <https://controllerstech.com/pwm-in-stm32/>
- Coordinated stepper motor control (arduino) <https://youtu.be/fHAO7SW-SZI>
- STM32 timer <https://youtu.be/VfbW6nfG4kw>
- Stepper motor configuration <https://github.com/brentnd/PiPlot>
- GRBL stm32 <https://github.com/dungjk/grbl-stm32>
- Stepper S-Curve <https://github.com/MGDG/SLineControl>
