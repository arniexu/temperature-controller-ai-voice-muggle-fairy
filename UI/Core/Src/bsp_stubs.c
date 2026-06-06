#include "bsp_stubs.h"

#include "main.h"
#include "gpio.h"
#include "ili9341.h"
#include "fsmc_8080.h"
#include "xpt2046.h"

void lcd_init(void)
{
    ili9341_Init();
	uint32_t chip_id=LCD_IO_ReadChipID();
    ili9341_DisplayOn();
}

void lcd_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_IO_WriteReg(LCD_COLUMN_ADDR);
    LCD_IO_WriteData((uint16_t)(x1 >> 8));
    LCD_IO_WriteData((uint16_t)(x1 & 0xFFU));
    LCD_IO_WriteData((uint16_t)(x2 >> 8));
    LCD_IO_WriteData((uint16_t)(x2 & 0xFFU));

    LCD_IO_WriteReg(LCD_PAGE_ADDR);
    LCD_IO_WriteData((uint16_t)(y1 >> 8));
    LCD_IO_WriteData((uint16_t)(y1 & 0xFFU));
    LCD_IO_WriteData((uint16_t)(y2 >> 8));
    LCD_IO_WriteData((uint16_t)(y2 & 0xFFU));

    LCD_IO_WriteReg(LCD_GRAM);
}

void lcd_write_pixels(const uint16_t *data, uint32_t count)
{
    while (count--) {
        LCD_IO_WriteData(*data++);
    }
}

void lcd_write_pixel(uint16_t color)
{
    LCD_IO_WriteData(color);
}

void lcd_set_backlight_hw(uint8_t percent)
{
    GPIO_PinState state = (percent == 0U) ? GPIO_PIN_RESET : GPIO_PIN_SET;
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, state);
}

void lcd_enter_sleep(void)
{
    ili9341_DisplayOff();
}

void lcd_exit_sleep(void)
{
    ili9341_DisplayOn();
}

void touch_init_hw(void)
{
    xpt2046_init();
}

uint8_t touch_is_pressed(void)
{
    return xpt2046_pressed();
}

void touch_read_coords(uint16_t *x, uint16_t *y)
{
    xpt2046_update();
    xpt2046_read_position(x, y);
}
