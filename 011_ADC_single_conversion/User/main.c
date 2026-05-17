#include "stm32f10x.h"      // STM32 standard peripheral definitions (registers, types)
#include "bsp_led.h"        // LED driver (GPIO config + control functions)
#include "bsp_key.h"        // Key (button) driver
#include "bsp_rccclkconfig.h" // System clock configuration (HSE/PLL setup)
#include "bsp_uart.h"       // UART driver (init + send/receive)
#include "bsp_dma_uart.h"
#include "bsp_i2c.h"
#include "bsp_spi_flash.h"
#include "ff.h"
#include "bsp_adc.h"

extern __IO uint16_t ADC_ConvertedValue;

float ADC_ConvertedValueLocal;   

void Delay(__IO uint32_t nCount);


FATFS fs;
FIL fnew;
FRESULT res_flash;
UINT fnum;
BYTE buffer[1024] = {0};
BYTE textFileBuffer[] = "Hello World\r\n";
BYTE work[4096];


int main(void)
{

	LED_GPIO_Config();
	LED3_GPIO_Set(); //Blue
	LED1_GPIO_Set(); //Red
	LED2_GPIO_Set(); //Green
	UART_Config();
	
	ADCx_Init();
	ADC_NVIC_Config();
	
	printf("This is a ADC simgle conversion test\r\n");
	
	for(;;)
	{
		ADC_ConvertedValueLocal = (float)ADC_ConvertedValue/4096*3.3;
		
		printf("Current raw ADC value 0x%04X \r\n", ADC_ConvertedValue);
		printf("Current real ADC value %f V \r\n", ADC_ConvertedValueLocal);
		printf("\r\n\r\n");
		Delay(0xffff);
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

