/**
 * @file touch_drv.c
 * @brief Touch driver implementation for STM32F407
 * @note  Integrates with LVGL v8.3 input device callback
 */

#include "touch_drv.h"
#include "display_drv.h"
#include "bsp_stubs.h"
#include <string.h>

/* Calibration matrix (default identity, updated after calibrate) */
static struct {
    float a, b, c;
    float d, e, f;
} cal_matrix = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };

static lv_indev_drv_t indev_drv;
static lv_indev_t *indev = NULL;

/*=================================================================*/
/* LVGL Touch Read Callback                                        */
/*=================================================================*/
static void touchpad_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    (void)drv;

    if (touch_is_pressed()) {
        uint16_t raw_x, raw_y;
        touch_read_coords(&raw_x, &raw_y);

        /* Apply calibration */
        int16_t x = (int16_t)(cal_matrix.a * raw_x + cal_matrix.b * raw_y + cal_matrix.c);
        int16_t y = (int16_t)(cal_matrix.d * raw_x + cal_matrix.e * raw_y + cal_matrix.f);

        /* Clamp to screen bounds */
        if (x < 0) x = 0;
        if (x >= LCD_WIDTH) x = LCD_WIDTH - 1;
        if (y < 0) y = 0;
        if (y >= LCD_HEIGHT) y = LCD_HEIGHT - 1;

        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

/*=================================================================*/
/* Public Functions                                                */
/*=================================================================*/

int8_t touch_drv_init(void)
{
    touch_init_hw();

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read_cb;
    indev_drv.long_press_time = 400;
    indev_drv.scroll_limit = 10;

    indev = lv_indev_drv_register(&indev_drv);

    return (indev != NULL) ? 0 : -1;
}

void touch_drv_calibrate(void)
{
    /* Simple 3-point calibration */
    /* BSP should implement this with visual feedback */
    /* For now, use identity matrix assuming calibrated display */
    cal_matrix.a = (float)LCD_WIDTH / 4096.0f;
    cal_matrix.e = (float)LCD_HEIGHT / 4096.0f;
    cal_matrix.c = 0.0f;
    cal_matrix.f = 0.0f;
    cal_matrix.b = 0.0f;
    cal_matrix.d = 0.0f;
}

void touch_drv_get_raw(int16_t *x, int16_t *y, uint8_t *pressed)
{
    *pressed = touch_is_pressed();
    if (*pressed) {
        uint16_t rx, ry;
        touch_read_coords(&rx, &ry);
        *x = (int16_t)rx;
        *y = (int16_t)ry;
    }
}
