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

## To do

### Stepper

- Implement code from PiPlot

### OLED

- OLED need not be real time, 5Hz refresh rate is sufficient
- OLED should display warning, notification, send and receive message from USB serial port, and current state (decending priorities)

        If OLEDMessage.warning != NULL
        Display warning message
        OLEDMessage.warning = NULL
        osDelay(1000)

- Notification such as hit limit switch, black background last for 1s
- Serial message might be too frequent to display all. When this happens, display

        Rx: 12 messages
        Tx: 4 messages

- When no Rx Tx for 2 seconds, display current state of the machine, 10Hz refresh rate

        State: Zeroing
        Z: 12, Angle: 120

## Fixed Issue

- On board LED not working after RTC enabled: Need to disable RTC output
- FreeRTOS osDelay cause Hard Fault: increase task stack size
- OLED interrupt display conflict with display task, result in broken display. Change to constant refresh rate without interrupt(interrupt message does not show at once, but on the next screen refresh)

## References

1. STM32 project template <https://github.com/Oct19/Bluepill-CubeMX-PlatformIO-Template>
2. STM32 Driver for OLED Display SSD1306 <https://github.com/afiskon/stm32-ssd1306>
