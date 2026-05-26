/**
 * @file tianji_voice.c
 * @brief AI Voice Interaction Core implementation
 * @note  Orbital rings animation using LVGL animations
 *        Glowing orb with CSS-style shadow effects
 */

#include "tianji_voice.h"
#include "tianji_data.h"
#include <string.h>

/*=================================================================*/
/* Animation constants                                               */
/*=================================================================*/
#define ORB_SIZE            28
#define RING1_W             90
#define RING2_W             70
#define RING3_W             52

/*=================================================================*/
/* Internal: Orb animation - continuous rotation                     */
/*=================================================================*/

static void anim_ring_rotate(void *var, int32_t v)
{
    lv_obj_t *ring = (lv_obj_t *)var;
    /* Simulate 3D perspective by scaling X */
    float scale_x = 0.6f;
    int16_t w = lv_obj_get_width(ring);
    int16_t h = lv_obj_get_height(ring);
    /* Use LVGL transform */
    lv_obj_set_style_transform_angle(ring, v * 10, 0);
}

/*=================================================================*/
/* Internal: Animation timer callback                                */
/*=================================================================*/

static void voice_anim_timer(lv_timer_t *timer)
{
    tj_voice_t *vc = (tj_voice_t *)timer->user_data;
    vc->anim_frame++;

    if (vc->state == TJ_VOICE_IDLE) {
        /* Slow pulse */
        uint8_t pulse = (uint8_t)(128 + 64 * tianji_sin((int16_t)(vc->anim_frame * 3)) / 256);
        lv_obj_set_style_bg_opa(vc->orb_btn, (lv_opa_t)((pulse * LV_OPA_COVER) / 255), 0);
    }
    else if (vc->state == TJ_VOICE_LISTENING) {
        /* Fast vibration */
        uint8_t vib = (uint8_t)(100 + 80 * tianji_sin((int16_t)(vc->anim_frame * 15)) / 256);
        lv_obj_set_style_bg_opa(vc->orb_btn, (lv_opa_t)((vib * LV_OPA_COVER) / 255), 0);

        /* Scale oscillation via padding */
        int8_t pad = (int8_t)(tianji_sin((int16_t)(vc->anim_frame * 12)) / 32);
        lv_obj_set_style_pad_all(vc->orb_btn, 4 + pad, 0);
    }
    else if (vc->state == TJ_VOICE_PROCESSING) {
        /* Rings spin fast - using transform angle */
        lv_obj_set_style_transform_angle(vc->ring1, vc->anim_frame * 80, 0);
        lv_obj_set_style_transform_angle(vc->ring2, -vc->anim_frame * 100, 0);
        lv_obj_set_style_transform_angle(vc->ring3, vc->anim_frame * 120, 0);
    }
}

/*=================================================================*/
/* Internal: Orb button click handler                                */
/*=================================================================*/

static void orb_click_cb(lv_event_t *e)
{
    tj_voice_t *vc = (tj_voice_t *)lv_event_get_user_data(e);

    /* State machine */
    switch (vc->state) {
        case TJ_VOICE_IDLE:
            tj_voice_set_state(vc, TJ_VOICE_LISTENING);
            tj_voice_set_text(vc, "\xE9\xA6\x96\xE5\x9C\xA8\xE4\xB8\xAD..."); /* 聆听中... */
            if (vc->on_activate) {
                vc->on_activate(vc->user_data);
            }
            /* Auto return to idle after 3 seconds */
            lv_timer_reset(vc->anim_timer);
            break;

        case TJ_VOICE_LISTENING:
        case TJ_VOICE_PROCESSING:
            tj_voice_set_state(vc, TJ_VOICE_IDLE);
            tj_voice_set_text(vc, "\xE6\xB0\x94\xE8\x81\x9A\xE7\xA5\x9E\xE5\x87\x9D"); /* 气聚神凝 */
            break;

        case TJ_VOICE_RESPONDING:
            tj_voice_set_state(vc, TJ_VOICE_IDLE);
            tj_voice_set_text(vc, "\xE6\xB0\x94\xE8\x81\x9A\xE7\xA5\x9E\xE5\x87\x9D");
            break;
    }
}

/*=================================================================*/
/* Public API                                                        */
/*=================================================================*/

