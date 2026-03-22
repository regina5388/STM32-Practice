#include "stm32f10x.h"    // STM32 standard peripheral definitions (registers, types)
#include "bsp_led.h"      // LED driver header (user-defined functions)

#define SOFT_DELAY Delay(0x0FFFFF)   // Software delay macro (simple busy-wait delay)

void Delay(__IO u32 nCount);         // Delay function declaration
                                    // __IO = volatile (prevents compiler optimization)

int main(void)
{
    LED_GPIO_Config();   // Initialize GPIO pins for LEDs (mode, speed, etc.)

    for(;;)              // Infinite loop (typical in embedded systems)
    {
        LED1_GPIO_Set();   // Turn on LED1 (depends on hardware: high or low level)
        SOFT_DELAY;        // Delay

        LED2_GPIO_Set();   // Turn on LED2
        SOFT_DELAY;

        LED3_GPIO_Set();   // Turn on LED3
        SOFT_DELAY;

        // Toggle LED3 (invert its current state)
        // If ON -> OFF, if OFF -> ON
        GPIO_Toggle(LED3_GPIO_PORT, LED3_GPIO_PIN);
        SOFT_DELAY;

        GPIO_Toggle(LED3_GPIO_PORT, LED3_GPIO_PIN);
        SOFT_DELAY;

        GPIO_Toggle(LED3_GPIO_PORT, LED3_GPIO_PIN);
        SOFT_DELAY;

        GPIO_Toggle(LED3_GPIO_PORT, LED3_GPIO_PIN);
        SOFT_DELAY;
    }
}

// Simple software delay function (busy-wait loop)
void Delay(__IO u32 nCount)
{
    for (; nCount != 0; nCount--);
    // Each loop consumes CPU cycles ? creates a rough time delay
}