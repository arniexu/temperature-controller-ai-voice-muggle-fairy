/**
 * @file tianji_ui.c
 * @brief Main UI assembly - puts together all the Tianji Thermostat components
 * @note  Layout for 240x320 vertical screen:
 *        [0,0]  Status Bar (240x30)
 *        [0,30] Dial Area (240x195) - dial centered, particles behind
 *        [0,225] Realm Bar (240x22) - cultivation level indicators
 *        [0,247] Voice Core (240x35) - orb + status
 *        [0,282] Bottom Controls (240x38) - 3 buttons
 *        [0,320] Marquee
 */

#include "tianji_ui.h"
#include "tianji_data.h"
#include "tianji_particles.h"
#include "tianji_dial.h"
#include "tianji_voice.h"
#include <stdio.h>
#include <string.h>

/*=================================================================*/
/* Status Bar                                                       */
/*=================================================================*/

static void create_status_bar(tianji_ui_t *ui)
{
    ui->status_bar = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ui->status_bar, TIANJI_SCREEN_W, TIANJI_STATUS_H);
    lv_obj_set_pos(ui->status_bar, 0, 0);
    lv_obj_set_style_bg_color(ui->status_bar, lv_color_hex(TIANJI_COLOR_BG), 0);
    lv_obj_set_style_bg_opa(ui->status_bar, LV_OPA_80, 0);
    lv_obj_set_style_border_width(ui->status_bar, 0, 0);
    lv_obj_set_style_pad_all(ui->status_bar, 2, 0);
    lv_obj_clear_flag(ui->status_bar, LV_OBJ_FLAG_SCROLLABLE);

    /* Solar term label */
    ui->term_label = lv_label_create(ui->status_bar);
    const tianji_solar_term_t *term = tianji_get_current_term();
    char buf[32];
    snprintf(buf, sizeof(buf), "%s \xC2\xB7 %s", term->name, term->sub);
    lv_label_set_text(ui->term_label, buf);
    lv_obj_set_style_text_font(ui->term_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(ui->term_label, lv_color_hex(TIANJI_COLOR_NEON_GREEN), 0);
    lv_obj_set_pos(ui->term_label, 4, 2);

    /* Lunar date */
    ui->lunar_label = lv_label_create(ui->status_bar);
    char lunar_buf[32];
    tianji_get_lunar_date(lunar_buf);
    lv_label_set_text(ui->lunar_label, lunar_buf);
    lv_obj_set_style_text_font(ui->lunar_label, &lv_font_montserrat_8, 0);
    lv_obj_set_style_text_color(ui->lunar_label, lv_color_hex(0x6A6A8A), 0);
    lv_obj_set_pos(ui->lunar_label, 4, 16);

    /* Outdoor temp */
    ui->outdoor_label = lv_label_create(ui->status_bar);
    lv_label_set_text(ui->outdoor_label, "\xE5\xA4\x96 28\xC2\xB0" "C \xC2\xB7 52%");
    lv_obj_set_style_text_font(ui->outdoor_label, &lv_font_montserrat_8, 0);
    lv_obj_set_style_text_color(ui->outdoor_label, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
    lv_obj_align(ui->outdoor_label, LV_ALIGN_TOP_RIGHT, -30, 2);

    /* AQI badge */
    ui->aqi_badge = lv_obj_create(ui->status_bar);
    lv_obj_set_size(ui->aqi_badge, 28, 16);
    lv_obj_align(ui->aqi_badge, LV_ALIGN_TOP_RIGHT, -2, 4);
    lv_obj_set_style_radius(ui->aqi_badge, 8, 0);
    lv_obj_set_style_bg_color(ui->aqi_badge, lv_color_hex(0x0A1A0A), 0);
    lv_obj_set_style_bg_opa(ui->aqi_badge, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(ui->aqi_badge, lv_color_hex(TIANJI_COLOR_NEON_GREEN), 0);
    lv_obj_set_style_border_width(ui->aqi_badge, 1, 0);
    lv_obj_set_style_border_opa(ui->aqi_badge, LV_OPA_40, 0);
    lv_obj_set_style_pad_all(ui->aqi_badge, 0, 0);
    lv_obj_clear_flag(ui->aqi_badge, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *aqi_text = lv_label_create(ui->aqi_badge);
    lv_label_set_text(aqi_text, "\xE4\xBC\x98");
    lv_obj_set_style_text_font(aqi_text, &lv_font_montserrat_8, 0);
    lv_obj_set_style_text_color(aqi_text, lv_color_hex(TIANJI_COLOR_NEON_GREEN), 0);
    lv_obj_center(aqi_text);
}

/*=================================================================*/
/* Cultivation Level Bar                                            */
/*=================================================================*/

static lv_obj_t *realm_dots[7];

static void create_realm_bar(tianji_ui_t *ui, lv_obj_t *parent, lv_coord_t y)
{
    ui->realm_bar = lv_obj_create(parent);
    lv_obj_set_size(ui->realm_bar, TIANJI_SCREEN_W, TIANJI_REALM_H);
    lv_obj_set_pos(ui->realm_bar, 0, y);
    lv_obj_set_style_bg_opa(ui->realm_bar, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(ui->realm_bar, 0, 0);
    lv_obj_set_style_pad_all(ui->realm_bar, 2, 0);
    lv_obj_clear_flag(ui->realm_bar, LV_OBJ_FLAG_SCROLLABLE);

    /* 7 cultivation level indicators as small dots */
    const char *realm_names[7] = {
        "\xE7\x82\xBC\xE6\xB0\x94", "\xE7\xAD\x91\xE5\x9F\xBA", "\xE9\x87\x91\xE4\xB8\xB9",
        "\xE5\x85\x83\xE5\xA9\xB4", "\xE5\x8C\x96\xE7\xA5\x9E", "\xE7\x82\xBC\xE8\x99\x9A", "\xE5\x90\x88\xE4\xBD\x93"
    };

    lv_coord_t start_x = 8;
    lv_coord_t spacing = 32;
    for (int i = 0; i < 7; i++) {
        lv_obj_t *dot = lv_obj_create(ui->realm_bar);
        lv_obj_set_size(dot, 3, 3);
        lv_obj_set_pos(dot, start_x + i * spacing + 8, 2);
        lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(dot, lv_color_hex(0x2A2A4A), 0);
        lv_obj_set_style_border_width(dot, 0, 0);
        realm_dots[i] = dot;

        lv_obj_t *label = lv_label_create(ui->realm_bar);
        lv_label_set_text(label, realm_names[i]);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_8, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0x3A3A5A), 0);
        lv_obj_set_pos(label, start_x + i * spacing, 8);
    }
}

static void update_realm_bar(tianji_ui_t *ui)
{
    const tianji_cultivation_t *cult = tianji_get_cultivation(ui->current_temp);
    for (int i = 0; i < 7; i++) {
        uint16_t color = (ui->current_temp >= tianji_cultivation_levels[i].minTemp &&
                         ui->current_temp < tianji_cultivation_levels[i].maxTemp)
                         ? cult->color : 0x2A2A4A;
        lv_obj_set_style_bg_color(realm_dots[i], lv_color_hex(color), 0);
        if (color != 0x2A2A4A) {
            lv_obj_set_style_shadow_color(realm_dots[i], lv_color_hex(color), 0);
            lv_obj_set_style_shadow_width(realm_dots[i], 4, 0);
            lv_obj_set_style_shadow_opa(realm_dots[i], LV_OPA_60, 0);
        } else {
            lv_obj_set_style_shadow_width(realm_dots[i], 0, 0);
        }
    }
}

/*=================================================================*/
/* Bottom Controls                                                  */
/*=================================================================*/

static void btn_mode_cb(lv_event_t *e)
{
    tianji_ui_t *ui = (tianji_ui_t *)lv_event_get_user_data(e);
    ui->mode_idx = !ui->mode_idx;

    const char *labels[2] = { "\xE4\xBF\xAE\xE4\xBB\x99", "\xE5\x87\xA1\xE5\xB0\x98" };
    lv_obj_t *label = lv_obj_get_child(ui->btn_mode, 0);
    lv_label_set_text(label, labels[ui->mode_idx]);

    uint32_t colors[2] = { TIANJI_COLOR_NEON_GREEN, TIANJI_COLOR_ICE_BLUE };
    lv_obj_set_style_text_color(label, lv_color_hex(colors[ui->mode_idx]), 0);
    lv_obj_set_style_border_color(ui->btn_mode, lv_color_hex(colors[ui->mode_idx]), 0);
}

static void btn_wind_cb(lv_event_t *e)
{
    tianji_ui_t *ui = (tianji_ui_t *)lv_event_get_user_data(e);
    ui->wind_idx = !ui->wind_idx;

    const char *labels[2] = { "\xE5\xB7\xBD\xE9\xA3\x8E", "\xE7\xA6\xBB\xE7\x81\xAB" };
    lv_obj_t *label = lv_obj_get_child(ui->btn_wind, 0);
    lv_label_set_text(label, labels[ui->wind_idx]);

    uint32_t colors[2] = { TIANJI_COLOR_NEON_GREEN, TIANJI_COLOR_FIRE };
    lv_obj_set_style_text_color(label, lv_color_hex(colors[ui->wind_idx]), 0);
    lv_obj_set_style_border_color(ui->btn_wind, lv_color_hex(colors[ui->wind_idx]), 0);
}

static void btn_timer_cb(lv_event_t *e)
{
    tianji_ui_t *ui = (tianji_ui_t *)lv_event_get_user_data(e);
    ui->timer_idx = !ui->timer_idx;

    const char *labels[2] = { "\xE7\x94\xB2\xE5\xAD\x90", "\xE5\xBA\x9A\xE7\x94\xB3" };
    lv_obj_t *label = lv_obj_get_child(ui->btn_timer, 0);
    lv_label_set_text(label, labels[ui->timer_idx]);
}

static lv_obj_t* create_neon_btn(lv_obj_t *parent, lv_coord_t x, lv_coord_t y,
                                  const char *label_text, uint32_t color,
                                  lv_event_cb_t cb, void *user)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 68, 34);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_style_radius(btn, 10, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(TIANJI_COLOR_BG), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_80, 0);
    lv_obj_set_style_border_color(btn, lv_color_hex(color), 0);
    lv_obj_set_style_border_width(btn, 1, 0);
    lv_obj_set_style_border_opa(btn, LV_OPA_50, 0);
    lv_obj_set_style_shadow_color(btn, lv_color_hex(color), 0);
    lv_obj_set_style_shadow_width(btn, 0, 0);
    lv_obj_set_style_pad_all(btn, 2, 0);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, label_text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(color), 0);
    lv_obj_center(label);

    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, user);

    return btn;
}

