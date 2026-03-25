#include "stm32f10x.h"    // STM32 standard peripheral definitions (registers, types)
#include "bsp_led.h"      // LED driver header (user-defined functions)
#include "bsp_key.h" 
#include "bsp_rccclkconfig.h"

#define SOFT_DELAY Delay(0x0FFFFF)   // Software delay macro (simple busy-wait delay)

void Delay(__IO u32 nCount);         // Delay function declaration
                                    // __IO = volatile (prevents compiler optimization)

int main(void)
{
    LED_GPIO_Config();   // Initialize GPIO pins for LEDs (mode, speed, etc.)
		LED1_GPIO_Set();
	
		//72MHz SysClk
		HSE_SetSysClock(RCC_PLLMul_9);
		
	/**
	*     @arg RCC_MCO_NoClock: No clock selected
  *     @arg RCC_MCO_SYSCLK: System clock selected
  *     @arg RCC_MCO_HSI: HSI oscillator clock selected
  *     @arg RCC_MCO_HSE: HSE oscillator clock selected
  *     @arg RCC_MCO_PLLCLK_Div2: PLL clock divided by 2 selected
	*/
		MCO_GPIO_Config();
    RCC_MCOConfig(RCC_MCO_SYSCLK);
	
    for(;;)              // Infinite loop (typical in embedded systems)
    {
			GPIO_Toggle(LED1_GPIO_PORT, LED1_GPIO_PIN);
			Delay(0x0FFFFF);
		}
		
}

// Simple software delay function (busy-wait loop)
void Delay(__IO u32 nCount)
{
    for (; nCount != 0; nCount--);
    // Each loop consumes CPU cycles ? creates a rough time delay
}
