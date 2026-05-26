#ifndef BSP_STUBS_H
#define BSP_STUBS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void lcd_init(void);
void lcd_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void lcd_write_pixels(const uint16_t *data, uint32_t count);
void lcd_write_pixel(uint16_t color);
void lcd_set_backlight_hw(uint8_t percent);
void lcd_enter_sleep(void);
void lcd_exit_sleep(void);

void touch_init_hw(void);
uint8_t touch_is_pressed(void);
void touch_read_coords(uint16_t *x, uint16_t *y);

#ifdef __cplusplus
}
#endif

#endif /* BSP_STUBS_H */
