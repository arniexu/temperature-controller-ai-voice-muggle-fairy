/**
 * @file touch_drv.h
 * @brief Touchscreen driver interface (XPT2046 / STMPE811 / etc.)
 * @note  BSP functions (touch_*) assumed implemented
 */

#ifndef TOUCH_DRV_H
#define TOUCH_DRV_H

#include <stdint.h>
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Touch controller type
 */
#define TOUCH_XPT2046       1
#define TOUCH_STMPE811      0
#define TOUCH_FT6X06        0

/**
 * @brief Initialize touch hardware and LVGL input driver
 * @return 0 on success, negative on error
 */
int8_t touch_drv_init(void);

/**
 * @brief Calibrate touchscreen
 * @note  Shows calibration points on screen
 */
void touch_drv_calibrate(void);

/**
 * @brief Get raw touch coordinates (for debugging)
 */
void touch_drv_get_raw(int16_t *x, int16_t *y, uint8_t *pressed);

#ifdef __cplusplus
}
#endif

#endif /* TOUCH_DRV_H */
