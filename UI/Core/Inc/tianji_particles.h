/**
 * @file tianji_particles.h
 * @brief Dual Yin-Yang Particle Helix effect for embedded
 * @note  Optimized for STM32F407 with LVGL Canvas
 *        Reduced particle count, lookup-table math
 */

#ifndef TIANJI_PARTICLES_H
#define TIANJI_PARTICLES_H

#include "lvgl.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Maximum number of particles (reduce for smaller flash/RAM)
 */
#define TJ_PARTICLE_COUNT       20

/**
 * @brief Particle structure - 16 bytes each
 */
typedef struct {
    uint16_t    t;          /* Angle accumulator (0-65535 maps to 0-2pi) */
    uint16_t    t_speed;    /* Angular speed per tick */
    uint8_t     radius;     /* Orbit radius in pixels */
    int8_t      y_offset;   /* Vertical offset from center */
    uint8_t     size;       /* Particle radius in pixels */
    uint8_t     alpha;      /* 0-255 transparency */
    int8_t      strand;     /* +1 or -1 for helix direction */
    bool        alive;      /* Active flag */
} tj_particle_t;

/**
 * @brief Particle helix effect controller
 */
typedef struct {
    lv_obj_t       *canvas;         /* LVGL canvas object */
    lv_color_t     *canvas_buf;     /* Canvas framebuffer */
    lv_draw_label_dsc_t draw_dsc;   /* Drawing descriptor */
    tj_particle_t   particles[TJ_PARTICLE_COUNT];
    uint16_t        center_x;
    uint16_t        center_y;
    uint16_t        color1;         /* RGB565 */
    uint16_t        color2;
    uint8_t         speed;          /* Animation speed multiplier */
    uint8_t         spread;         /* Vertical spread in pixels */
    bool            active;
    uint32_t        frame_count;
    lv_timer_t     *timer;          /* Refresh timer */
} tj_particle_helix_t;

/**
 * @brief Create particle helix effect on a parent object
 * @param parent    LVGL parent object (typically a container)
 * @param x         X position
 * @param y         Y position
 * @param w         Canvas width
 * @param h         Canvas height
 * @param color1    First helix color (RGB565)
 * @param color2    Second helix color (RGB565)
 * @return Pointer to helix controller
 */
tj_particle_helix_t* tj_particles_create(lv_obj_t *parent,
                                         lv_coord_t x, lv_coord_t y,
                                         lv_coord_t w, lv_coord_t h,
                                         uint16_t color1, uint16_t color2);

/**
 * @brief Destroy particle effect and free resources
 */
void tj_particles_destroy(tj_particle_helix_t *helix);

/**
 * @brief Set animation speed
 * @param speed 1-10, higher = faster
 */
void tj_particles_set_speed(tj_particle_helix_t *helix, uint8_t speed);

/**
 * @brief Start/stop the animation
 */
void tj_particles_start(tj_particle_helix_t *helix);
void tj_particles_stop(tj_particle_helix_t *helix);

/**
 * @brief Force one frame render (for use when active but timer paused)
 */
void tj_particles_render_frame(tj_particle_helix_t *helix);

#ifdef __cplusplus
}
#endif

#endif /* TIANJI_PARTICLES_H */
