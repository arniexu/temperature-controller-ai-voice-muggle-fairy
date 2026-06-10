/**
  ******************************************************************************
  * @file    lvgl_mini_test.c
  * @brief   Minimal LVGL smoke-test implementation
  *
  * Runs a brief LVGL rendering test in bare-metal context before FreeRTOS
  * starts. Uses the production display_drv.c path so the same flush callback
  * and ILI9341 driver are exercised.
  *
  * Enable by defining LVGL_MINI_TEST (e.g. in project preprocessor symbols).
  * When the macro is NOT defined this entire TU is compiled out via the guard
  * at the top of the file.
  *
  * g_boot_stage values: 0x30-0x3F reserved for this module.
  * (freertos.c uses 0x20-0x26, main.c uses 0x10-0x11.)
  ******************************************************************************
  */

#ifdef LVGL_MINI_TEST

#include "main.h"
#include "gpio.h"
#include "lvgl.h"
#include "display_drv.h"

extern volatile uint32_t g_boot_stage;

/* ------------------------------------------------------------------ */
/* Local helpers                                                      */
/* ------------------------------------------------------------------ */

static void mini_test_delay(uint32_t ms)
{
    HAL_Delay(ms);
}

static void mini_test_backlight(uint8_t on)
{
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
  * @brief  Pump LVGL timer + flush for @p cycles iterations.
  *         Each iteration delays 5 ms, giving a total of cycles*5 ms.
  *
  * LVGL tick is provided by HAL_GetTick() (LV_TICK_CUSTOM=1),
  * so HAL_Delay() advances the tick naturally via SysTick.
  */
static void mini_test_pump(uint32_t cycles)
{
    uint32_t i;
    for (i = 0U; i < cycles; i++) {
        lv_timer_handler();
        mini_test_delay(5U);
    }
}

/**
  * @brief  Remove all child objects from a screen.
  *         Safe to call on the active screen — only explicitly-created
  *         children are deleted, not LVGL's internal objects.
  */
static void mini_test_clean_children(lv_obj_t *scr)
{
    lv_obj_t *child;
    while ((child = lv_obj_get_child(scr, NULL)) != NULL) {
        lv_obj_del(child);
    }
}

/**
  * @brief  Set the screen background to solid black.
  */
static void mini_test_black_bg(lv_obj_t *scr)
{
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
}

/* ------------------------------------------------------------------ */
/* Scene 1: Simple coloured label with a coloured background           */
/* ------------------------------------------------------------------ */
static void mini_test_scene1_label(lv_obj_t *scr)
{
    lv_obj_t *bg;
    lv_obj_t *label;

    g_boot_stage = 0x35U;
    mini_test_black_bg(scr);
    mini_test_pump(10U);  /* Flush background */
    g_boot_stage = 0x36U;  /* After first pump (black flush) */

    /* Green rectangle behind the label so we see a fill area */
    bg = lv_obj_create(scr);
    g_boot_stage = 0x37U;
    lv_obj_set_size(bg, 200, 80);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(bg, lv_color_hex(0x007700), 0);
    lv_obj_set_style_bg_opa(bg, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(bg, 0, 0);
    lv_obj_set_style_radius(bg, 8, 0);

    /* White label on the green background */
    label = lv_label_create(bg);
    g_boot_stage = 0x38U;
    lv_label_set_text(label, "LVGL OK");
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_center(label);

    mini_test_pump(100U);  /* 500 ms */
    g_boot_stage = 0x39U;  /* After second pump (scene render) */
}

/* ------------------------------------------------------------------ */
/* Scene 2: Colour bars (R, G, B)                                     */
/* ------------------------------------------------------------------ */
static void mini_test_scene2_bars(lv_obj_t *scr)
{
    lv_obj_t *bar_r, *bar_g, *bar_b;
    uint16_t bar_w = 80;
    uint16_t bar_h = 200;

    mini_test_black_bg(scr);
    mini_test_pump(10U);  /* Flush background */

    /* Red bar */
    bar_r = lv_obj_create(scr);
    lv_obj_set_size(bar_r, bar_w, bar_h);
    lv_obj_align(bar_r, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_color(bar_r, lv_color_hex(0xFF0000), 0);
    lv_obj_set_style_bg_opa(bar_r, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(bar_r, 0, 0);

    /* Green bar */
    bar_g = lv_obj_create(scr);
    lv_obj_set_size(bar_g, bar_w, bar_h);
    lv_obj_align(bar_g, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(bar_g, lv_color_hex(0x00FF00), 0);
    lv_obj_set_style_bg_opa(bar_g, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(bar_g, 0, 0);

    /* Blue bar */
    bar_b = lv_obj_create(scr);
    lv_obj_set_size(bar_b, bar_w, bar_h);
    lv_obj_align(bar_b, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_bg_color(bar_b, lv_color_hex(0x0000FF), 0);
    lv_obj_set_style_bg_opa(bar_b, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(bar_b, 0, 0);

    mini_test_pump(200U);  /* 1000 ms */
}

/* ------------------------------------------------------------------ */
/* Scene 3: Arc/ring spinner (exercises complex draw)                  */
/* ------------------------------------------------------------------ */
static void mini_test_scene3_spinner(lv_obj_t *scr)
{
    lv_obj_t *arc;
    int16_t angle;

    mini_test_black_bg(scr);
    mini_test_pump(10U);  /* Flush background */

    arc = lv_arc_create(scr);
    lv_obj_set_size(arc, 120, 120);
    lv_obj_center(arc);
    lv_arc_set_mode(arc, LV_ARC_MODE_NORMAL);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x00CC00), 0);
    lv_obj_set_style_arc_opa(arc, LV_OPA_COVER, 0);

    /* Animate the arc end-angle through a few frames */
    for (angle = 0; angle <= 360; angle += 15) {
        lv_arc_set_value(arc, angle);
        mini_test_pump(8U);  /* 40 ms per frame */
    }

    mini_test_pump(100U);  /* 500 ms */
}

/* ------------------------------------------------------------------ */
/* Summary scene: PASS or FAIL                                        */
/* ------------------------------------------------------------------ */
static void mini_test_show_result(lv_obj_t *scr, uint8_t pass)
{
    lv_obj_t *label;

    if (pass != 0U) {
        lv_obj_set_style_bg_color(scr, lv_color_hex(0x004400), 0);
    } else {
        lv_obj_set_style_bg_color(scr, lv_color_hex(0x440000), 0);
    }
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    mini_test_pump(10U);  /* Flush background */

    label = lv_label_create(scr);
    lv_label_set_text(label, pass ? "LVGL: PASS" : "LVGL: FAIL");
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_center(label);

    mini_test_pump(300U);  /* 1500 ms */
}

/* ------------------------------------------------------------------ */
/* Public entry point                                                 */
/* ------------------------------------------------------------------ */

/**
  * @brief  Run the minimal LVGL smoke test.
  *
  * Prerequisites (must already have been called by main.c):
  *   - HAL_Init()
  *   - SystemClock_Config()
  *   - MX_GPIO_Init()
  *   - MX_FSMC_Init()
  *
  * The function returns when the sequence is complete. It does **not**
  * infinite-loop — the caller (main.c) proceeds to FreeRTOS / full LVGL
  * initialisation as normal.
  *
  * Note: display_drv_init() internally calls lcd_init() which calls
  * ili9341_Init(). This is idempotent and safe even if the BSP self-test
  * already initialised the panel.
  */
void lvgl_mini_test(void)
{
    lv_obj_t *scr;
    uint8_t pass = 1U;

    /* ---- Platform init ---- */
    g_boot_stage = 0x30U;
    mini_test_backlight(1U);

    /* ---- LVGL core + display driver ---- */
    lv_init();
    g_boot_stage = 0x31U;
    if (display_drv_init() != 0) {
        /* Display driver registration failed — abort with failure */
        pass = 0U;
        scr = lv_scr_act();
        mini_test_show_result(scr, pass);
        return;
    }
    g_boot_stage = 0x32U;

    /* Use LVGL's default screen throughout — single-screen model
       avoids all the async lv_scr_load / active-screen lifecycle
       issues that cause hardfaults in LVGL 8.x bare-metal context. */
    scr = lv_scr_act();
    g_boot_stage = 0x33U;

    /* ---- Scene 1: Label ---- */
    g_boot_stage = 0x34U;
    mini_test_scene1_label(scr);
    mini_test_clean_children(scr);
    g_boot_stage = 0x3AU;

    /* ---- Scene 2: Colour bars ---- */
    mini_test_scene2_bars(scr);
    mini_test_clean_children(scr);

    /* ---- Scene 3: Spinner arc ---- */
    mini_test_scene3_spinner(scr);
    mini_test_clean_children(scr);

    /* ---- Summary ---- */
    mini_test_show_result(scr, pass);
    mini_test_clean_children(scr);

    /* ---- Tear-down: leave the LCD in a clean state for production path ---- */
    mini_test_black_bg(scr);
    mini_test_pump(20U);  /* Flush the black screen */

    mini_test_backlight(0U);
    g_boot_stage = 0x3FU;
}

#endif /* LVGL_MINI_TEST */