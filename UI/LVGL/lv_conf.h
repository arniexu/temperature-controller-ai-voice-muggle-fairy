/**
 * @file lv_conf.h
 * @brief LVGL Configuration for Tianji Thermostat on STM32F407
 * @note  Screen: 240x320 TFT LCD with touchscreen
 *        Target: STM32F407VG (1MB Flash, 192KB SRAM)
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*===================
 * COLOR SETTINGS
 *===================*/
#define LV_COLOR_DEPTH          16          /* 16-bit RGB565 */
#define LV_COLOR_16_SWAP        0
#define LV_COLOR_SCREEN_TRANSP  0

/*====================
 * MEMORY SETTINGS
 *====================*/
#define LV_MEM_CUSTOM           0
#define LV_MEM_SIZE             (32U * 1024U)   /* 32KB for LVGL heap */

/*====================
 * HAL SETTINGS
 *====================*/
#define LV_TICK_CUSTOM          1
#define LV_TICK_CUSTOM_INCLUDE  "stm32f4xx_hal.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR    (HAL_GetTick())

/*===================
 * FEATURE CONFIG
 *===================*/
#define LV_USE_PERF_MONITOR     0
#define LV_USE_MEM_MONITOR      0
#define LV_USE_LOG              1
#define LV_LOG_LEVEL            LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF           1

/*===================
 * DRAW SETTINGS
 *===================*/
#define LV_DRAW_COMPLEX         1
#define LV_SHADOW_CACHE_SIZE    0
#define LV_IMG_CACHE_DEF_SIZE   1
#define LV_GRADIENT_MAX_STOPS   2
#define LV_DISP_DEF_REFR_PERIOD 16      /* ~60FPS */
#define LV_INDEV_DEF_READ_PERIOD 20     /* 50Hz touch polling */

/*=======================
 * GPU / OPTIMIZATION
 *=======================*/
/* STM32F4 has Chrom-ART (DMA2D) - enable if available */
#define LV_USE_GPU_STM32_DMA2D  0
#define LV_USE_GPU              0

/*===================
 * FONT SETTINGS
 *===================*/
#define LV_FONT_MONTSERRAT_8    1
#define LV_FONT_MONTSERRAT_10   1
#define LV_FONT_MONTSERRAT_12   1
#define LV_FONT_MONTSERRAT_14   1
#define LV_FONT_MONTSERRAT_16   1
#define LV_FONT_MONTSERRAT_20   1
#define LV_FONT_MONTSERRAT_24   1
#define LV_FONT_MONTSERRAT_28   1
#define LV_FONT_MONTSERRAT_32   1
#define LV_FONT_MONTSERRAT_36   1
#define LV_FONT_MONTSERRAT_40   1
#define LV_FONT_MONTSERRAT_48   1

#define LV_FONT_DEFAULT         &lv_font_montserrat_14

/* Enable compressed font support for CJK characters */
#define LV_USE_FONT_COMPRESSED  1

/*===================
 * WIDGET SELECTION
 *===================*/
#define LV_USE_ARC              1
#define LV_USE_BAR              1
#define LV_USE_BTN              1
#define LV_USE_BTNMATRIX        0
#define LV_USE_CANVAS           1
#define LV_USE_CHECKBOX         0
#define LV_USE_DROPDOWN         0
#define LV_USE_IMG              1
#define LV_USE_LABEL            1
#define LV_USE_LINE             1
#define LV_USE_ROLLER           0
#define LV_USE_SLIDER           1
#define LV_USE_SWITCH           0
#define LV_USE_TEXTAREA         0
#define LV_USE_TABLE            0
#define LV_USE_CHART            0
#define LV_USE_COLORWHEEL       0
#define LV_USE_IMGBTN           1
#define LV_USE_KEYBOARD         0
#define LV_USE_LED              1
#define LV_USE_LIST             0
#define LV_USE_MENU             0
#define LV_USE_METER            1
#define LV_USE_MSGBOX           0
#define LV_USE_SPINBOX          0
#define LV_USE_SPINNER          1
#define LV_USE_CALENDAR         0
#define LV_USE_CALENDAR_HEADER_ARROW    0
#define LV_USE_CALENDAR_HEADER_DROPDOWN 0
#define LV_USE_TABVIEW          0
#define LV_USE_TILEVIEW         0
#define LV_USE_WIN              0
#define LV_USE_SPAN             0

/*=======================
 * ANIMATION / THEME
 *=======================*/
#define LV_USE_ANIMATION      1
#define LV_USE_THEME_DEFAULT  1
#define LV_THEME_DEFAULT_INIT 1

/*==================
 * LAYOUTS
 *==================*/
#define LV_USE_FLEX     1
#define LV_USE_GRID     0

/*==================
 * 3rd PARTIES
 *==================*/
#define LV_USE_SDL              0
#define LV_USE_LINUX_FBDEV      0
#define LV_USE_LINUX_DRM        0
#define LV_USE_NUTTX            0

/*==================
 * EXAMPLES
 *==================*/
#define LV_BUILD_EXAMPLES       0
#define LV_USE_DEMO_BENCHMARK   0
#define LV_USE_DEMO_KEYPAD_ENCODER 0
#define LV_USE_DEMO_MUSIC     0
#define LV_USE_DEMO_STRESS    0
#define LV_USE_DEMO_WIDGETS   0

#endif /* LV_CONF_H */
