#ifndef FSMC_8080_H
#define FSMC_8080_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/* HAL delay is provided by STM32 HAL runtime. */
extern void HAL_Delay(uint32_t Delay);

#define LCD_FSMC_BASE_ADDR  ((uint32_t)0x6C000000U)
#define LCD_CMD_BASE        ((uint32_t)(LCD_FSMC_BASE_ADDR | 0x00001ffeU))
#define LCD_DATA_BASE       ((uint32_t)(LCD_FSMC_BASE_ADDR | 0x00002000U))

#define LCD_REG_ADDR        (*((volatile uint16_t *)LCD_CMD_BASE))
#define LCD_RAM_ADDR        (*((volatile uint16_t *)LCD_DATA_BASE))

void LCD_IO_Init(void);
void LCD_IO_WriteData(uint16_t RegValue);
void LCD_IO_WriteReg(uint8_t Reg);
uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);
void LCD_Delay(uint32_t delay);
uint16_t LCD_IO_ReadChipID(void);
void LCD_IO_WriteData2Cmd(uint16_t RegValue,uint16_t d);
uint16_t LCD_IO_ReadDataFromReg(uint16_t reg);

#ifdef __cplusplus
}
#endif

#endif /* FSMC_8080_H */
