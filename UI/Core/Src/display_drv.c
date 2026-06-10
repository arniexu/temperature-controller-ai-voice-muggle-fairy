#include "display_drv.h"

#include "bsp_stubs.h"

static lv_disp_draw_buf_t s_draw_buf;
static lv_color_t s_buf1[LVGL_DRAW_BUF_SIZE];
static lv_disp_drv_t s_disp_drv;
static lv_disp_t *s_disp = NULL;

static void display_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (uint32_t)(area->x2 - area->x1 + 1);
    uint32_t h = (uint32_t)(area->y2 - area->y1 + 1);
    uint32_t pixels = w * h;

    (void)drv;

    lcd_set_window((uint16_t)area->x1, (uint16_t)area->y1, (uint16_t)area->x2, (uint16_t)area->y2);
    lcd_write_pixels((const uint16_t *)color_p, pixels);

    lv_disp_flush_ready(drv);
}

int8_t display_drv_init(void)
{
    lcd_init();

    lv_disp_draw_buf_init(&s_draw_buf, s_buf1, NULL, LVGL_DRAW_BUF_SIZE);

    lv_disp_drv_init(&s_disp_drv);
    s_disp_drv.hor_res = LCD_WIDTH;
    s_disp_drv.ver_res = LCD_HEIGHT;
    s_disp_drv.flush_cb = display_flush_cb;
    s_disp_drv.draw_buf = &s_draw_buf;

    s_disp = lv_disp_drv_register(&s_disp_drv);
    return (s_disp != NULL) ? 0 : -1;
}

lv_disp_t *display_drv_get_lv_disp(void)
{
    return s_disp;
}

void display_drv_set_backlight(uint8_t percent)
{
    lcd_set_backlight_hw(percent);
}

void display_drv_sleep(void)
{
    lcd_enter_sleep();
}

void display_drv_wakeup(void)
{
    lcd_exit_sleep();
}
