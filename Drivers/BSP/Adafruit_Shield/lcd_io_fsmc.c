/*
 * @file lcd_io_fsmc.c
 * @brief ILI9341 low-level LCD IO implementation on FSMC 8080 bus
 */

#include <stdint.h>
#include "main.h"
#include "../Components/ili9341/ili9341.h"

#define LCD_FSMC_BASE_ADDR  ((uint32_t)0x6C000000U)
#define LCD_CMD_BASE        ((uint32_t)(LCD_FSMC_BASE_ADDR | 0x00001FFEU))
#define LCD_DATA_BASE       ((uint32_t)(LCD_FSMC_BASE_ADDR | 0x00002000U))

#define LCD_REG_ADDR        (*((volatile uint16_t *)LCD_CMD_BASE))
#define LCD_RAM_ADDR        (*((volatile uint16_t *)LCD_DATA_BASE))

void LCD_IO_Init(void)
{
  /* GPIO/FSMC are initialized by MX_GPIO_Init() and MX_FSMC_Init(). */
}

void LCD_IO_WriteData16(uint16_t RegValue)
{
  LCD_RAM_ADDR = RegValue;
}

void LCD_IO_WriteCmd(uint16_t Reg)
{
  LCD_REG_ADDR = Reg;
}

void LCD_IO_WriteReg(uint16_t Reg, uint16_t RegValue)
{
  LCD_REG_ADDR = Reg;
  LCD_RAM_ADDR = RegValue;
}

uint16_t LCD_IO_ReadReg(uint16_t RegValue)
{
  LCD_REG_ADDR = RegValue;
  return LCD_RAM_ADDR;
}

uint32_t LCD_IO_ReadDataN(uint16_t RegValue, uint8_t ReadSize)
{
  uint32_t read = 0U;

  LCD_IO_WriteCmd(RegValue);
  while(ReadSize--) {
    read = (read << 8) | (uint8_t)(LCD_RAM_ADDR & 0x00FFU);
  }

  return read;
}

void LCD_Delay(uint32_t delay)
{
  HAL_Delay(delay);
}
