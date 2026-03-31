#include "stm32f10x.h"      // STM32 standard peripheral definitions (registers, types)
#include "bsp_led.h"        // LED driver (GPIO config + control functions)
#include "bsp_key.h"        // Key (button) driver
#include "bsp_rccclkconfig.h" // System clock configuration (HSE/PLL setup)
#include "bsp_uart.h"       // UART driver (init + send/receive)
#include "bsp_dma_uart.h"

#define SOFT_DELAY Delay(0x0FFFFF)   // Simple blocking delay macro (busy-wait)

// External function (likely defined in another file)
// Configures EXTI interrupt for keys
extern void EXTI_Key_Config(void);
extern uint8_t SendBuff[SENDBUFF_SIZE];

// Delay function prototype
// __IO = volatile ? prevents compiler optimization of the loop
void Delay(__IO u32 nCount);

int main(void)
{

		int i = 0;
    /* -------------------- Hardware Initialization -------------------- */

    LED_GPIO_Config();     // Configure LED GPIO pins (mode, speed, etc.)
    LED1_GPIO_Set();       // Turn ON LED1 (used as a power/ready indicator)
		UART_Config();
		DMA_UART_Config();      // Initialize UART (GPIO + USART + NVIC)

    /* -------------------- UART Test -------------------- */

    for (i = 0; i<SENDBUFF_SIZE; i++)
	{
		SendBuff[i] = 'P';
	}
	
	  USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);


    /* -------------------- Echo Loop -------------------- */

    for (;;)
    {
			GPIO_Toggle(LED1_GPIO_PORT,LED1_GPIO_PIN);
			Delay(0xFFFFFF);
    }
}

/* -------------------- Delay Function -------------------- */

// Simple software delay using a busy loop
// NOTE:
// - Not accurate (depends on compiler optimization & clock)
// - CPU is fully occupied during delay (not efficient)
// - Only suitable for simple demos/tests
void Delay(__IO u32 nCount)
{
    for (; nCount != 0; nCount--);
}
