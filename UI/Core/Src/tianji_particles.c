/**
 * @file tianji_particles.c
 * @brief Dual Yin-Yang Particle Helix implementation
 * @note  Optimized for 240x320 STM32F407
 *        - Uses LVGL Canvas for rendering
 *        - Fixed-point arithmetic (no float in ISR)
 *        - Reduced particle count for performance
 */

#include "tianji_particles.h"
#include "tianji_data.h"
#include <stdlib.h>
#include <string.h>

/*=================================================================*/
/* Internal: Fast fixed-point sin/cos using lookup table           */
/*=================================================================*/

/* sin256[i] = sin(i * 2pi / 256) * 128, i = 0..255 */
static const int8_t sin256[256] = {
      0,    3,    6,    9,   12,   16,   19,   22,
     25,   28,   31,   34,   37,   40,   43,   46,
     49,   51,   54,   57,   60,   62,   65,   67,
     70,   72,   75,   77,   79,   81,   83,   85,
     87,   89,   91,   93,   95,   96,   98,   99,
    101,  102,  103,  104,  105,  106,  107,  108,
    109,  109,  110,  110,  111,  111,  111,  111,
    112,  111,  111,  111,  111,  110,  110,  109,
    109,  108,  107,  106,  105,  104,  103,  102,
    101,   99,   98,   96,   95,   93,   91,   89,
     87,   85,   83,   81,   79,   77,   75,   72,
     70,   67,   65,   62,   60,   57,   54,   51,
     49,   46,   43,   40,   37,   34,   31,   28,
     25,   22,   19,   16,   12,    9,    6,    3,
      0,   -3,   -6,   -9,  -12,  -16,  -19,  -22,
    -25,  -28,  -31,  -34,  -37,  -40,  -43,  -46,
    -49,  -51,  -54,  -57,  -60,  -62,  -65,  -67,
    -70,  -72,  -75,  -77,  -79,  -81,  -83,  -85,
    -87,  -89,  -91,  -93,  -95,  -96,  -98,  -99,
   -101, -102, -103, -104, -105, -106, -107, -108,
   -109, -109, -110, -110, -111, -111, -111, -111,
   -112, -111, -111, -111, -111, -110, -110, -109,
   -109, -108, -107, -106, -105, -104, -103, -102,
   -101,  -99,  -98,  -96,  -95,  -93,  -91,  -89,
    -87,  -85,  -83,  -81,  -79,  -77,  -75,  -72,
    -70,  -67,  -65,  -62,  -60,  -57,  -54,  -51,
    -49,  -46,  -43,  -40,  -37,  -34,  -31,  -28,
    -25,  -22,  -19,  -16,  -12,   -9,   -6,   -3
};

static inline int16_t fast_sin(uint16_t angle_q8)
{
    uint8_t idx = (uint8_t)(angle_q8 >> 8);
    return (int16_t)sin256[idx];
}

static inline int16_t fast_cos(uint16_t angle_q8)
{
    return fast_sin(angle_q8 + (uint16_t)(64 << 8));
}

/*=================================================================*/
/* Internal: Draw a single glowing particle                        */
/*=================================================================*/

static void draw_particle(tj_particle_helix_t *h, int16_t x, int16_t y,
                          uint8_t size, uint8_t alpha, uint16_t color565)
{
    /* Convert RGB565 to lv_color_t */
    lv_color_t c;
    c.full = color565;

    /* Simple filled circle with alpha blending */
    /* For performance on STM32, use LVGL's rectangle with radius */
    lv_area_t area;
    area.x1 = x - size;
    area.y1 = y - size;
    area.x2 = x + size;
    area.y2 = y + size;

    if (area.x1 < 0) area.x1 = 0;
    if (area.y1 < 0) area.y1 = 0;
    if (area.x2 >= lv_obj_get_width(h->canvas)) area.x2 = lv_obj_get_width(h->canvas) - 1;
    if (area.y2 >= lv_obj_get_height(h->canvas)) area.y2 = lv_obj_get_height(h->canvas) - 1;

    /* Use LVGL draw function */
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = c;
    rect_dsc.bg_opa = (lv_opa_t)((alpha * LV_OPA_COVER) / 255);
    rect_dsc.radius = size;

    lv_canvas_draw_rect(h->canvas, area.x1, area.y1,
                        area.x2 - area.x1 + 1, area.y2 - area.y1 + 1,
                        &rect_dsc);
}

/*=================================================================*/
/* Internal: Animation timer callback                              */
/*=================================================================*/

