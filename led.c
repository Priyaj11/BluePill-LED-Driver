led.c (Engine, contains all GPIO code, only this file knows the hardware)

#include "led.h"
#include "stm32f1xx_hal.h"   /* RCC, GPIOC, register defines, HAL_Delay */

#define LED_PIN  13          /* onboard LED is on PC13 */

void led_init(void)
{
    /* 1. Enable the clock to GPIO port C. Without this, writes do nothing. */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /* 2. Configure PC13 as a push-pull output at 2 MHz.
     *    On the F1, pins 8-15 are configured in CRH, 4 bits each.
     *    PC13's four bits start at bit (13 - 8) * 4 = 20. */
    GPIOC->CRH &= ~(0xFu << ((LED_PIN - 8) * 4));  /* clear PC13's config bits */
    GPIOC->CRH |=  (0x2u << ((LED_PIN - 8) * 4));  /* MODE=10 (2MHz), CNF=00 (PP) */

    /* 3. Start with the LED off (PC13 high, because it is active-low). */
    GPIOC->BSRR = (1u << LED_PIN);
}

void led_on(void)
{
    GPIOC->BSRR = (1u << (LED_PIN + 16));   /* upper half of BSRR resets -> pin low -> LED on */
}

void led_off(void)
{
    GPIOC->BSRR = (1u << LED_PIN);          /* lower half of BSRR sets -> pin high -> LED off */
}

void led_toggle(void)
{
    GPIOC->ODR ^= (1u << LED_PIN);          /* flip PC13 */
}

void led_blink(uint32_t rate_ms)
{
    led_toggle();
    HAL_Delay(rate_ms);                     /* uses SysTick, set up by HAL_Init() */
}
