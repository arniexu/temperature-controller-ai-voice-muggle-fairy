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
  * Copyright (c) 2014 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "ili9341.h"

static void ili9341_SetCursor(uint16_t Xpos, uint16_t Ypos);
static void ili9341_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code);
static uint16_t ili9341_ReadPixel(uint16_t Xpos, uint16_t Ypos);
static void ili9341_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
static void ili9341_DrawHLine(uint16_t RGB_Code, uint16_t Xpos, uint16_t Ypos, uint16_t Length);
static void ili9341_DrawVLine(uint16_t RGB_Code, uint16_t Xpos, uint16_t Ypos, uint16_t Length);

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

LCD_DrvTypeDef   ili9341_drv = 
{
  ili9341_Init,
  ili9341_ReadID,
  ili9341_DisplayOn,
  ili9341_DisplayOff,
  ili9341_SetCursor,
  ili9341_WritePixel,
  ili9341_ReadPixel,
  ili9341_SetDisplayWindow,
  ili9341_DrawHLine,
  ili9341_DrawVLine,
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
void ili9341_Init(void)
{
  /* Initialize ILI9341 low level bus layer ----------------------------------*/

  LCD_IO_Init();
  
  /* Configure LCD */
  ili9341_WriteReg(0xCA);
  ili9341_WriteData(0xC3);
  ili9341_WriteData(0x08);
  ili9341_WriteData(0x50);
  ili9341_WriteReg(LCD_POWERB);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0xC1);
  ili9341_WriteData(0x30);
  ili9341_WriteReg(LCD_POWER_SEQ);
  ili9341_WriteData(0x64);
  ili9341_WriteData(0x03);
  ili9341_WriteData(0x12);
  ili9341_WriteData(0x81);
  ili9341_WriteReg(LCD_DTCA);
  ili9341_WriteData(0x85);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x78);
  ili9341_WriteReg(LCD_POWERA);
  ili9341_WriteData(0x39);
  ili9341_WriteData(0x2C);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x34);
  ili9341_WriteData(0x02);
  ili9341_WriteRegData(LCD_PRC, 0x20);
  ili9341_WriteReg(LCD_DTCB);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x00);
  ili9341_WriteReg(LCD_FRMCTR1);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x1B);
  ili9341_WriteReg(LCD_DFC);
  ili9341_WriteData(0x0A);
  ili9341_WriteData(0xA2);
  ili9341_WriteRegData(LCD_POWER1, 0x10);
  ili9341_WriteRegData(LCD_POWER2, 0x10);
  ili9341_WriteReg(LCD_VCOM1);
  ili9341_WriteData(0x45);
  ili9341_WriteData(0x15);
  ili9341_WriteRegData(LCD_VCOM2, 0x90);
  ili9341_WriteRegData(LCD_MAC, 0xC8);
  ili9341_WriteRegData(LCD_3GAMMA_EN, 0x00);
  ili9341_WriteRegData(LCD_RGB_INTERFACE, 0xC2);
  ili9341_WriteReg(LCD_DFC);
  ili9341_WriteData(0x0A);
  ili9341_WriteData(0xA7);
  ili9341_WriteData(0x27);
  ili9341_WriteData(0x04);
  
  /* Colomn address set */
  ili9341_WriteReg(LCD_COLUMN_ADDR);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0xEF);
  /* Page address set */
  ili9341_WriteReg(LCD_PAGE_ADDR);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x01);
  ili9341_WriteData(0x3F);
  ili9341_WriteReg(LCD_INTERFACE);
  ili9341_WriteData(0x01);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x06);
  
  ili9341_WriteReg(LCD_GRAM);
  LCD_Delay(200);
  
  ili9341_WriteRegData(LCD_GAMMA, 0x01);
  
  ili9341_WriteReg(LCD_PGAMMA);
  ili9341_WriteData(0x0F);
  ili9341_WriteData(0x29);
  ili9341_WriteData(0x24);
  ili9341_WriteData(0x0C);
  ili9341_WriteData(0x0E);
  ili9341_WriteData(0x09);
  ili9341_WriteData(0x4E);
  ili9341_WriteData(0x78);
  ili9341_WriteData(0x3C);
  ili9341_WriteData(0x09);
  ili9341_WriteData(0x13);
  ili9341_WriteData(0x05);
  ili9341_WriteData(0x17);
  ili9341_WriteData(0x11);
  ili9341_WriteData(0x00);
  ili9341_WriteReg(LCD_NGAMMA);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x16);
  ili9341_WriteData(0x1B);
  ili9341_WriteData(0x04);
  ili9341_WriteData(0x11);
  ili9341_WriteData(0x07);
  ili9341_WriteData(0x31);
  ili9341_WriteData(0x33);
  ili9341_WriteData(0x42);
  ili9341_WriteData(0x05);
  ili9341_WriteData(0x0C);
  ili9341_WriteData(0x0A);
  ili9341_WriteData(0x28);
  ili9341_WriteData(0x2F);
  ili9341_WriteData(0x0F);
  
  ili9341_WriteReg(LCD_SLEEP_OUT);
  LCD_Delay(200);
  ili9341_WriteReg(LCD_DISPLAY_ON);
  /* GRAM start writing */
  ili9341_WriteReg(LCD_GRAM);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval LCD Register Value.
  */