static void create_bottom_controls(tianji_ui_t *ui)
{
    lv_obj_t *bar = lv_obj_create(lv_scr_act());
    lv_obj_set_size(bar, TIANJI_SCREEN_W, TIANJI_BOTTOM_H);
    lv_obj_set_pos(bar, 0, TIANJI_BOTTOM_Y);
    lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(bar, 0, 0);
    lv_obj_set_style_pad_all(bar, 2, 0);
    lv_obj_clear_flag(bar, LV_OBJ_FLAG_SCROLLABLE);

    ui->btn_mode = create_neon_btn(bar, 8, 2,
        "\xE4\xBF\xAE\xE4\xBB\x99", TIANJI_COLOR_NEON_GREEN, btn_mode_cb, ui);
    ui->btn_wind = create_neon_btn(bar, 86, 2,
        "\xE5\xB7\xBD\xE9\xA3\x8E", TIANJI_COLOR_NEON_GREEN, btn_wind_cb, ui);
    ui->btn_timer = create_neon_btn(bar, 164, 2,
        "\xE7\x94\xB2\xE5\xAD\x90", TIANJI_COLOR_ICE_BLUE, btn_timer_cb, ui);
}

/*=================================================================*/
/* Marquee                                                          */
/*=================================================================*/

static const char *marquee_text =
    "\xE7\xAB\x8B\xE6\x98\xA5 \xC2\xB7 \xE9\x9B\xA8\xE6\xB0\xB4 \xC2\xB7 \xE6\x83\x8A\xE8\x9B\xB0 \xC2\xB7 \xE6\x98\xA5\xE5\x88\x86 \xC2\xB7 \xE6\xB8\x85\xE6\x98\x8E \xC2\xB7 \xE8\xB0\xB7\xE9\x9B\xA8 \xC2\xB7 "
    "\xE7\xAB\x8B\xE5\xA4\x8F \xC2\xB7 \xE5\xB0\x8F\xE6\xBB\xA1 \xC2\xB7 \xE8\x8A\x92\xE7\xA7\x8D \xC2\xB7 \xE5\xA4\x8F\xE8\x87\xB3 \xC2\xB7 \xE5\xB0\x8F\xE6\x9A\x91 \xC2\xB7 \xE5\xA4\xA7\xE6\x9A\x91 \xC2\xB7 "
    "\xE7\xAB\x8B\xE7\xA7\x8B \xC2\xB7 \xE5\xA4\x84\xE6\x9A\x91 \xC2\xB7 \xE7\x99\xBD\xE9\x9C\xB2 \xC2\xB7 \xE7\xA7\x8B\xE5\x88\x86 \xC2\xB7 \xE5\xAF\x92\xE9\x9C\xB2 \xC2\xB7 \xE9\x9C\x9C\xE9\x99\x8D \xC2\xB7 "
    "\xE7\xAB\x8B\xE5\x86\xAC \xC2\xB7 \xE5\xB0\x8F\xE9\x9B\xAA \xC2\xB7 \xE5\xA4\xA7\xE9\x9B\xAA \xC2\xB7 \xE5\x86\xAC\xE8\x87\xB3 \xC2\xB7 \xE5\xB0\x8F\xE5\xAF\x92 \xC2\xB7 \xE5\xA4\xA7\xE5\xAF\x92 \xC2\xB7 ";

