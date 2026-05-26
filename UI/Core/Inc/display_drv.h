/**
 * @file display_drv.h
 * @brief Display driver interface for STM32F407 + 240x320 TFT LCD
 * @note  Adapt this to your specific LCD controller (ILI9341/ST7789/etc.)
 *        BSP functions are assumed to be pre-implemented.
 */

#ifndef DISPLAY_DRV_H
#define DISPLAY_DRV_H

#include <stdint.h>
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief LCD controller type - select your hardware
 */
#define LCD_DRV_ILI9341     1
#define LCD_DRV_ST7789      0
#define LCD_DRV_OTHER       0

/**
 * @brief Display dimensions
 */
#define LCD_WIDTH           240
#define LCD_HEIGHT          320

/**
 * @brief LVGL draw buffer size in pixels.
 *        Use a line-buffer strategy to keep RAM usage low on STM32F407.
 */
#define LVGL_DRAW_BUF_LINES 20
#define LVGL_DRAW_BUF_SIZE  (LCD_WIDTH * LVGL_DRAW_BUF_LINES)

/**
 * @brief Initialize display hardware and LVGL display driver
 * @return 0 on success, negative on error
 */
int8_t display_drv_init(void);

/**
 * @brief Get the LVGL display object
 * @return Pointer to lv_disp_t
 */
lv_disp_t* display_drv_get_lv_disp(void);

/**
 * @brief Backlight control
 * @param percent 0-100 brightness level
 */
void display_drv_set_backlight(uint8_t percent);

/**
 * @brief Enter low power / sleep mode
 */
void display_drv_sleep(void);

/**
 * @brief Wake from sleep mode
 */
void display_drv_wakeup(void);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_DRV_H */
