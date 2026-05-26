/**
 * @file tianji_dial.c
 * @brief Thermostat dial with arc progress and touch drag
 */

#include "tianji_dial.h"
#include "tianji_data.h"
#include <stdio.h>
#include <math.h>

/*=================================================================*/
/* Internal helpers                                                */
/*=================================================================*/

static void update_arc_from_temp(tj_dial_t *d)
{
    /* Map temp (160-300) to arc angle (0-270 degrees) */
    float pct = (float)(d->temp - d->temp_min) / (float)(d->temp_max - d->temp_min);
    if (pct < 0.0f) pct = 0.0f;
    if (pct > 1.0f) pct = 1.0f;

    /* LVGL arc: 0 deg = 6 o'clock, we want -135 to +135 */
    int16_t angle = (int16_t)(pct * 270.0f);
    lv_arc_set_value(d->arc_fg, angle);

    /* Update gradient color based on temp */
    uint16_t grad_color = tianji_temp_gradient(pct);
    lv_obj_set_style_arc_color(d->arc_fg, lv_color_hex(grad_color), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(d->arc_fg, lv_color_hex(grad_color), LV_PART_KNOB);

    /* Glow effect via shadow */
    lv_obj_set_style_shadow_color(d->arc_fg, lv_color_hex(grad_color), LV_PART_KNOB);
}

static void update_labels(tj_dial_t *d)
{
    char buf[8];

    /* Temperature integer part */
    tianji_format_temp(d->temp, buf);
    lv_label_set_text(d->center_label, buf);

    /* Cultivation realm */
    const tianji_cultivation_t *cult = tianji_get_cultivation(d->temp);
    if (cult) {
        lv_label_set_text(d->realm_label, cult->name);
        lv_obj_set_style_text_color(d->realm_label, lv_color_hex(cult->color), 0);
    }
}

/*=================================================================*/
/* Event handlers                                                  */
/*=================================================================*/

static void dial_event_cb(lv_event_t *e)
{
    tj_dial_t *d = (tj_dial_t *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_PRESSED) {
        d->dragging = true;
        lv_indev_t *indev = lv_indev_get_act();
        lv_indev_get_vect(indev, &d->drag_start);
        d->temp_at_drag = d->temp;
    }
    else if (code == LV_EVENT_PRESSING && d->dragging) {
        lv_indev_t *indev = lv_indev_get_act();
        lv_point_t vect;
        lv_indev_get_vect(indev, &vect);

        /* Horizontal drag increases temp, vertical decreases */
        int16_t delta = (int16_t)(vect.x * 0.3f - vect.y * 0.2f);
        int16_t new_temp = d->temp_at_drag + delta;

        if (new_temp < d->temp_min) new_temp = d->temp_min;
        if (new_temp > d->temp_max) new_temp = d->temp_max;

        if (new_temp != d->temp) {
            d->temp = new_temp;
            update_arc_from_temp(d);
            update_labels(d);
        }
    }
    else if (code == LV_EVENT_RELEASED) {
        d->dragging = false;
    }
}

/*=================================================================*/
/* Public API                                                      */
/*=================================================================*/

tj_dial_t* tj_dial_create(lv_obj_t *parent, lv_coord_t x, lv_coord_t y)
{
    tj_dial_t *d = lv_mem_alloc(sizeof(tj_dial_t));
    if (!d) return NULL;
    memset(d, 0, sizeof(tj_dial_t));

    d->temp = TIANJI_TEMP_DEFAULT;
    d->temp_min = TIANJI_TEMP_MIN;
    d->temp_max = TIANJI_TEMP_MAX;
    d->dragging = false;

    /* Container */
    d->container = lv_obj_create(parent);
    lv_obj_set_pos(d->container, x, y);
    lv_obj_set_size(d->container, 180, 180);
    lv_obj_set_style_bg_opa(d->container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(d->container, 0, 0);
    lv_obj_set_style_pad_all(d->container, 0, 0);
    lv_obj_clear_flag(d->container, LV_OBJ_FLAG_SCROLLABLE);

    /* Background arc (decorative ring) */
    d->arc_bg = lv_arc_create(d->container);
    lv_obj_set_size(d->arc_bg, 170, 170);
    lv_obj_center(d->arc_bg);
    lv_arc_set_rotation(d->arc_bg, 135);
    lv_arc_set_bg_angles(d->arc_bg, 0, 270);
    lv_arc_set_value(d->arc_bg, 100);
    lv_obj_set_style_arc_color(d->arc_bg, lv_color_hex(0x1A1A2E), LV_PART_MAIN);
    lv_obj_set_style_arc_width(d->arc_bg, 3, LV_PART_MAIN);
    lv_obj_set_style_arc_rounded(d->arc_bg, true, LV_PART_MAIN);
    lv_obj_remove_style(d->arc_bg, NULL, LV_PART_KNOB);
    lv_obj_remove_style(d->arc_bg, NULL, LV_PART_INDICATOR);

    /* Foreground arc (progress) */
    d->arc_fg = lv_arc_create(d->container);
    lv_obj_set_size(d->arc_fg, 170, 170);
    lv_obj_center(d->arc_fg);
    lv_arc_set_rotation(d->arc_fg, 135);
    lv_arc_set_bg_angles(d->arc_fg, 0, 270);
    lv_arc_set_range(d->arc_fg, 0, 270);
    lv_arc_set_value(d->arc_fg, 135); /* halfway for 24C */

    lv_obj_set_style_arc_color(d->arc_fg, lv_color_hex(TIANJI_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_style_arc_width(d->arc_fg, 4, LV_PART_MAIN);

    lv_obj_set_style_arc_color(d->arc_fg, lv_color_hex(TIANJI_COLOR_NEON_GREEN), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(d->arc_fg, 4, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(d->arc_fg, true, LV_PART_INDICATOR);

    /* Knob style - glowing orb */
    lv_obj_set_style_bg_color(d->arc_fg, lv_color_hex(TIANJI_COLOR_NEON_GREEN), LV_PART_KNOB);
    lv_obj_set_style_bg_opa(d->arc_fg, LV_OPA_COVER, LV_PART_KNOB);
    lv_obj_set_style_shadow_color(d->arc_fg, lv_color_hex(TIANJI_COLOR_NEON_GREEN), LV_PART_KNOB);
    lv_obj_set_style_shadow_width(d->arc_fg, 10, LV_PART_KNOB);
    lv_obj_set_style_shadow_opa(d->arc_fg, LV_OPA_60, LV_PART_KNOB);
    lv_obj_set_style_pad_all(d->arc_fg, 5, LV_PART_KNOB);

    /* Remove arc default click handling, we'll do our own drag */
    lv_obj_clear_flag(d->arc_fg, LV_OBJ_FLAG_CLICKABLE);

    /* Center temperature label */
    d->center_label = lv_label_create(d->container);
    lv_label_set_text(d->center_label, "24.0");
    lv_obj_set_style_text_font(d->center_label, &lv_font_montserrat_36, 0);
    lv_obj_set_style_text_color(d->center_label, lv_color_hex(TIANJI_COLOR_NEON_GREEN), 0);
    lv_obj_align(d->center_label, LV_ALIGN_CENTER, -5, -8);

    /* Unit label */
    d->unit_label = lv_label_create(d->container);
    lv_label_set_text(d->unit_label, "\xC2\xB0" "C");
    lv_obj_set_style_text_font(d->unit_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(d->unit_label, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
    lv_obj_align_to(d->unit_label, d->center_label, LV_ALIGN_OUT_RIGHT_TOP, 2, 4);

    /* Realm label */
    d->realm_label = lv_label_create(d->container);
    lv_label_set_text(d->realm_label, "\xE5\x85\x83\xE5\xA9\xB4");
    lv_obj_set_style_text_font(d->realm_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(d->realm_label, lv_color_hex(TIANJI_COLOR_NEON_GREEN), 0);
    lv_obj_align(d->realm_label, LV_ALIGN_CENTER, 0, 28);

    /* Trigram labels around the dial */
    const char *trigram_names[8] = {
        "\xE4\xB9\xBE", "\xE5\x85\x91", "\xE7\xA6\xBB", "\xE9\x9C\x87",
        "\xE7\xBD\x8B", "\xE5\x9D\x8E", "\xE8\x89\xAE", "\xE5\x9D\xA4"
    };
    for (int i = 0; i < 8; i++) {
        d->trigram_labels[i] = lv_label_create(d->container);
        lv_label_set_text(d->trigram_labels[i], trigram_names[i]);
        lv_obj_set_style_text_font(d->trigram_labels[i], &lv_font_montserrat_8, 0);
        lv_obj_set_style_text_color(d->trigram_labels[i],
            lv_color_hex(0x3A3A5A), 0);
        /* Position around circle */
        float angle_rad = (i * 45 - 90) * 3.14159f / 180.0f;
        lv_coord_t tx = (lv_coord_t)(85 + 78 * cosf(angle_rad) - 4);
        lv_coord_t ty = (lv_coord_t)(85 + 78 * sinf(angle_rad) - 4);
        lv_obj_set_pos(d->trigram_labels[i], tx, ty);
    }

    /* Event handling - on container for drag */
    lv_obj_add_event_cb(d->container, dial_event_cb, LV_EVENT_ALL, d);
    lv_obj_add_flag(d->container, LV_OBJ_FLAG_CLICKABLE);

    /* Initial sync */
    update_arc_from_temp(d);
    update_labels(d);

    return d;
}

void tj_dial_destroy(tj_dial_t *dial)
{
    if (!dial) return;
    if (dial->container) {
        lv_obj_del(dial->container);
    }
    lv_mem_free(dial);
}

void tj_dial_set_temp(tj_dial_t *dial, int16_t temp_tenth)
{
    if (!dial) return;
    if (temp_tenth < dial->temp_min) temp_tenth = dial->temp_min;
    if (temp_tenth > dial->temp_max) temp_tenth = dial->temp_max;
    dial->temp = temp_tenth;
    update_arc_from_temp(dial);
    update_labels(dial);
}

int16_t tj_dial_get_temp(tj_dial_t *dial)
{
    return dial ? dial->temp : 0;
}

void tj_dial_set_range(tj_dial_t *dial, int16_t min_tenth, int16_t max_tenth)
{
    if (!dial) return;
    dial->temp_min = min_tenth;
    dial->temp_max = max_tenth;
    if (dial->temp < min_tenth) dial->temp = min_tenth;
    if (dial->temp > max_tenth) dial->temp = max_tenth;
    update_arc_from_temp(dial);
}

void tj_dial_refresh(tj_dial_t *dial)
{
    if (!dial) return;
    update_arc_from_temp(dial);
    update_labels(dial);
}

lv_obj_t* tj_dial_get_obj(tj_dial_t *dial)
{
    return dial ? dial->container : NULL;
}
