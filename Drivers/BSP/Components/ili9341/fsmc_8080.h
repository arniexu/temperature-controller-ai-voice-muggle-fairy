#ifndef FSMC_8080_H
#define FSMC_8080_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void LCD_IO_Init(void);
void LCD_IO_WriteData(uint16_t RegValue);
void LCD_IO_WriteReg(uint8_t Reg);
uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);
void LCD_Delay(uint32_t delay);

#ifdef __cplusplus
}
#endif

#endif /* FSMC_8080_H */
