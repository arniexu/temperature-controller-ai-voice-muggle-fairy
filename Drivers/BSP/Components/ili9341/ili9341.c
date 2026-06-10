/**
 ******************************************************************************
 * @file    ili9341.c
 * @author  MCD Application Team
 * @version V1.0.2
 * @date    02-December-2014
 * @brief   This file includes the LCD driver for ILI9341 LCD.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ili9341.h"
#include "fsmc_8080.h"

/** @addtogroup BSP
 * @{
 */

/** @addtogroup Components
 * @{
 */

/** @addtogroup ILI9341
 * @brief This file provides a set of functions needed to drive the
 *        ILI9341 LCD.
 * @{
 */

/** @defgroup ILI9341_Private_TypesDefinitions
 * @{
 */
/**
 * @}
 */

/** @defgroup ILI9341_Private_Defines
 * @{
 */
/**
 * @}
 */

/** @defgroup ILI9341_Private_Macros
 * @{
 */
/**
 * @}
 */

/** @defgroup ILI9341_Private_Variables
 * @{
 */

LCD_DrvTypeDef ili9341_drv = {
    ili9341_Init,
    ili9341_ReadID,
    ili9341_DisplayOn,
    ili9341_DisplayOff,
    0,
    0,
    0,
    0,
    0,
    0,
    ili9341_GetLcdPixelWidth,
    ili9341_GetLcdPixelHeight,
    0,
    0,
};

/**
 * @}
 */

/** @defgroup ILI9341_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @defgroup ILI9341_Private_Functions
 * @{
 */

/**
 * @brief  Power on the LCD.
 * @param  None
 * @retval None
 */
void ili9341_Init(void) {
  /* Initialize ILI9341 low level bus layer ----------------------------------*/
  LCD_IO_Init();

#if 1
	ili9341_WriteReg(0x11);
	LCD_Delay(120);
	ili9341_WriteReg(0x36);
	ili9341_WriteData(0x48);
	
		ili9341_WriteReg(0x3a);
	ili9341_WriteData(0x55);
	
		ili9341_WriteReg(0x29);
LCD_Delay(20);

	ili9341_WriteReg(0x2a);
	ili9341_WriteData(0x0);
	ili9341_WriteData(0x0);
	ili9341_WriteData(0x0);
	ili9341_WriteData(0xef);

	ili9341_WriteReg(0x2b);
	ili9341_WriteData(0x0);
	ili9341_WriteData(0x0);
	ili9341_WriteData(0x1);
	ili9341_WriteData(0x3f);

	ili9341_WriteReg(0x2c);

// for(int i = 0; i < 240*320; i++){
// 	ili9341_WriteData(0xf800
// }
LCD_Delay(10);
#else
  /* Configure LCD */
  LCD_REG_ADDR = 0xCF;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0xC1;
  LCD_RAM_ADDR = 0X30;
  LCD_REG_ADDR = 0xED;
  LCD_RAM_ADDR = 0x64;
  LCD_RAM_ADDR = 0x03;
  LCD_RAM_ADDR = 0X12;
  LCD_RAM_ADDR = 0X81;
  LCD_REG_ADDR = 0xE8;
  LCD_RAM_ADDR = 0x85;
  LCD_RAM_ADDR = 0x10;
  LCD_RAM_ADDR = 0x7A;
  LCD_REG_ADDR = 0xCB;
  LCD_RAM_ADDR = 0x39;
  LCD_RAM_ADDR = 0x2C;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x34;
  LCD_RAM_ADDR = 0x02;
  LCD_REG_ADDR = 0xF7;
  LCD_RAM_ADDR = 0x20;
  LCD_REG_ADDR = 0xEA;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x00;
  LCD_REG_ADDR = 0xC0;
  LCD_RAM_ADDR = 0x1B;
  LCD_REG_ADDR = 0xC1;
  LCD_RAM_ADDR = 0x01;
  LCD_REG_ADDR = 0xC5;
  LCD_RAM_ADDR = 0x30;
  LCD_RAM_ADDR = 0x30;
  LCD_REG_ADDR = 0xC7;
  LCD_RAM_ADDR = 0XB7;
  LCD_REG_ADDR = 0x36;
  LCD_RAM_ADDR = 0x48;

  LCD_REG_ADDR = 0x3A;
  LCD_RAM_ADDR = 0x55;
  LCD_REG_ADDR = 0xB1;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x1A;
  LCD_REG_ADDR = 0xB6;
  LCD_RAM_ADDR = 0x0A;
  LCD_RAM_ADDR = 0xA2;
  LCD_REG_ADDR = 0xF2;
  LCD_RAM_ADDR = 0x00;
  LCD_REG_ADDR = 0x26;
  LCD_RAM_ADDR = 0x01;
  LCD_REG_ADDR = 0xE0;
  LCD_RAM_ADDR = 0x0F;
  LCD_RAM_ADDR = 0x2A;
  LCD_RAM_ADDR = 0x28;
  LCD_RAM_ADDR = 0x08;
  LCD_RAM_ADDR = 0x0E;
  LCD_RAM_ADDR = 0x08;
  LCD_RAM_ADDR = 0x54;
  LCD_RAM_ADDR = 0XA9;
  LCD_RAM_ADDR = 0x43;
  LCD_RAM_ADDR = 0x0A;
  LCD_RAM_ADDR = 0x0F;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x00;
  LCD_REG_ADDR = 0XE1;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x15;
  LCD_RAM_ADDR = 0x17;
  LCD_RAM_ADDR = 0x07;
  LCD_RAM_ADDR = 0x11;
  LCD_RAM_ADDR = 0x06;
  LCD_RAM_ADDR = 0x2B;
  LCD_RAM_ADDR = 0x56;
  LCD_RAM_ADDR = 0x3C;
  LCD_RAM_ADDR = 0x05;
  LCD_RAM_ADDR = 0x10;
  LCD_RAM_ADDR = 0x0F;
  LCD_RAM_ADDR = 0x3F;
  LCD_RAM_ADDR = 0x3F;
  LCD_RAM_ADDR = 0x0F;
  LCD_REG_ADDR = 0x2B;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x01;
  LCD_RAM_ADDR = 0x3f;
  LCD_REG_ADDR = 0x2A;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0xef;
  LCD_REG_ADDR = 0x11;
  LCD_Delay(120);
  LCD_REG_ADDR = 0x29;

#endif
}