static void marquee_timer_cb(lv_timer_t *timer)
{
    tianji_ui_t *ui = (tianji_ui_t *)timer->user_data;
    ui->marquee_offset -= 1;
    if (ui->marquee_offset < -400) {
        ui->marquee_offset = 0;
    }
    lv_obj_set_style_translate_x(ui->marquee_label, ui->marquee_offset, 0);
}

static void create_marquee(tianji_ui_t *ui)
{
    ui->marquee_label = lv_label_create(lv_scr_act());
    lv_label_set_text(ui->marquee_label, marquee_text);
    lv_obj_set_style_text_font(ui->marquee_label, &lv_font_montserrat_8, 0);
    lv_obj_set_style_text_color(ui->marquee_label, lv_color_hex(0x1A1A3E), 0);
    lv_obj_set_pos(ui->marquee_label, 0, 318);

    ui->marquee_timer = lv_timer_create(marquee_timer_cb, 50, ui);
}

/*=================================================================*/
/* Voice activation callback                                        */
/*=================================================================*/

static void voice_activated(void *user_data)
{
    tianji_ui_t *ui = (tianji_ui_t *)user_data;

    /* Simulate voice processing */
    tj_voice_set_state(ui->voice, TJ_VOICE_PROCESSING);
    tj_voice_set_text(ui->voice, "\xE5\xA4\x84\xE7\x90\x86\xE4\xB8\xAD...");

    /* After 2 seconds, show result */
    /* In real implementation, this would be triggered by ASR result */
    /* For demo: set temp based on mock command */
    lv_timer_t *result_timer = lv_timer_create((lv_timer_cb_t)NULL, 2000, ui);
    (void)result_timer; /* Schedule via app main loop */
}

