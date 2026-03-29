#include "stm32f10x.h"      // STM32 standard peripheral definitions (registers, types)
#include "bsp_led.h"        // LED driver (GPIO config + control functions)
#include "bsp_key.h"        // Key (button) driver
#include "bsp_rccclkconfig.h" // System clock configuration (HSE/PLL setup)
#include "bsp_uart.h"       // UART driver (init + send/receive)

#define SOFT_DELAY Delay(0x0FFFFF)   // Simple blocking delay macro (busy-wait)

// External function (likely defined in another file)
// Configures EXTI interrupt for keys
extern void EXTI_Key_Config(void);

// Delay function prototype
// __IO = volatile ? prevents compiler optimization of the loop
void Delay(__IO u32 nCount);

int main(void)
{
    char ch;   // Variable to store received UART character

    /* -------------------- Hardware Initialization -------------------- */

    LED_GPIO_Config();     // Configure LED GPIO pins (mode, speed, etc.)
    LED1_GPIO_Set();       // Turn ON LED1 (used as a power/ready indicator)

    UART_Config();         // Initialize UART (GPIO + USART + NVIC)

    /* -------------------- UART Test -------------------- */

    // Send a single byte (0x12) through UART
    Uart_SendByte(DEBUG_USARTx, 0x12);

    // Send a string through UART (custom function)
    Uart_SendString(DEBUG_USARTx, "Hello, this is a UART TEST\n");

    // Test printf redirection (via fputc or _write)
    printf("Test C standard library\n");

    /* -------------------- Echo Loop -------------------- */

    for (;;)
    {
        // Blocking read: waits until a character is received from UART
        ch = getchar();

        // Echo received character back to terminal
        printf("Received: %c\n", ch);
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
