#include "stm32f10x.h"      // STM32 standard peripheral definitions (registers, types)
#include "bsp_led.h"        // LED driver (GPIO config + control functions)
#include "bsp_key.h"        // Key (button) driver
#include "bsp_rccclkconfig.h" // System clock configuration (HSE/PLL setup)
#include "bsp_uart.h"       // UART driver (init + send/receive)
#include "bsp_dma_uart.h"
#include "bsp_i2c.h"

#define SOFT_DELAY Delay(0x0FFFFF)   // Simple blocking delay macro (busy-wait)
#define  EEP_Firstpage      0x00

uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];
uint8_t I2C_Test(void);


uint8_t I2C_Test(void)
{
	u16 i;
	EEPROM_INFO("Written Data: ");
	
	for (i = 0; i < 255; i++)
	{
		I2c_Buf_Write[i] = i;
		
		printf("0x%02X", I2c_Buf_Write[i]);
		
		if (i%16 == 15)
				printf("\n\r");
	}
	I2C_EE_BufferWrite(I2c_Buf_Write,EEP_Firstpage,256);
	EEPROM_INFO("Finished Writting");
	
	EEPROM_INFO("Data Read: ");
	I2C_EE_BufferRead(I2c_Buf_Read,EEP_Firstpage,256);
	
	for (i = 0; i < 255; i++)
	{
		if (I2c_Buf_Read[i]!= I2c_Buf_Write[i])
		{
			printf("0x%02X", I2c_Buf_Read[i]);
			EEPROM_ERROR("Results not matching");
			return 0;
		}
		
		printf("0x%02X", I2c_Buf_Read[i]);
		
		if (i%16 == 15)
				printf("\n\r");
	}
	
	
	EEPROM_INFO("Test Done");
	return 1;
}

int main(void)
{

	LED_GPIO_Config();
	LED3_GPIO_Set();
	UART_Config();
	
	printf("\r\n This is for testing I2C Communication with EEPROM \r\n");
	
	I2C_GPIO_Config();
	
	if(I2C_Test()==1)
	{
		LED2_GPIO_Set();
	}
	else
	{
		LED1_GPIO_Set();
		printf("\r\n Result not matching\r\n");
	
	}
//		printf("SCL=%d, SDA=%d\r\n",
//       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6),
//       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
	
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