tj_voice_t* tj_voice_create(lv_obj_t *parent, lv_coord_t x, lv_coord_t y)
{
    tj_voice_t *vc = lv_mem_alloc(sizeof(tj_voice_t));
    if (!vc) return NULL;
    memset(vc, 0, sizeof(tj_voice_t));

    vc->state = TJ_VOICE_IDLE;
    vc->on_activate = NULL;
    vc->user_data = NULL;
    vc->anim_frame = 0;

    /* Container */
    vc->container = lv_obj_create(parent);
    lv_obj_set_pos(vc->container, x, y);
    lv_obj_set_size(vc->container, 100, 90);
    lv_obj_set_style_bg_opa(vc->container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(vc->container, 0, 0);
    lv_obj_set_style_pad_all(vc->container, 0, 0);
    lv_obj_clear_flag(vc->container, LV_OBJ_FLAG_SCROLLABLE);

    /* Ring 1 - outer */
    vc->ring1 = lv_obj_create(vc->container);
    lv_obj_set_size(vc->ring1, RING1_W, RING1_W);
    lv_obj_center(vc->ring1);
    lv_obj_set_style_radius(vc->ring1, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(vc->ring1, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(vc->ring1, lv_color_hex(0x1A1A3E), 0);
    lv_obj_set_style_border_width(vc->ring1, 1, 0);
    lv_obj_set_style_border_opa(vc->ring1, LV_OPA_50, 0);
    lv_obj_set_style_transform_width(vc->ring1, -RING1_W / 3, 0);
    lv_obj_clear_flag(vc->ring1, LV_OBJ_FLAG_CLICKABLE);

    /* Ring 2 - middle */
    vc->ring2 = lv_obj_create(vc->container);
    lv_obj_set_size(vc->ring2, RING2_W, RING2_W);
    lv_obj_center(vc->ring2);
    lv_obj_set_style_radius(vc->ring2, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(vc->ring2, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(vc->ring2, lv_color_hex(0x1A1A3E), 0);
    lv_obj_set_style_border_width(vc->ring2, 1, 0);
    lv_obj_set_style_border_opa(vc->ring2, LV_OPA_40, 0);
    lv_obj_set_style_transform_width(vc->ring2, -RING2_W / 3, 0);
    lv_obj_clear_flag(vc->ring2, LV_OBJ_FLAG_CLICKABLE);

    /* Ring 3 - inner */
    vc->ring3 = lv_obj_create(vc->container);
    lv_obj_set_size(vc->ring3, RING3_W, RING3_W);
    lv_obj_center(vc->ring3);
    lv_obj_set_style_radius(vc->ring3, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(vc->ring3, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(vc->ring3, lv_color_hex(0x1A1A3E), 0);
    lv_obj_set_style_border_width(vc->ring3, 1, 0);
    lv_obj_set_style_border_opa(vc->ring3, LV_OPA_30, 0);
    lv_obj_set_style_transform_width(vc->ring3, -RING3_W / 3, 0);
    lv_obj_clear_flag(vc->ring3, LV_OBJ_FLAG_CLICKABLE);

    /* Orb button */
    vc->orb_btn = lv_btn_create(vc->container);
    lv_obj_set_size(vc->orb_btn, ORB_SIZE, ORB_SIZE);
    lv_obj_center(vc->orb_btn);
    lv_obj_set_style_radius(vc->orb_btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(vc->orb_btn, lv_color_hex(TIANJI_COLOR_BG), 0);
    lv_obj_set_style_bg_opa(vc->orb_btn, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(vc->orb_btn, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
    lv_obj_set_style_border_width(vc->orb_btn, 1, 0);
    lv_obj_set_style_border_opa(vc->orb_btn, LV_OPA_50, 0);
    lv_obj_set_style_shadow_color(vc->orb_btn, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
    lv_obj_set_style_shadow_width(vc->orb_btn, 8, 0);
    lv_obj_set_style_shadow_opa(vc->orb_btn, LV_OPA_40, 0);
    lv_obj_set_style_pad_all(vc->orb_btn, 4, 0);

    /* Inner orb */
    vc->orb_inner = lv_obj_create(vc->orb_btn);
    lv_obj_set_size(vc->orb_inner, 14, 14);
    lv_obj_center(vc->orb_inner);
    lv_obj_set_style_radius(vc->orb_inner, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(vc->orb_inner, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
    lv_obj_set_style_bg_opa(vc->orb_inner, LV_OPA_70, 0);
    lv_obj_set_style_border_width(vc->orb_inner, 0, 0);
    lv_obj_set_style_shadow_color(vc->orb_inner, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
    lv_obj_set_style_shadow_width(vc->orb_inner, 6, 0);
    lv_obj_set_style_shadow_opa(vc->orb_inner, LV_OPA_50, 0);
    lv_obj_clear_flag(vc->orb_inner, LV_OBJ_FLAG_CLICKABLE);

    /* Click event */
    lv_obj_add_event_cb(vc->orb_btn, orb_click_cb, LV_EVENT_CLICKED, vc);

    /* Status label - positioned below orb area */
    vc->status_label = lv_label_create(parent);
    lv_label_set_text(vc->status_label, "\xE6\xB0\x94\xE8\x81\x9A\xE7\xA5\x9E\xE5\x87\x9D"); /* 气聚神凝 */
    lv_obj_set_style_text_font(vc->status_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(vc->status_label, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
    lv_obj_set_style_text_letter_space(vc->status_label, 3, 0);
    lv_obj_align_to(vc->status_label, vc->container, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

    /* Hint label */
    vc->hint_label = lv_label_create(parent);
    lv_label_set_text(vc->hint_label, "\xE7\x82\xB9\xE5\x87\xBB\xE7\x81\xB5\xE7\x8F\xA0\xE5\x94\xA4\xE9\x86\x92"); /* 点击灵珠唤醒 */
    lv_obj_set_style_text_font(vc->hint_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(vc->hint_label, lv_color_hex(0x4A4A6A), 0);
    lv_obj_align_to(vc->hint_label, vc->status_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 1);

    /* Animation timer ~30fps */
    vc->anim_timer = lv_timer_create(voice_anim_timer, 33, vc);

    return vc;
}

void tj_voice_destroy(tj_voice_t *vc)
{
    if (!vc) return;
    if (vc->anim_timer) lv_timer_del(vc->anim_timer);
    if (vc->container) lv_obj_del(vc->container);
    /* Labels are in parent, container deletion doesn't affect them */
    if (vc->status_label) lv_obj_del(vc->status_label);
    if (vc->hint_label) lv_obj_del(vc->hint_label);
    lv_mem_free(vc);
}

void tj_voice_set_state(tj_voice_t *vc, tj_voice_state_t state)
{
    if (!vc || vc->state == state) return;
    vc->state = state;
    vc->last_activity = lv_tick_get();

    switch (state) {
        case TJ_VOICE_IDLE:
            lv_obj_set_style_border_color(vc->orb_btn, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
            lv_obj_set_style_bg_color(vc->orb_inner, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
            lv_obj_set_style_shadow_color(vc->orb_btn, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
            lv_label_set_text(vc->hint_label, "\xE7\x82\xB9\xE5\x87\xBB\xE7\x81\xB5\xE7\x8F\xA0\xE5\x94\xA4\xE9\x86\x92");
            break;

        case TJ_VOICE_LISTENING:
            lv_obj_set_style_border_color(vc->orb_btn, lv_color_hex(TIANJI_COLOR_NEON_GREEN), 0);
            lv_obj_set_style_bg_color(vc->orb_inner, lv_color_hex(TIANJI_COLOR_NEON_GREEN), 0);
            lv_obj_set_style_shadow_color(vc->orb_btn, lv_color_hex(TIANJI_COLOR_NEON_GREEN), 0);
            lv_label_set_text(vc->hint_label, "\xE8\xAF\xB7\xE8\xAF\xB4\xE6\x8C\x87\xE4\xBB\xA4..."); /* 请说指令... */
            break;

        case TJ_VOICE_PROCESSING:
            lv_label_set_text(vc->hint_label, "\xE5\xA4\x84\xE7\x90\x86\xE4\xB8\xAD..."); /* 处理中... */
            break;

        case TJ_VOICE_RESPONDING:
            lv_label_set_text(vc->hint_label, "");
            break;
    }
}

tj_voice_state_t tj_voice_get_state(tj_voice_t *vc)
{
    return vc ? vc->state : TJ_VOICE_IDLE;
}

void tj_voice_set_text(tj_voice_t *vc, const char *text)
{
    if (!vc || !text) return;
    lv_label_set_text(vc->status_label, text);
}

void tj_voice_set_callback(tj_voice_t *vc, void (*cb)(void*), void *user)
{
    if (!vc) return;
    vc->on_activate = cb;
    vc->user_data = user;
}

void tj_voice_pulse(tj_voice_t *vc)
{
    if (!vc) return;
    /* One-shot pulse animation */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, vc->orb_btn);
    lv_anim_set_values(&a, 28, 36);
    lv_anim_set_time(&a, 200);
    lv_anim_set_playback_time(&a, 200);
    lv_anim_set_repeat_count(&a, 2);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_width);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);
}
