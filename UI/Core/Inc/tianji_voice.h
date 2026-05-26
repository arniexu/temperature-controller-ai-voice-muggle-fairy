/**
 * @file tianji_voice.h
 * @brief AI Voice Interaction Core UI
 * @note  Orbital rings + glowing orb button + status text
 */

#ifndef TIANJI_VOICE_WIDGET_H
#define TIANJI_VOICE_WIDGET_H

#include "lvgl.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Voice core states
 */
typedef enum {
    TJ_VOICE_IDLE = 0,      /* Waiting - orb pulses slowly */
    TJ_VOICE_LISTENING,     /* Active listening - orb vibrates */
    TJ_VOICE_PROCESSING,    /* Processing - rings spin fast */
    TJ_VOICE_RESPONDING,    /* Showing response text */
} tj_voice_state_t;

/**
 * @brief Voice core widget
 */
typedef struct {
    lv_obj_t           *container;
    lv_obj_t           *ring1;          /* Outer orbital ring */
    lv_obj_t           *ring2;          /* Middle ring */
    lv_obj_t           *ring3;          /* Inner ring */
    lv_obj_t           *orb_btn;        /* Center glowing orb button */
    lv_obj_t           *orb_inner;      /* Inner glow */
    lv_obj_t           *status_label;   /* Main status text */
    lv_obj_t           *hint_label;     /* Subtle hint text */

    tj_voice_state_t    state;
    lv_timer_t         *anim_timer;     /* Animation timer */
    uint16_t            anim_frame;     /* Animation frame counter */
    uint32_t            last_activity;  /* Tick of last state change */

    /* Callback for voice activation */
    void (*on_activate)(void *user_data);
    void *user_data;
} tj_voice_t;

/**
 * @brief Create voice core widget
 */
tj_voice_t* tj_voice_create(lv_obj_t *parent, lv_coord_t x, lv_coord_t y);

/**
 * @brief Destroy voice core
 */
void tj_voice_destroy(tj_voice_t *vc);

/**
 * @brief Set state
 */
void tj_voice_set_state(tj_voice_t *vc, tj_voice_state_t state);

/**
 * @brief Get current state
 */
tj_voice_state_t tj_voice_get_state(tj_voice_t *vc);

/**
 * @brief Set status text
 */
void tj_voice_set_text(tj_voice_t *vc, const char *text);

/**
 * @brief Set activation callback
 */
void tj_voice_set_callback(tj_voice_t *vc, void (*cb)(void*), void *user);

/**
 * @brief Pulse animation trigger (one-shot)
 */
void tj_voice_pulse(tj_voice_t *vc);

#ifdef __cplusplus
}
#endif

#endif /* TIANJI_VOICE_WIDGET_H */
