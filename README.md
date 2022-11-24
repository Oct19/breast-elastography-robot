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

## To do

- Add VBAT for time keeping
- Reduce FLASH memory usage
- Add stepper motor control
- Add limit switch interrupt

## Fixed Issue

- On board LED not working after RTC enabled: Need to disable RTC output
- FreeRTOS osDelay cause Hard Fault: increase task stack size

## References

1. STM32 project template <https://github.com/Oct19/Bluepill-CubeMX-PlatformIO-Template>
2. STM32 Driver for OLED Display SSD1306 <https://github.com/afiskon/stm32-ssd1306>