uint16_t ili9341_ReadID(void)
{
  LCD_IO_Init();
  return ((uint16_t)ili9341_ReadData(LCD_READ_ID4, LCD_READ_ID4_SIZE));
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void ili9341_DisplayOn(void)
{
  /* Display On */
  ili9341_WriteReg(LCD_DISPLAY_ON);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void ili9341_DisplayOff(void)
{
  /*toggle the gpio pin */
  //
  /* Display Off */
  ili9341_WriteReg(LCD_DISPLAY_OFF);
}

/**
  * @brief  Writes  to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @retval None
  */
void ili9341_WriteReg(uint16_t LCD_Reg)
{
  LCD_IO_WriteCmd(LCD_Reg);
}

/**
  * @brief  Writes register index and data in one transaction.
  * @param  LCD_Reg: Address of the selected register.
  * @param  RegValue: Data to be written.
  * @retval None
  */
void ili9341_WriteRegData(uint16_t LCD_Reg, uint16_t RegValue)
{
  LCD_IO_WriteReg(LCD_Reg, RegValue);
}

/**
  * @brief  Writes data to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @retval None
  */
void ili9341_WriteData(uint16_t RegValue)
{
  LCD_IO_WriteData16(RegValue);
}

/**
  * @brief  Reads the selected LCD Register.
  * @param  RegValue: Address of the register to read
  * @param  ReadSize: Number of bytes to read
  * @retval LCD Register Value.
  */
uint32_t ili9341_ReadData(uint16_t RegValue, uint8_t ReadSize)
{
  /* Read a max of 4 bytes */
  return (LCD_IO_ReadDataN(RegValue, ReadSize));
}

/**
  * @brief  Get LCD PIXEL WIDTH.
  * @param  None
  * @retval LCD PIXEL WIDTH.
  */
uint16_t ili9341_GetLcdPixelWidth(void)
{
  /* Return LCD PIXEL WIDTH */
  return ILI9341_LCD_PIXEL_WIDTH;
}

/**
  * @brief  Get LCD PIXEL HEIGHT.
  * @param  None
  * @retval LCD PIXEL HEIGHT.
  */
uint16_t ili9341_GetLcdPixelHeight(void)
{
  /* Return LCD PIXEL HEIGHT */
  return ILI9341_LCD_PIXEL_HEIGHT;
}

/**
  * @brief  Sets the LCD cursor to the specified coordinates.
  * @param  Xpos: X position.
  * @param  Ypos: Y position.
  * @retval None.
  */
static void ili9341_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  ili9341_WriteReg(LCD_COLUMN_ADDR);
  ili9341_WriteData((uint16_t)(Xpos >> 8));
  ili9341_WriteData((uint16_t)(Xpos & 0x00FFU));
  ili9341_WriteData((uint16_t)(Xpos >> 8));
  ili9341_WriteData((uint16_t)(Xpos & 0x00FFU));

  ili9341_WriteReg(LCD_PAGE_ADDR);
  ili9341_WriteData((uint16_t)(Ypos >> 8));
  ili9341_WriteData((uint16_t)(Ypos & 0x00FFU));
  ili9341_WriteData((uint16_t)(Ypos >> 8));
  ili9341_WriteData((uint16_t)(Ypos & 0x00FFU));
}

/**
  * @brief  Writes a pixel at the specified coordinates.
  * @param  Xpos: X position.
  * @param  Ypos: Y position.
  * @param  RGB_Code: Pixel color in RGB565.
  * @retval None.
  */
static void ili9341_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code)
{
  ili9341_SetCursor(Xpos, Ypos);
  ili9341_WriteReg(LCD_GRAM);
  ili9341_WriteData(RGB_Code);
}

