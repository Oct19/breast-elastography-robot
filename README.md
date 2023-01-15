# breast-elastography-robot

Hardware:

- STM32F103C8T6 board (Bluepill)
- ST-Link V2
- Stepper Motor Driver UM242 (TB6600 family)
- Stepper Motor from UMotBug

## Know Issues

Unable to compile in some Windows environment

## CONNECTION

STEP DRIVER 1 controls 4 platform raising motors, 2 of those connect in reverse (switch RED with BLACK, BLUE with GREEN)

* STM PB14 (ENA) - STEP DRIVER 1 ENA-
* STM PB15 (DIR1) - STEP DRIVER 1 DIR-
* STM PA8 (PUL1) - STEP DRIVER 1 PUL-
* STEP DRIVER ENA+ - 5V
* STEP DRIVER DIR+ - 5V
* STEP DRIVER PUL+ - 5V
* STEP DRIVER A+ - MOTOR RED
* STEP DRIVER A- - MOTOR BLUE
* STEP DRIVER B+ - MOTOR BLACK
* STEP DRIVER B- - MOTOR GREEN

## Fixed Issue

- On board LED not working after RTC enabled: Need to disable RTC output
- FreeRTOS osDelay cause Hard Fault: increase task stack size
- OLED interrupt display conflict with display task, result in broken display. Change to constant refresh rate without interrupt(interrupt message does not show at once, but on the next screen refresh
- Sometimes PlatformIO need to build twice to build successfully, or after Clean All
- sprintf print use big flash size. Switch to portable <inttypes.h>

  ```c
  uint32_t value = 123;
  char textToWrite[10];
  sprintf(textToWrite,"%" PRIu32, value);
  ```
- [Linux RS485 to USB device cannot connect]([https://unix.stackexchange.com/questions/670636/unable-to-use-usb-dongle-based-on-usb-serial-converter-chip](https://unix.stackexchange.com/questions/670636/unable-to-use-usb-dongle-based-on-usb-serial-converter-chip))

## References

- [STM32 project template](https://github.com/Oct19/Bluepill-CubeMX-PlatformIO-Template)
- [STM32 timer interrupt]([https://controllerstech.com/pwm-in-stm32/](https://controllerstech.com/pwm-in-stm32/))
- [Coordinated stepper motor control (arduino)]([https://youtu.be/fHAO7SW-SZI](https://youtu.be/fHAO7SW-SZI))
- [STM32 timer]([https://youtu.be/VfbW6nfG4kw](https://youtu.be/VfbW6nfG4kw))
- [Stepper motor configuration]([https://github.com/brentnd/PiPlot](https://github.com/brentnd/PiPlot))
- [GRBL stm32]([https://github.com/dungjk/grbl-stm32](https://github.com/dungjk/grbl-stm32))
- [Stepper S-Curve]([https://github.com/MGDG/SLineControl](https://github.com/MGDG/SLineControl))
- [UART with DMA and FreeRTOS](https://www.devcoons.com/stm32-uart-receive-unknown-size-data-using-dma-and-freertos/)
