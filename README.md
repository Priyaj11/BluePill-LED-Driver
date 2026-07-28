# BluePill-LED-Driver
A small, reusable LED driver for the STM32F103C8 "Blue Pill", written with direct
GPIO register access and wrapped in a clean, minimal API. The point of this
project was to go past a one-off blink and build the LED control as a proper
driver module that keeps the hardware details separate from the application code.

## API

| Function | Description |
| --- | --- |
| `led_init()` | Enables the GPIO port clock and configures PC13 as a push-pull output |
| `led_on()` | Turns the LED on (drives PC13 low, since the onboard LED is active-low) |
| `led_off()` | Turns the LED off (drives PC13 high) |
| `led_toggle()` | Flips the current LED state |
| `led_blink(rate_ms)` | Toggles once and waits `rate_ms`; call it in a loop for a steady blink |

## Hardware

Target: STM32F103C8 "Blue Pill", ARM Cortex-M3 at 72 MHz.
LED: the onboard LED wired to pin PC13. It is active-low, so the pin is driven
low to light the LED and high to turn it off.

## How it works

The driver talks to the GPIO at the register level rather than through the HAL,
so the hardware interaction stays explicit:

* Clock: sets the `IOPCEN` bit in `RCC->APB2ENR` to enable GPIO port C.
* Pin config: writes the mode and configuration bits for PC13 in `GPIOC->CRH`
  (push-pull output, 2 MHz).
* On and off: uses `GPIOC->BSRR` for an atomic, glitch-free set and reset of the pin.
* Toggle: XORs the PC13 bit in `GPIOC->ODR`.

Timing in `led_blink` uses `HAL_Delay`, which runs off the SysTick timer set up
during startup.

## Project structure

```
Core/
  Inc/led.h     LED driver interface
  Src/led.c     LED driver implementation (register level)
  Src/main.c    Initializes the driver and blinks the LED
```

## Build and run

Built with STM32CubeIDE. I ran it without physical hardware by simulating it in
the browser with Wokwi:

1. In STM32CubeIDE, enable Intel Hex output at
   `Project > Properties > C/C++ Build > Settings > MCU Post build outputs >
   Convert to Intel Hex file`, then build the project.
2. Open an STM32 Blue Pill project on wokwi.com.
3. Click into the code editor, press F1, choose
   "Load HEX File and Start Simulation", and select the generated
   `Debug/BluePill_Blink.hex`.

The onboard LED blinks every 500 ms.

## What this project covers

* GPIO configuration and control through direct register access
* A reusable driver with a clean interface, kept separate from application logic
* The header and source split, and a simple hardware abstraction layer
