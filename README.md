# breast-elastography-robot

Hardware:

- STM32F103C8T6 board (Bluepill)
- ST-Link V2
- OLED Display SSD1306, width 128, height 32

Connection:

- OLEP Display SSD1306: GND -> GND
                        VCC -> 3.3
                        SCL -> B10
                        SDA -> B11
                        I
- Serial Port: On board USB port

## Bug

### General

- If migrate private include from FreeRTOS.c to main.h, USBD_CDC_ItfTypeDef is not defined

- When message too long, OLED stuck

## To do

### G code

### RoboDK

### Stepper

- Implement stepper code from GRBL

### Serial Port

- Tx is currently not interrupt but a task

### Git

- Update USB related to template repo

### OLED

- Update OLED_HandleTypeDef
- Tx Rx last for how long? If one of them keep updating, the other one should also stay
- For task display: in each loop, if limit switch interrupt happened after get_priority, the immediate display function erased notification message before passing on to the next loop. Tmeporay fix: clear message buffer at coorsponding switch case insead of clear all buffer at the end of the function. Same priority level messages still can miss

## Fixed Issue

- On board LED not working after RTC enabled: Need to disable RTC output
- FreeRTOS osDelay cause Hard Fault: increase task stack size
- OLED interrupt display conflict with display task, result in broken display. Change to constant refresh rate without interrupt(interrupt message does not show at once, but on the next screen refresh
- RTC increase flash size by a lot, removed since calender feature is not necessary
- Sometimes PlatformIO need to build twice to build successfully, or after Clean All
- sprintf takes half of the flash size, because it brings the whole math implementation for floats

## Notes

### Timer

- Tout = (Prescaler x Preload)/Clock Frequency
- 0.5s = (   1000   x 36000  )/     72MHz
- The value for prescaler and ARR should be 1 less than the actual value
- Hence in STM32CubeMX, set prescaler to `1000-1`, and Preload to `36000-1`

## References

- STM32 project template <https://github.com/Oct19/Bluepill-CubeMX-PlatformIO-Template>
- STM32 Driver for OLED Display SSD1306 <https://github.com/afiskon/stm32-ssd1306>
- STM32 timer interrupt <https://controllerstech.com/pwm-in-stm32/>
- STM32 timer <https://youtu.be/VfbW6nfG4kw>
- Stepper motor configuration <https://github.com/brentnd/PiPlot>
- GRBL stm32 <https://github.com/dungjk/grbl-stm32>
