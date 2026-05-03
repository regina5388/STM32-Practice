#include "stm32f10x.h"      // STM32 standard peripheral definitions (registers, types)
#include "bsp_led.h"        // LED driver (GPIO config + control functions)
#include "bsp_key.h"        // Key (button) driver
#include "bsp_rccclkconfig.h" // System clock configuration (HSE/PLL setup)
#include "bsp_uart.h"       // UART driver (init + send/receive)
#include "bsp_dma_uart.h"
#include "bsp_i2c.h"
#include "bsp_spi_flash.h"

#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t* pBuffer1,uint8_t* pBuffer2, uint16_t BufferLength);

__IO u32 Device_ID = 0;
__IO u32 Flash_ID = 0;
__IO TestStatus TransferStatus1 = FAILED;

uint8_t Tx_Buffer[] = "Hello World\r\n";
uint8_t Rx_Buffer[BufferSize];

int main(void)
{

	LED_GPIO_Config();
	LED3_GPIO_Set();
	UART_Config();
	
	printf("\r\n This is for testing SPI Communication with Flash \r\n");
	
	SPI_FLASH_Init();
	
	Flash_ID = SPI_Flash_ReadID();
	Delay(200);
	
	Device_ID = SPI_Flash_ReadDeviceID();
	Delay(200);
	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", Flash_ID, Device_ID);
	
	
	if(Flash_ID==sFLASH_ID)
	{
		printf("\r\n Device Detected \r\n");
		
		SPI_Flash_SectorErase(FLASH_SectorToErase);
		
		SPI_Flash_BufferWrite(Tx_Buffer,FLASH_WriteAddress, BufferSize);
		printf("\r\n  Data written are: %s \r\n ", Tx_Buffer);
		
		SPI_Flash_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n  Data read are: %s \r\n ", Rx_Buffer);
		
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if(TransferStatus1 == PASSED)
		{
			LED2_GPIO_Set();
			printf("\r\n Test Succeed \r\n");
		}else{
			LED1_GPIO_Set();
			printf("\r\n Test Failed \r\n");
		}
	}
	else
	{
		LED1_GPIO_Set();
		printf("\r\n Device not found\r\n");
	
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

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}
