/*
 * @file fsmc_8080.c
 * @brief FSMC 8080 low-level LCD IO implementation for ILI9341
 */

#include <stdint.h>

#include "ili9341.h"
#include "fsmc_8080.h"

/* HAL delay is provided by STM32 HAL runtime. */
extern void HAL_Delay(uint32_t Delay);

#define LCD_FSMC_BASE_ADDR  ((uint32_t)0x6C000000U)
#define LCD_CMD_BASE        ((uint32_t)(LCD_FSMC_BASE_ADDR | 0x00001FFEU))
#define LCD_DATA_BASE       ((uint32_t)(LCD_FSMC_BASE_ADDR | 0x00002000U))

#define LCD_REG_ADDR        (*((volatile uint16_t *)LCD_CMD_BASE))
#define LCD_RAM_ADDR        (*((volatile uint16_t *)LCD_DATA_BASE))

void LCD_IO_Init(void)
{
	/* GPIO/FSMC are initialized by MX_GPIO_Init() and MX_FSMC_Init(). */
}

void LCD_IO_WriteData(uint16_t RegValue)
{
	LCD_RAM_ADDR = RegValue;
}

void LCD_IO_WriteReg(uint8_t Reg)
{
	LCD_REG_ADDR = Reg;
}

uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize)
{
	uint32_t read = 0U;

	LCD_IO_WriteReg((uint8_t)RegValue);
	while (ReadSize--) {
		read = (read << 8) | (uint8_t)(LCD_RAM_ADDR & 0x00FFU);
	}

	return read;
}

void LCD_Delay(uint32_t delay)
{
	HAL_Delay(delay);
}