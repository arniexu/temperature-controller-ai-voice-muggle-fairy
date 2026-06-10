/*
 * @file fsmc_8080.c
 * @brief FSMC 8080 low-level LCD IO implementation for ILI9341
 */

#include <stdint.h>

#include "fsmc_8080.h"


void LCD_IO_Init(void)
{
	/* GPIO/FSMC are initialized by MX_GPIO_Init() and MX_FSMC_Init(). */
}

void LCD_IO_WriteData(uint16_t RegValue)
{
	LCD_RAM_ADDR = RegValue;
}

void LCD_IO_WriteData2Cmd(uint16_t RegValue,uint16_t d)
{
	LCD_REG_ADDR = RegValue;
	LCD_RAM_ADDR = d;
}

void LCD_IO_WriteReg(uint8_t Reg)
{
	LCD_REG_ADDR = Reg;
}

uint16_t LCD_IO_ReadDataFromReg(uint16_t reg)
{
	LCD_REG_ADDR = reg;
	// DELAY 4 US
	return LCD_RAM_ADDR;
}

uint16_t LCD_IO_ReadChipID(void)
{
	uint16_t id = 0;
	LCD_REG_ADDR = 0xd3;
	volatile uint16_t b1 = LCD_RAM_ADDR & 0xff;
	volatile uint16_t b2 = LCD_RAM_ADDR & 0xff;
	volatile uint16_t b3 = LCD_RAM_ADDR & 0xff;
	volatile uint16_t b4 = LCD_RAM_ADDR & 0xff;

	return (b3 << 8)|b4;
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
