/**
  ******************************************************************************
  * @file    lcd_log_conf.h
  * @brief   LCD log configuration for current board support package.
  ******************************************************************************
  */

#ifndef __LCD_LOG_CONF_H__
#define __LCD_LOG_CONF_H__

#include "../Fonts/fonts.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>

extern uint16_t lcd_width;
extern uint16_t lcd_height;
extern uint16_t BRUSH_COLOR;
extern uint16_t BACK_COLOR;

void LCD_Clear(uint16_t color);
void LCD_Fill_onecolor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
void LCD_DisplayString(uint16_t x, uint16_t y, uint8_t size, uint8_t *text);

/* Enable log scroll feature */
#define LCD_SCROLL_ENABLED 1

/* Fonts */
#define LCD_LOG_HEADER_FONT Font16
#define LCD_LOG_FOOTER_FONT Font12
#define LCD_LOG_TEXT_FONT   Font12

/* Colors */
#define LCD_LOG_BACKGROUND_COLOR       LCD_COLOR_WHITE
#define LCD_LOG_TEXT_COLOR             LCD_COLOR_DARKBLUE
#define LCD_LOG_SOLID_BACKGROUND_COLOR LCD_COLOR_BLUE
#define LCD_LOG_SOLID_TEXT_COLOR       LCD_COLOR_WHITE
#define LCD_LOG_DEFAULT_COLOR          LCD_LOG_TEXT_COLOR

/* Map generic LCD log colors to this board driver colors */
#define LCD_COLOR_WHITE    WHITE
#define LCD_COLOR_BLACK    BLACK
#define LCD_COLOR_RED      RED
#define LCD_COLOR_BLUE     BLUE
#define LCD_COLOR_DARKBLUE BLUE
#define LCD_COLOR_CYAN     CYAN

/* Cache depth and visible lines */
#define CACHE_SIZE   100
#define YWINDOW_SIZE 17

#if (YWINDOW_SIZE > 17)
#error "Wrong YWINDOW_SIZE"
#endif

#ifdef __GNUC__
#define LCD_LOG_PUTCHAR int __io_putchar(int ch)
#else
#define LCD_LOG_PUTCHAR int fputc(int ch, FILE *f)
#endif

/* Minimal BSP LCD compatibility layer used by Utilities/Log/lcd_log.c */
typedef enum
{
  CENTER_MODE = 0x01,
  RIGHT_MODE  = 0x02,
  LEFT_MODE   = 0x03
} Text_AlignModeTypdef;

static sFONT *g_lcd_log_font = &Font12;

static inline uint8_t lcd_log_font_size(void)
{
  if (g_lcd_log_font == &Font24) {
    return 24;
  }
  if (g_lcd_log_font == &Font16) {
    return 16;
  }
  return 12;
}

static inline void BSP_LCD_SetFont(sFONT *fonts)
{
  if (fonts != NULL) {
    g_lcd_log_font = fonts;
  }
}

static inline sFONT *BSP_LCD_GetFont(void)
{
  return g_lcd_log_font;
}

static inline void BSP_LCD_SetTextColor(uint32_t color)
{
  BRUSH_COLOR = (uint16_t)color;
}

static inline void BSP_LCD_SetBackColor(uint32_t color)
{
  BACK_COLOR = (uint16_t)color;
}

static inline void BSP_LCD_Clear(uint16_t color)
{
  LCD_Clear(color);
}

static inline uint16_t BSP_LCD_GetXSize(void)
{
  return lcd_width;
}

static inline uint16_t BSP_LCD_GetYSize(void)
{
  return lcd_height;
}

static inline void BSP_LCD_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  if ((width == 0U) || (height == 0U)) {
    return;
  }
  LCD_Fill_onecolor(x, y, x + width - 1U, y + height - 1U, BRUSH_COLOR);
}

static inline void BSP_LCD_DisplayStringAt(uint16_t x, uint16_t y, uint8_t *text, Text_AlignModeTypdef mode)
{
  uint8_t size = lcd_log_font_size();

  if (mode == CENTER_MODE) {
    uint16_t text_len = (uint16_t)strlen((const char *)text);
    uint16_t text_width = (uint16_t)(text_len * (size / 2U));
    if (text_width < lcd_width) {
      x = (uint16_t)((lcd_width - text_width) / 2U);
    }
  }

  LCD_DisplayString(x, y, size, text);
}

static inline void BSP_LCD_ClearStringLine(uint16_t line)
{
  uint16_t font_h = (uint16_t)BSP_LCD_GetFont()->Height;
  uint16_t y = (uint16_t)(line * font_h);
  if (y >= lcd_height) {
    return;
  }
  LCD_Fill_onecolor(0, y, lcd_width - 1U, (uint16_t)(y + font_h - 1U), BACK_COLOR);
}

#endif /* __LCD_LOG_CONF_H__ */