/**
  * @brief  Reads a pixel at the specified coordinates.
  * @param  Xpos: X position.
  * @param  Ypos: Y position.
  * @retval Pixel color in RGB565.
  */
static uint16_t ili9341_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  ili9341_SetCursor(Xpos, Ypos);
  return (uint16_t)ili9341_ReadData(LCD_RAMRD, 2);
}

/**
  * @brief  Sets the display window.
  * @param  Xpos: Left coordinate.
  * @param  Ypos: Top coordinate.
  * @param  Width: Window width.
  * @param  Height: Window height.
  * @retval None.
  */
static void ili9341_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  uint16_t x_end;
  uint16_t y_end;

  if ((Width == 0U) || (Height == 0U))
  {
    return;
  }

  x_end = (uint16_t)(Xpos + Width - 1U);
  y_end = (uint16_t)(Ypos + Height - 1U);

  ili9341_WriteReg(LCD_COLUMN_ADDR);
  ili9341_WriteData((uint16_t)(Xpos >> 8));
  ili9341_WriteData((uint16_t)(Xpos & 0x00FFU));
  ili9341_WriteData((uint16_t)(x_end >> 8));
  ili9341_WriteData((uint16_t)(x_end & 0x00FFU));

  ili9341_WriteReg(LCD_PAGE_ADDR);
  ili9341_WriteData((uint16_t)(Ypos >> 8));
  ili9341_WriteData((uint16_t)(Ypos & 0x00FFU));
  ili9341_WriteData((uint16_t)(y_end >> 8));
  ili9341_WriteData((uint16_t)(y_end & 0x00FFU));
}

/**
  * @brief  Draws a horizontal line.
  * @param  RGB_Code: Pixel color in RGB565.
  * @param  Xpos: Left coordinate.
  * @param  Ypos: Y coordinate.
  * @param  Length: Line length.
  * @retval None.
  */
static void ili9341_DrawHLine(uint16_t RGB_Code, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t index;

  if (Length == 0U)
  {
    return;
  }

  ili9341_SetDisplayWindow(Xpos, Ypos, Length, 1U);
  ili9341_WriteReg(LCD_GRAM);

  for (index = 0U; index < Length; index++)
  {
    ili9341_WriteData(RGB_Code);
  }
}

/**
  * @brief  Draws a vertical line.
  * @param  RGB_Code: Pixel color in RGB565.
  * @param  Xpos: X coordinate.
  * @param  Ypos: Top coordinate.
  * @param  Length: Line length.
  * @retval None.
  */
static void ili9341_DrawVLine(uint16_t RGB_Code, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t index;

  if (Length == 0U)
  {
    return;
  }

  ili9341_SetDisplayWindow(Xpos, Ypos, 1U, Length);
  ili9341_WriteReg(LCD_GRAM);

  for (index = 0U; index < Length; index++)
  {
    ili9341_WriteData(RGB_Code);
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
