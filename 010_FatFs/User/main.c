#include "stm32f10x.h"      // STM32 standard peripheral definitions (registers, types)
#include "bsp_led.h"        // LED driver (GPIO config + control functions)
#include "bsp_key.h"        // Key (button) driver
#include "bsp_rccclkconfig.h" // System clock configuration (HSE/PLL setup)
#include "bsp_uart.h"       // UART driver (init + send/receive)
#include "bsp_dma_uart.h"
#include "bsp_i2c.h"
#include "bsp_spi_flash.h"
#include "ff.h"

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
	LED3_GPIO_Set();
	UART_Config();
	
	printf("\r\n This is for testing SPI Communication with Flash \r\n");
	
	res_flash = f_mount(&fs,"1:",1);
	
	if(res_flash==FR_NO_FILESYSTEM)
	{
		printf("\r\n No file system. Formatting device\r\n");
		
		res_flash = f_mkfs("1:", NULL, work, sizeof(work));
			
		if(res_flash == FR_OK)
		{
			printf("\r\n Formatting succeeded\r\n");
			res_flash = f_mount(NULL,"1:",1);
			res_flash = f_mount(&fs,"1:",1);
		}
		else
		{
			LED1_GPIO_Set();
			printf("\r\n Formatting failed (%d)\r\n", res_flash);
			while(1);
		}
		
	}
	else if (res_flash != FR_OK)
	{
		LED1_GPIO_Set();
		printf("\r\n Mount Failed (%d)\r\n", res_flash);
		while (1);
	
	}
	else
	{
		printf("\r\n Mount Success\r\n");
	}
	
	//Write test
	printf("\r\n This is for testing write \r\n");
	res_flash = f_open(&fnew,"1:test.txt", FA_WRITE | FA_CREATE_ALWAYS );
	if (res_flash == FR_OK)
	{
		printf("\r\n Created test file, now try to write\r\n");
		res_flash = f_write(&fnew, textFileBuffer, sizeof(textFileBuffer), &fnum);
		if (res_flash == FR_OK)
		{
			printf("\r\n Data write succeed, wrote %d bytes\r\n", fnum);
		}
		else
		{
			printf("\r\n Data write failed, wrote %d bytes\r\n (%d)", fnum, res_flash);
		}
		
		f_close(&fnew);
	}
	else
	{
		LED1_GPIO_Set();
		printf("\r\n File Open Failed (%d)\r\n", res_flash);
	}
	
	//Read test
	printf("\r\n This is for testing read \r\n");
	res_flash = f_open(&fnew,"1:test.txt", FA_READ | FA_OPEN_EXISTING );
	if (res_flash == FR_OK)
	{
		LED2_GPIO_Set();
		printf("\r\n File open succeed \r\n");
		res_flash = f_read(&fnew, buffer, sizeof(buffer), &fnum);
		
		if(res_flash == FR_OK)
		{
			printf("\r\n Data Read: %s \r\n", buffer);
			printf("\r\n Data read succeed, read %d bytes\r\n", fnum);
		}
		else
		{
			LED1_GPIO_Set();
			printf("\r\n Data Read Failed (%d)\r\n", res_flash);
		}
		
	}
	else
	{
		LED1_GPIO_Set();
		printf("\r\n File open failed \r\n");
	}
	
	f_close(&fnew);
	
	res_flash = f_mount(NULL,"1:",1);
	
	
	
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