/**
 * @brief  Disables the Display.
 * @param  None
 * @retval LCD Register Value.
 */
uint16_t ili9341_ReadID(void) {
  LCD_IO_Init();
  return LCD_IO_ReadChipID();
}

/**
 * @brief  Enables the Display.
 * @param  None
 * @retval None
 */
void ili9341_DisplayOn(void) {
  /* Display On */
  ili9341_WriteReg(LCD_DISPLAY_ON);
}

/**
 * @brief  Disables the Display.
 * @param  None
 * @retval None
 */
void ili9341_DisplayOff(void) {
  /* Display Off */
  ili9341_WriteReg(LCD_DISPLAY_OFF);
}

/**
 * @brief  Writes  to the selected LCD register.
 * @param  LCD_Reg: address of the selected register.
 * @retval None
 */
void ili9341_WriteReg(uint8_t LCD_Reg) { LCD_IO_WriteReg(LCD_Reg); }

/**
 * @brief  Writes data to the selected LCD register.
 * @param  LCD_Reg: address of the selected register.
 * @retval None
 */
void ili9341_WriteData(uint16_t RegValue) { LCD_IO_WriteData(RegValue); }

/**
 * @brief  Reads the selected LCD Register.
 * @param  RegValue: Address of the register to read
 * @param  ReadSize: Number of bytes to read
 * @retval LCD Register Value.
 */
uint32_t ili9341_ReadData(uint16_t RegValue, uint8_t ReadSize) {
  /* Read a max of 4 bytes */
  return (LCD_IO_ReadData(RegValue, ReadSize));
}

/**
 * @brief  Get LCD PIXEL WIDTH.
 * @param  None
 * @retval LCD PIXEL WIDTH.
 */
uint16_t ili9341_GetLcdPixelWidth(void) {
  /* Return LCD PIXEL WIDTH */
  return ILI9341_LCD_PIXEL_WIDTH;
}

/**
 * @brief  Get LCD PIXEL HEIGHT.
 * @param  None
 * @retval LCD PIXEL HEIGHT.
 */
uint16_t ili9341_GetLcdPixelHeight(void) {
  /* Return LCD PIXEL HEIGHT */
  return ILI9341_LCD_PIXEL_HEIGHT;
}

uint8_t setxcmd = 0X2A;
uint8_t setycmd = 0X2B;
uint8_t dir_flag = 0; // 1963竖屏标志
uint16_t lcd_width = 240;
uint16_t lcd_height = 320;

