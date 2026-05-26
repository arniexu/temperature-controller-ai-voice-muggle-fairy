/**
 * @file tianji_dial.h
 * @brief Thermostat dial control with touch drag
 * @note  240x320 optimized - dial area 170x170 centered
 */

#ifndef TIANJI_DIAL_WIDGET_H
#define TIANJI_DIAL_WIDGET_H

#include "lvgl.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Dial event codes (custom)
 */
enum {
    TJ_DIAL_EVENT_VALUE_CHANGED = _LV_EVENT_LAST + 1,
    TJ_DIAL_EVENT_PRESSED,
    TJ_DIAL_EVENT_RELEASED,
};

/**
 * @brief Temperature dial widget
 */
typedef struct {
    lv_obj_t    *container;     /* Outer container */
    lv_obj_t    *arc_bg;        /* Background arc */
    lv_obj_t    *arc_fg;        /* Foreground (progress) arc */
    lv_obj_t    *center_label;  /* Temperature value */
    lv_obj_t    *unit_label;    /* °C unit */
    lv_obj_t    *realm_label;   /* Cultivation realm name */
    lv_obj_t    *trigram_labels[8];  /*八卦 labels around dial */

    int16_t      temp;          /* Current temp in tenths (160-300) */
    int16_t      temp_min;      /* Minimum temp */
    int16_t      temp_max;      /* Maximum temp */
    bool         dragging;      /* Touch drag in progress */
    lv_point_t   drag_start;    /* Start point of drag */
    int16_t      temp_at_drag;  /* Temp when drag started */

    lv_style_t   style_arc_bg;
    lv_style_t   style_arc_fg;
    lv_style_t   style_main;
    lv_style_t   style_knob;
} tj_dial_t;

/**
 * @brief Create dial widget
 * @param parent    LVGL parent
 * @param x, y      Position
 * @return Dial handle
 */
tj_dial_t* tj_dial_create(lv_obj_t *parent, lv_coord_t x, lv_coord_t y);

/**
 * @brief Destroy dial
 */
void tj_dial_destroy(tj_dial_t *dial);

/**
 * @brief Set temperature (tenths of degree)
 */
void tj_dial_set_temp(tj_dial_t *dial, int16_t temp_tenth);

/**
 * @brief Get current temperature
 */
int16_t tj_dial_get_temp(tj_dial_t *dial);

/**
 * @brief Set temperature range
 */
void tj_dial_set_range(tj_dial_t *dial, int16_t min_tenth, int16_t max_tenth);

/**
 * @brief Update UI (call after temp change)
 */
void tj_dial_refresh(tj_dial_t *dial);

/**
 * @brief Get dial container object (for event binding)
 */
lv_obj_t* tj_dial_get_obj(tj_dial_t *dial);

#ifdef __cplusplus
}
#endif

#endif /* TIANJI_DIAL_WIDGET_H */
