led.h (Public interface, Declares the functions for use in other source files, No implementations only declerations)
#ifndef LED_H
#define LED_H
 
#include <stdint.h>
 
/* LED driver for the onboard LED (PC13) on the STM32F103C8 "Blue Pill". */
 
void led_init(void);                 /* configure PC13 as an output */
void led_on(void);                   /* LED on  (PC13 driven low, active-low) */
void led_off(void);                  /* LED off (PC13 driven high) */
void led_toggle(void);               /* flip the current LED state */
void led_blink(uint32_t rate_ms);    /* toggle once, then wait rate_ms */
 
#endif /* LED_H */
 