/****************************************************************************
* 名    称: void LCD_Open_Window(uint16_t X0,uint16_t Y0,uint16_t width,uint16_t
height)
* 功    能：开窗口,并设置画点坐标到窗口左上角(X0,Y0)
* 入口参数：X0,Y0:窗口起始坐标(左上角)
            width,height:窗口宽度和高度
* 返回参数：无
* 说    明：窗体大小:width*height.
****************************************************************************/
void illi9341_Open_Window(uint16_t X0, uint16_t Y0, uint16_t width,
                          uint16_t height) {
  width = X0 + width - 1; // 算出右下角坐标
  height = Y0 + height - 1;
  uint16_t lcd_id = 0X9341;
  if (dir_flag == 0 && lcd_id == 0X9341) // 1963竖屏处理
  {
    LCD_REG_ADDR = 0X2A;
    LCD_RAM_ADDR = X0 >> 8;
    LCD_RAM_ADDR = X0 & 0XFF;
    LCD_RAM_ADDR = width >> 8;
    LCD_RAM_ADDR = width & 0XFF;
    LCD_REG_ADDR = 0X2B;
    LCD_RAM_ADDR = Y0 >> 8;
    LCD_RAM_ADDR = Y0 & 0XFF;
    LCD_RAM_ADDR = height >> 8;
    LCD_RAM_ADDR = height & 0XFF;
  }
}

/****************************************************************************
 * 名    称: void Set_Scan_Direction(u8 direction)    启￥明#欣￥欣
 * 功    能：设置LCD的扫描方向
 * 入口参数：direction：扫描方向
 * 返回参数：无
 * 说    明：
 ****************************************************************************/
void illi9341_Scan_Direction(uint8_t direction) {
  uint16_t skhda = 0;
  uint16_t diomf = 0;
  uint16_t lcd_id = 0X9341;
  // 9341横屏和1963竖屏时需要转化下
  if ((dir_flag == 1 && lcd_id == 0X9341) ||
      (dir_flag == 0 && lcd_id == 0X1963)) {
    switch (direction) // 方向转换
    {
    case 0:
      direction = 6;
      break;
    case 1:
      direction = 7;
      break;
    case 2:
      direction = 4;
      break;
    case 3:
      direction = 5;
      break;
    case 4:
      direction = 1;
      break;
    case 5:
      direction = 0;
      break;
    case 6:
      direction = 3;
      break;
    case 7:
      direction = 2;
      break;
    }
  }

  switch (direction) {
  case 0: // 从左到右,从上到下
    skhda |= (0 << 7) | (0 << 6) | (0 << 5);
    break;
  case 1: // 从左到右,从下到上
    skhda |= (1 << 7) | (0 << 6) | (0 << 5);
    break;
  case 2: // 从右到左,从上到下
    skhda |= (0 << 7) | (1 << 6) | (0 << 5);
    break;
  case 3: // 从右到左,从下到上
    skhda |= (1 << 7) | (1 << 6) | (0 << 5);
    break;
  case 4: // 从上到下,从左到右
    skhda |= (0 << 7) | (0 << 6) | (1 << 5);
    break;
  case 5: // 从上到下,从右到左
    skhda |= (0 << 7) | (1 << 6) | (1 << 5);
    break;
  case 6: // 从下到上,从左到右
    skhda |= (1 << 7) | (0 << 6) | (1 << 5);
    break;
  case 7: // 从下到上,从右到左
    skhda |= (1 << 7) | (1 << 6) | (1 << 5);
    break;
  }
  diomf = 0X36;
  if (lcd_id == 0X9341)
    skhda |= 0X08;
  LCD_IO_WriteData2Cmd(diomf, skhda);
  illi9341_Open_Window(
      0, 0, ili9341_GetLcdPixelWidth(),
      ili9341_GetLcdPixelHeight()); // 设置完扫描方向后，开显示区域为全屏窗口
}

/****************************************************************************
* 名    称: void Set_Display_Mode(u8 mode)
* 功    能：设置LCD显示方向
* 入口参数：mode: 0,竖屏
                  1,横屏
* 返回参数：无
* 说    明：
****************************************************************************/
void illi9341_Display_Mode(uint8_t mode) {
  if (mode == 0) // 竖屏
  {
    illi9341_Scan_Direction(0); // 设置扫描方向   从左到右,从下到上
  }
}

