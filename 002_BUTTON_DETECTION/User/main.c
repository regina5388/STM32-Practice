#include "stm32f10x.h"    // STM32 standard peripheral definitions (registers, types)
#include "bsp_led.h"      // LED driver header (user-defined functions)
#include "bsp_key.h" 

#define SOFT_DELAY Delay(0x0FFFFF)   // Software delay macro (simple busy-wait delay)

void Delay(__IO u32 nCount);         // Delay function declaration
                                    // __IO = volatile (prevents compiler optimization)

int main(void)
{
    LED_GPIO_Config();   // Initialize GPIO pins for LEDs (mode, speed, etc.)
		LED1_GPIO_Set();
		KEY_GPIO_Config();
	
    for(;;)              // Infinite loop (typical in embedded systems)
    {
        if(KEY_SCAN(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
				{
					GPIO_Toggle(LED1_GPIO_PORT, LED1_GPIO_PIN);
				}
					
				
				if(KEY_SCAN(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
			  {
					GPIO_Toggle(LED2_GPIO_PORT, LED2_GPIO_PIN);
				}
					
    }
}

// Simple software delay function (busy-wait loop)
void Delay(__IO u32 nCount)
{
    for (; nCount != 0; nCount--);
    // Each loop consumes CPU cycles ? creates a rough time delay
}