/*=================================================================*/
/* Public API                                                       */
/*=================================================================*/

tianji_ui_t* tianji_ui_init(void)
{
    tianji_ui_t *ui = lv_mem_alloc(sizeof(tianji_ui_t));
    if (!ui) return NULL;
    memset(ui, 0, sizeof(tianji_ui_t));

    ui->current_temp = TIANJI_TEMP_DEFAULT;
    ui->mode_idx = 0;
    ui->wind_idx = 0;
    ui->timer_idx = 0;

    /* Set screen background */
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(TIANJI_COLOR_BG), 0);

    /* Create status bar */
    create_status_bar(ui);

    /* Create particle helix behind dial */
    /* Place particles in the dial area */
    ui->particles = tj_particles_create(
        lv_scr_act(),
        35, TIANJI_DIAL_Y + 10,
        170, 170,
        TIANJI_COLOR_NEON_GREEN,
        TIANJI_COLOR_ICE_BLUE
    );
    if (ui->particles) {
        tj_particles_set_speed(ui->particles, 4);
        tj_particles_start(ui->particles);
    }

    /* Create dial (on top of particles) */
    ui->dial = tj_dial_create(lv_scr_act(), 30, TIANJI_DIAL_Y + 5);
    tj_dial_set_temp(ui->dial, ui->current_temp);

    /* Create cultivation realm bar */
    create_realm_bar(ui, lv_scr_act(), TIANJI_REALM_Y);
    update_realm_bar(ui);

    /* Create voice core */
    ui->voice = tj_voice_create(lv_scr_act(), 70, TIANJI_VOICE_Y);
    tj_voice_set_callback(ui->voice, voice_activated, ui);

    /* Create bottom controls */
    create_bottom_controls(ui);

    /* Create marquee */
    create_marquee(ui);

    return ui;
}

