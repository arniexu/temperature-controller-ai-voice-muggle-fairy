/**
 * @file tianji_ui.h
 * @brief Main UI assembly for Tianji Thermostat
 * @note  Assembles all widgets: dial, voice, particles, controls, status bar
 */

#ifndef TIANJI_UI_H
#define TIANJI_UI_H

#include "lvgl.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Main UI context
 */
typedef struct {
    /* Status bar elements */
    lv_obj_t       *status_bar;
    lv_obj_t       *term_label;
    lv_obj_t       *lunar_label;
    lv_obj_t       *outdoor_label;
    lv_obj_t       *aqi_badge;

    /* Main dial */
    struct tj_dial_s    *dial;

    /* Particle effect */
    struct tj_particle_helix_s *particles;

    /* Voice core */
    struct tj_voice_s   *voice;

    /* Bottom control buttons */
    lv_obj_t       *btn_mode;
    lv_obj_t       *btn_wind;
    lv_obj_t       *btn_timer;

    /* Cultivation level indicators */
    lv_obj_t       *realm_bar;

    /* State */
    int16_t         current_temp;
    uint8_t         mode_idx;       /* 0=xiuxian, 1=fanchen */
    uint8_t         wind_idx;       /* 0=xunfeng, 1=lihuo */
    uint8_t         timer_idx;      /* 0=jiazi, 1=gengshen */

    /* Marquee */
    lv_obj_t       *marquee_label;
    lv_timer_t     *marquee_timer;
    int16_t         marquee_offset;
} tianji_ui_t;

/**
 * @brief Initialize complete UI
 * @return UI context pointer
 */
tianji_ui_t* tianji_ui_init(void);

/**
 * @brief Deinitialize UI
 */
void tianji_ui_deinit(tianji_ui_t *ui);

/**
 * @brief Update status bar (call periodically)
 */
void tianji_ui_update_status(tianji_ui_t *ui);

/**
 * @brief Set temperature from external source (e.g. voice command)
 */
void tianji_ui_set_temp(tianji_ui_t *ui, int16_t temp_tenth);

/**
 * @brief Get current temperature
 */
int16_t tianji_ui_get_temp(tianji_ui_t *ui);

/**
 * @brief Process voice command result
 */
void tianji_ui_voice_result(tianji_ui_t *ui, const char *cmd, int16_t target_temp);

#ifdef __cplusplus
}
#endif

#endif /* TIANJI_UI_H */
