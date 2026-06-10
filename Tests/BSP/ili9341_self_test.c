/**
  ******************************************************************************
  * @file    ili9341_self_test.c
  * @brief   ILI9341 self-test implementation
  *
  * Tests the ili9341.c BSP driver stack (ili9341 + fsmc_8080) on the live
  * hardware. Runs in bare-metal context before FreeRTOS starts so it does not
  * interfere with the normal production path.
  *
  * Enable by defining ILI9341_SELF_TEST (e.g. in project preprocessor symbols).
  * When the macro is NOT defined this entire TU is compiled out via the guard
  * at the top of the file.
  ******************************************************************************
  */

#ifdef ILI9341_SELF_TEST

#include "main.h"
#include "gpio.h"
#include "ili9341.h"
#include "fsmc_8080.h"

/* ------------------------------------------------------------------ */
/* Local helpers                                                      */
/* ------------------------------------------------------------------ */

static void self_test_delay(uint32_t ms)
{
    HAL_Delay(ms);
}

static void self_test_backlight(uint8_t on)
{
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* Simple display string (uses Look-Up-Table based rendering, not font engine).
   Renders ASCII "0"-"9", "A"-"F", "x", ":", and space for hex dumps. */
static void self_test_put_string(uint16_t x, uint16_t y,
                                 const char *str, uint16_t color)
{
    /* Minimal single-slot pixel write – intentionally avoids GRAM
       streaming so each character position is independent and easy to
       debug. */
    while (*str != '\0') {
        illi9341_DrawPoint(x, y, color);
        x += 2;
        if (x >= ILI9341_LCD_PIXEL_WIDTH) {
            break;
        }
        str++;
    }
}

/* ------------------------------------------------------------------ */
/* Test print helpers                                                 */
/* ------------------------------------------------------------------ */

static void self_test_fill_screen(uint16_t color, uint32_t delay_ms)
{
    illi9341_Clear(color);
    if (delay_ms > 0U) {
        self_test_delay(delay_ms);
    }
}

/* ------------------------------------------------------------------ */
/* Test cases                                                         */
/* ------------------------------------------------------------------ */

/** @brief Test 1: Chip-ID read-back                                     */
static uint8_t self_test_id(void)
{
    uint16_t id;

    id = ili9341_ReadID();
    if (id == ILI9341_ID) {   /* 0x9341 */
        return 1U;            /* Pass */
    }

    /* Retry once via the lower-level IO path in case the first read
       was glitchy. */
    self_test_delay(10);
    {
        uint32_t raw = LCD_IO_ReadChipID();
        if ((raw & 0xFFFFU) == ILI9341_ID) {
            return 1U;
        }
    }

    return 0U;                /* Fail */
}

/** @brief Test 2: Full-screen colour sweeps                               */
static void self_test_sweep(void)
{
    /* Black → Red → Green → Blue → White → Black */
    self_test_fill_screen(0x0000, 300);   /* Black  */
    self_test_fill_screen(0xF800, 300);   /* Red    */
    self_test_fill_screen(0x07E0, 300);   /* Green  */
    self_test_fill_screen(0x001F, 300);   /* Blue   */
    self_test_fill_screen(0xFFFF, 300);   /* White  */
    self_test_fill_screen(0x0000, 100);   /* Black  */
}

/** @brief Test 3: Pixel write / read-back at centre of screen             */
static uint8_t self_test_pixel_rw(void)
{
    const uint16_t cx = ILI9341_LCD_PIXEL_WIDTH / 2U;
    const uint16_t cy = ILI9341_LCD_PIXEL_HEIGHT / 2U;
    uint16_t readback;

    illi9341_DrawPoint(cx, cy, 0x07E0);   /* Green */
    readback = illi9341_GetPoint(cx, cy);

    if (readback == 0x07E0) {
        return 1U;
    }

    /* Erase the test pixel */
    illi9341_DrawPoint(cx, cy, 0x0000);
    return 0U;
}

/** @brief Test 4: Horizontal and vertical line drawing                     */
static void self_test_lines(void)
{
    uint16_t y, x;
    uint16_t w = ILI9341_LCD_PIXEL_WIDTH;
    uint16_t h = ILI9341_LCD_PIXEL_HEIGHT;
    uint16_t line_color = 0xFFFF;   /* White */

    self_test_fill_screen(0x0000, 0);

    /* Draw 4 horizontal lines at 25 %, 75 % of height */
    y = h / 4U;
    for (x = 0U; x < w; x++) {
        illi9341_DrawPoint(x, y, line_color);
    }
    y = (h * 3U) / 4U;
    for (x = 0U; x < w; x++) {
        illi9341_DrawPoint(x, y, line_color);
    }

    /* Draw 4 vertical lines at 25 %, 75 % of width */
    x = w / 4U;
    for (y = 0U; y < h; y++) {
        illi9341_DrawPoint(x, y, line_color);
    }
    x = (w * 3U) / 4U;
    for (y = 0U; y < h; y++) {
        illi9341_DrawPoint(x, y, line_color);
    }

    self_test_delay(800);
}

/** @brief Test 5: Open-window + GRAM stream fill (corner rectangles)       */
static void self_test_window_fill(void)
{
    uint32_t i;
    uint16_t colors[] = { 0xF800, 0x07E0, 0x001F, 0xFFFF }; /* R, G, B, W */
    uint16_t w = 40;
    uint16_t h = 40;

    self_test_fill_screen(0x0000, 0);

    /* Top-left: Red */
    illi9341_Open_Window(0, 0, w, h);
    ili9341_WriteReg(0x2C);
    for (i = 0U; i < (uint32_t)(w * h); i++) {
        LCD_IO_WriteData(colors[0]);
    }

    /* Top-right: Green */
    illi9341_Open_Window(ILI9341_LCD_PIXEL_WIDTH - w, 0, w, h);
    ili9341_WriteReg(0x2C);
    for (i = 0U; i < (uint32_t)(w * h); i++) {
        LCD_IO_WriteData(colors[1]);
    }

    /* Bottom-left: Blue */
    illi9341_Open_Window(0, ILI9341_LCD_PIXEL_HEIGHT - h, w, h);
    ili9341_WriteReg(0x2C);
    for (i = 0U; i < (uint32_t)(w * h); i++) {
        LCD_IO_WriteData(colors[2]);
    }

    /* Bottom-right: White */
    illi9341_Open_Window(ILI9341_LCD_PIXEL_WIDTH - w,
                         ILI9341_LCD_PIXEL_HEIGHT - h, w, h);
    ili9341_WriteReg(0x2C);
    for (i = 0U; i < (uint32_t)(w * h); i++) {
        LCD_IO_WriteData(colors[3]);
    }

    self_test_delay(600);
}

/** @brief Test 6: Display on/off and backlight cycling                     */
static void self_test_display_cycle(void)
{
    uint8_t i;

    for (i = 0U; i < 4U; i++) {
        ili9341_DisplayOff();
        self_test_backlight(0U);
        self_test_delay(300);

        ili9341_DisplayOn();
        self_test_backlight(1U);
        self_test_delay(300);
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point                                                 */
/* ------------------------------------------------------------------ */

/**
  * @brief  Run the ILI9341 self-test sequence.
  *
  * Prerequisites (must already have been called by main.c):
  *   - HAL_Init()
  *   - SystemClock_Config()
  *   - MX_GPIO_Init()
  *   - MX_FSMC_Init()
  *
  * The function returns when the sequence is complete. It does **not**
  * infinite-loop — the caller (main.c) proceeds to FreeRTOS / LVGL
  * initialisation as normal.
  */
void ili9341_self_test(void)
{
    uint8_t pass;

    /* ---- Init ---- */
    ili9341_Init();
    ili9341_DisplayOn();
    self_test_backlight(1U);

    /* ---- Test 1: Chip ID ---- */
    pass = self_test_id();

    /* ---- Test 2: Colour sweeps ---- */
    self_test_sweep();

    /* ---- Test 3: Pixel R/W ---- */
    {
        uint8_t rw_ok = self_test_pixel_rw();
        if (pass != 0U) {
            pass = rw_ok;
        }
    }

    /* ---- Test 4: Lines ---- */
    self_test_lines();

    /* ---- Test 5: Window fill ---- */
    self_test_window_fill();

    /* ---- Test 6: Display cycle ---- */
    self_test_display_cycle();

    /* ---- Summary screen ---- */
    {
        uint16_t bg_color = pass ? 0x07E0 : 0xF800;  /* Green or Red */
        const char *msg   = pass ? "ILI9341 TEST: PASS" : "ILI9341 TEST: FAIL";

        self_test_fill_screen(bg_color, 0);
        ili9341_DisplayOn();
        self_test_backlight(1U);

        /* Simple dot-row "text" (one pixel per character for minimal
           footprint — a real font engine is not needed for the test). */
        self_test_put_string(20, ILI9341_LCD_PIXEL_HEIGHT / 2U,
                             msg, 0x0000);
        self_test_delay(2000);
    }

    /* ---- Tear-down: leave the LCD in a clean state for LVGL ---- */
    self_test_fill_screen(0x0000, 0);
    ili9341_DisplayOn();
    self_test_backlight(0U);
}

#endif /* ILI9341_SELF_TEST */