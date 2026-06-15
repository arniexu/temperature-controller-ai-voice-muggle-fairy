#ifndef LVGL_FREERTOS_MEM_H
#define LVGL_FREERTOS_MEM_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "FreeRTOS.h"

typedef struct {
    size_t size;
} lvgl_freertos_mem_hdr_t;

static inline void *lvgl_freertos_malloc(size_t size)
{
    lvgl_freertos_mem_hdr_t *hdr;

    if(size == 0U) {
        return NULL;
    }

    hdr = (lvgl_freertos_mem_hdr_t *)pvPortMalloc(sizeof(lvgl_freertos_mem_hdr_t) + size);
    if(hdr == NULL) {
        return NULL;
    }

    hdr->size = size;
    return (void *)(hdr + 1);
}

static inline void lvgl_freertos_free(void *ptr)
{
    lvgl_freertos_mem_hdr_t *hdr;

    if(ptr == NULL) {
        return;
    }

    hdr = ((lvgl_freertos_mem_hdr_t *)ptr) - 1;
    vPortFree((void *)hdr);
}

static inline void *lvgl_freertos_realloc(void *ptr, size_t new_size)
{
    lvgl_freertos_mem_hdr_t *old_hdr;
    void *new_ptr;
    size_t copy_size;

    if(ptr == NULL) {
        return lvgl_freertos_malloc(new_size);
    }

    if(new_size == 0U) {
        lvgl_freertos_free(ptr);
        return NULL;
    }

    old_hdr = ((lvgl_freertos_mem_hdr_t *)ptr) - 1;
    new_ptr = lvgl_freertos_malloc(new_size);
    if(new_ptr == NULL) {
        return NULL;
    }

    copy_size = (old_hdr->size < new_size) ? old_hdr->size : new_size;
    memcpy(new_ptr, ptr, copy_size);
    lvgl_freertos_free(ptr);

    return new_ptr;
}

#endif /* LVGL_FREERTOS_MEM_H */