void tianji_ui_deinit(tianji_ui_t *ui)
{
    if (!ui) return;
    if (ui->particles) tj_particles_destroy(ui->particles);
    if (ui->dial) tj_dial_destroy(ui->dial);
    if (ui->voice) tj_voice_destroy(ui->voice);
    if (ui->marquee_timer) lv_timer_del(ui->marquee_timer);
    lv_mem_free(ui);
}

void tianji_ui_update_status(tianji_ui_t *ui)
{
    if (!ui) return;
    /* Update realm indicators */
    update_realm_bar(ui);
}

void tianji_ui_set_temp(tianji_ui_t *ui, int16_t temp_tenth)
{
    if (!ui) return;
    ui->current_temp = temp_tenth;
    if (ui->dial) {
        tj_dial_set_temp(ui->dial, temp_tenth);
    }
    update_realm_bar(ui);

    /* Update particle speed based on temp */
    if (ui->particles) {
        uint8_t speed = (uint8_t)(1 + (temp_tenth - 160) / 14);
        tj_particles_set_speed(ui->particles, speed);
    }
}

int16_t tianji_ui_get_temp(tianji_ui_t *ui)
{
    if (!ui) return TIANJI_TEMP_DEFAULT;
    if (ui->dial) {
        ui->current_temp = tj_dial_get_temp(ui->dial);
    }
    return ui->current_temp;
}

void tianji_ui_voice_result(tianji_ui_t *ui, const char *cmd, int16_t target_temp)
{
    if (!ui || !ui->voice) return;

    tj_voice_set_state(ui->voice, TJ_VOICE_RESPONDING);

    if (target_temp > 0) {
        char buf[32];
        snprintf(buf, sizeof(buf), "\xE5\xB7\xB2\xE8\xAE\xBE\xE5\xAE\x9A %d\xC2\xB0" "C", target_temp / 10);
        tj_voice_set_text(ui->voice, buf);
        tianji_ui_set_temp(ui, target_temp);
    } else {
        tj_voice_set_text(ui->voice, cmd ? cmd : "\xE5\xB7\xB2\xE6\x89\xA7\xE8\xA1\x8C");
    }

    /* Auto return to idle */
    /* Use a one-shot timer */
    /* In production, use lv_timer_create with limited repeat */
}
