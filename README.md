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

## References

1. STM32 project template <https://github.com/Oct19/Bluepill-CubeMX-PlatformIO-Template>
2. STM32 Driver for OLED Display SSD1306 <https://github.com/afiskon/stm32-ssd1306>