static void particle_timer_cb(lv_timer_t *timer)
{
    tj_particle_helix_t *h = (tj_particle_helix_t *)timer->user_data;
    if (!h || !h->active) return;

    lv_coord_t cw = lv_obj_get_width(h->canvas);
    lv_coord_t ch = lv_obj_get_height(h->canvas);

    /* Clear canvas with semi-transparent background (trail effect) */
    lv_canvas_fill_bg(h->canvas, lv_color_hex(TIANJI_COLOR_BG), LV_OPA_30);

    /* Update and draw each particle */
    for (uint8_t i = 0; i < TJ_PARTICLE_COUNT; i++) {
        tj_particle_t *p = &h->particles[i];
        if (!p->alive) continue;

        /* Update angle: t += speed */
        uint16_t speed_q = (uint16_t)(p->t_speed * h->speed) >> 4;
        if (speed_q < 1) speed_q = 1;
        p->t += speed_q;

        /* Calculate position using fixed-point sin/cos */
        /* x = cx + cos(t * strand) * radius * 0.5 */
        /* y = cy + sin(t) * radius */
        int16_t s = fast_sin(p->t);
        int16_t c = fast_cos((uint16_t)((int16_t)p->t * p->strand));

        int16_t px = h->center_x + (int16_t)((c * p->radius) >> 8);
        int16_t py = h->center_y + (int16_t)((s * p->radius) >> 7) + p->y_offset;

        /* Alpha oscillation */
        uint8_t a = (uint8_t)((p->alpha * (128 + (s >> 1))) >> 8);
        if (a < 30) a = 30;

        /* Draw particle */
        uint16_t color = (p->strand > 0) ? h->color1 : h->color2;
        draw_particle(h, px, py, p->size, a, color);
    }

    /* Darken overlay for trail effect */
    lv_area_t full;
    full.x1 = 0; full.y1 = 0;
    full.x2 = cw - 1; full.y2 = ch - 1;

    lv_draw_rect_dsc_t overlay;
    lv_draw_rect_dsc_init(&overlay);
    overlay.bg_color = lv_color_hex(TIANJI_COLOR_BG);
    overlay.bg_opa = LV_OPA_20;
    lv_canvas_draw_rect(h->canvas, 0, 0, cw, ch, &overlay);

    h->frame_count++;
}

/*=================================================================*/
/* Public API                                                      */
/*=================================================================*/

tj_particle_helix_t* tj_particles_create(lv_obj_t *parent,
                                         lv_coord_t x, lv_coord_t y,
                                         lv_coord_t w, lv_coord_t h,
                                         uint16_t color1, uint16_t color2)
{
    tj_particle_helix_t *helix = lv_mem_alloc(sizeof(tj_particle_helix_t));
    if (!helix) return NULL;
    memset(helix, 0, sizeof(tj_particle_helix_t));

    /* Allocate canvas buffer */
    uint32_t buf_size = w * h * sizeof(lv_color_t);
    helix->canvas_buf = lv_mem_alloc(buf_size);
    if (!helix->canvas_buf) {
        lv_mem_free(helix);
        return NULL;
    }
    memset(helix->canvas_buf, 0, buf_size);

    /* Create LVGL canvas */
    helix->canvas = lv_canvas_create(parent);
    lv_canvas_set_buffer(helix->canvas, helix->canvas_buf, w, h, LV_IMG_CF_TRUE_COLOR);
    lv_obj_set_pos(helix->canvas, x, y);
    lv_obj_set_size(helix->canvas, w, h);

    /* Fill with background color */
    lv_canvas_fill_bg(helix->canvas, lv_color_hex(TIANJI_COLOR_BG), LV_OPA_COVER);

    /* Setup parameters */
    helix->center_x = w / 2;
    helix->center_y = h / 2;
    helix->color1 = color1;
    helix->color2 = color2;
    helix->speed = 4;
    helix->spread = 40;
    helix->active = false;

    /* Initialize particles */
    for (uint8_t i = 0; i < TJ_PARTICLE_COUNT; i++) {
        tj_particle_t *p = &helix->particles[i];
        p->t = (uint16_t)(rand() % 65536);
        p->t_speed = 10 + (rand() % 30);
        p->radius = 20 + (rand() % 50);
        p->y_offset = (int8_t)((rand() % (helix->spread * 2)) - helix->spread);
        p->size = 1 + (rand() % 3);
        p->alpha = 60 + (rand() % 150);
        p->strand = (rand() % 2) ? 1 : -1;
        p->alive = true;
    }

    /* Create refresh timer (30 FPS) */
    helix->timer = lv_timer_create(particle_timer_cb, 33, helix);
    lv_timer_pause(helix->timer);

    return helix;
}

void tj_particles_destroy(tj_particle_helix_t *helix)
{
    if (!helix) return;
    if (helix->timer) {
        lv_timer_del(helix->timer);
    }
    if (helix->canvas) {
        lv_obj_del(helix->canvas);
    }
    if (helix->canvas_buf) {
        lv_mem_free(helix->canvas_buf);
    }
    lv_mem_free(helix);
}

void tj_particles_set_speed(tj_particle_helix_t *helix, uint8_t speed)
{
    if (!helix) return;
    if (speed < 1) speed = 1;
    if (speed > 10) speed = 10;
    helix->speed = speed;
}

void tj_particles_start(tj_particle_helix_t *helix)
{
    if (!helix) return;
    helix->active = true;
    lv_timer_resume(helix->timer);
}

void tj_particles_stop(tj_particle_helix_t *helix)
{
    if (!helix) return;
    helix->active = false;
    lv_timer_pause(helix->timer);
}

void tj_particles_render_frame(tj_particle_helix_t *helix)
{
    if (!helix || !helix->active) return;
    /* Manually trigger one frame */
    particle_timer_cb(helix->timer);
}