/****************************************************************************
* 名    称: void LCD_SetCursor(uint16_t Xaddr, uint16_t Yaddr)       启#明%欣#欣
* 功    能：设置光标位置
* 入口参数：x：x坐标
            y：y坐标
* 返回参数：无
* 说    明：
****************************************************************************/
void illi9341_SetCursor(uint16_t Xaddr, uint16_t Yaddr) {
  uint16_t lcd_id = 0X9341;
  if (lcd_id == 0X9341) {
    LCD_REG_ADDR = setxcmd;
    LCD_RAM_ADDR = (Xaddr >> 8);
    LCD_RAM_ADDR = (Xaddr & 0XFF);
    LCD_REG_ADDR = setycmd;
    LCD_RAM_ADDR = (Yaddr >> 8);
    LCD_RAM_ADDR = (Yaddr & 0XFF);
  } else if (lcd_id == 0X1963) {
    if (dir_flag == 0) {
      Xaddr = lcd_width - 1 - Xaddr;
      LCD_REG_ADDR = setxcmd;
      LCD_RAM_ADDR = 0;
      LCD_RAM_ADDR = 0;
      LCD_RAM_ADDR = Xaddr >> 8;
      LCD_RAM_ADDR = Xaddr & 0XFF;
    } else {
      LCD_REG_ADDR = setxcmd;
      LCD_RAM_ADDR = Xaddr >> 8;
      LCD_RAM_ADDR = Xaddr & 0XFF;
      LCD_RAM_ADDR = (lcd_width - 1) >> 8;
      LCD_RAM_ADDR = (lcd_width - 1) & 0XFF;
    }
    LCD_REG_ADDR = setycmd;
    LCD_RAM_ADDR = Yaddr >> 8;
    LCD_RAM_ADDR = Yaddr & 0XFF;
    LCD_RAM_ADDR = (lcd_height - 1) >> 8;
    LCD_RAM_ADDR = (lcd_height - 1) & 0XFF;
  }
}
uint16_t lcd_id = 0X9341;
/****************************************************************************
* 名    称: uint16_t LCD_GetPoint(uint16_t x,uint16_t y)
* 功    能：读取某点的颜色值
* 入口参数：x：x坐标
            y：y坐标
* 返回参数：此点的颜色
* 说    明：
****************************************************************************/
uint16_t illi9341_GetPoint(uint16_t x, uint16_t y) {
  uint16_t r = 0, g = 0, b = 0;

  illi9341_SetCursor(x, y);

  LCD_REG_ADDR = 0X2E; // 9341与1963读GRAM指令一样
  r = LCD_RAM_ADDR;

  if (lcd_id == 0X1963)
    return r; // 1963直接读出来就是16位颜色值

  else // 其他驱动就是9341
  {
    LCD_Delay(2);
    b = LCD_RAM_ADDR; // 9341要读2次
    g = r & 0XFF; // 9341第一次读取的是RG的值,R在前,G在后,各占8位
    g <<= 8;
    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11)); // 9341需公式转换
  }
}

/****************************************************************************
* 名    称: void LCD_DrawPoint(uint16_t x,uint16_t y)
* 功    能：画点（在该点写入画笔的颜色）
* 入口参数：x：x坐标
            y：y坐标
* 返回参数：无
* 说    明RUSH_COLOR:此点的颜色值
****************************************************************************/
void illi9341_DrawPoint(uint16_t x, uint16_t y, uint16_t color) {
  illi9341_SetCursor(x, y); // 设置光标位置
  LCD_REG_ADDR = 0X2C;      // 开始写入GRAM
  LCD_RAM_ADDR = color;
}

/****************************************************************************
 * 名    称: void Ssd1963_Set_BackLight(u8 BL_value)
 * 功    能：SSD1963 设置背光
 * 入口参数：BL_value：背光亮度大小  取值:0-255  设置255最亮
 * 返回参数：无
 * 说    明：
 ****************************************************************************/
void illi9341_Set_BackLight(uint8_t BL_value) {
  LCD_REG_ADDR = 0xBE;
  LCD_RAM_ADDR = 0x05;
  LCD_RAM_ADDR = BL_value;
  LCD_RAM_ADDR = 0x01;
  LCD_RAM_ADDR = 0xFF;
  LCD_RAM_ADDR = 0x00;
  LCD_RAM_ADDR = 0x00;
}

/****************************************************************************
 * 名    称: void LCD_Clear(uint16_t color)
 * 功    能：清屏函数
 * 入口参数：color: 要清屏的填充色
 * 返回参数：无
 * 说    明：
 ****************************************************************************/
void illi9341_Clear(uint16_t color) {
  uint32_t i = 0;
  uint32_t pointnum = 0;

  pointnum = lcd_width * lcd_height; // 得到LCD总点数
  illi9341_SetCursor(0x00, 0x00);    // 设置光标位置
  LCD_REG_ADDR = 0X2C;               // 开始写入GRAM
  for (i = 0; i < pointnum; i++) {
    LCD_RAM_ADDR = color;
  }
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
