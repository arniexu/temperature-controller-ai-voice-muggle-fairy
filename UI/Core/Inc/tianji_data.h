/**
 * @file tianji_data.h
 * @brief Solar terms, trigrams, wuxing, and cultivation data
 * @note  All constants and lookup tables for the Tianji Thermostat UI
 */

#ifndef TIANJI_DATA_H
#define TIANJI_DATA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============================
 * SCREEN DIMENSIONS (240x320)
 *=============================*/
#define TIANJI_SCREEN_W         240
#define TIANJI_SCREEN_H         320

#define TIANJI_STATUS_H         30      /* Top status bar height */
#define TIANJI_DIAL_Y           30      /* Dial area start */
#define TIANJI_DIAL_H           195     /* Dial area height */
#define TIANJI_REALM_Y          225     /* Cultivation realm text */
#define TIANJI_REALM_H          22
#define TIANJI_VOICE_Y          247     /* Voice core area */
#define TIANJI_VOICE_H          35
#define TIANJI_BOTTOM_Y         282     /* Bottom controls */
#define TIANJI_BOTTOM_H         38

/*=============================
 * COLOR PALETTE (RGB565)
 *=============================*/
#define TIANJI_COLOR_BG         0x080C    /* #05050A -> RGB565 */
#define TIANJI_COLOR_ICE_BLUE   0x9EBF    /* #A3D5FF -> RGB565 */
#define TIANJI_COLOR_NEON_GREEN 0x07E0    /* #39FF14 -> RGB565 */
#define TIANJI_COLOR_MYSTIC_PURPLE  0x7195 /* #7B2D8E -> RGB565 */
#define TIANJI_COLOR_WHITE      0xFFFF
#define TIANJI_COLOR_GRAY       0x7BEF    /* ~50% gray */
#define TIANJI_COLOR_DARK_GRAY  0x39E7    /* ~25% gray */
#define TIANJI_COLOR_GOLD       0xFE00    /* #FFD700 */
#define TIANJI_COLOR_FIRE       0xFA20    /* #FF4500 */
#define TIANJI_COLOR_ORANGE     0xFD20    /* #FF8C00 */
#define TIANJI_COLOR_CYAN       0x07FF

#define TIANJI_COLOR_PANEL_BG   0x1082    /* rgba(10,10,18,0.85) approx */
#define TIANJI_COLOR_GLOW_GREEN 0x07E0
#define TIANJI_COLOR_GLOW_BLUE  0x9EBF

/*=============================
 * TEMPERATURE RANGE
 *=============================*/
#define TIANJI_TEMP_MIN         160       /* 16.0 C, in tenths */
#define TIANJI_TEMP_MAX         300       /* 30.0 C, in tenths */
#define TIANJI_TEMP_DEFAULT     240       /* 24.0 C */

/*=============================
 * SOLAR TERM STRUCTURE
 *=============================*/
typedef struct {
    const char *name;       /* e.g. "芒种" */
    const char *sub;        /* e.g. "一候螳螂生" */
    uint8_t     startMonth;
    uint8_t     startDay;
    const char *element;    /* 金木水火土 */
    const char *trigram;    /* 乾坤震巽坎离艮兑 */
    const char *yinyang;    /* 阴阳平 */
} tianji_solar_term_t;

/*=============================
 * CULTIVATION LEVEL
 *=============================*/
typedef struct {
    const char *name;       /* e.g. "金丹" */
    int16_t     minTemp;    /* in tenths of degree */
    int16_t     maxTemp;
    uint16_t    color;      /* RGB565 color */
} tianji_cultivation_t;

/*=============================
 * WUXING ELEMENT
 *=============================*/
typedef struct {
    const char *name;
    uint16_t    color;
    uint16_t    angle;      /* degrees on dial */
} tianji_wuxing_t;

/*=============================
 * TRIGRAM
 *=============================*/
typedef struct {
    const char *name;
    const char *symbol;
    uint16_t    angle;
} tianji_trigram_t;

/*=============================
 * GLOBAL DATA TABLES
 *=============================*/
extern const tianji_solar_term_t  tianji_solar_terms[];
extern const uint8_t              tianji_solar_term_count;

extern const tianji_cultivation_t tianji_cultivation_levels[];
extern const uint8_t              tianji_cultivation_count;

extern const tianji_wuxing_t      tianji_wuxing_elements[];
extern const uint8_t              tianji_wuxing_count;

extern const tianji_trigram_t     tianji_trigrams[];
extern const uint8_t              tianji_trigram_count;

/*=============================
 * HELPER FUNCTIONS
 *=============================*/

/**
 * @brief Get current solar term based on system date
 * @return Pointer to current solar term
 * @note  Uses RTC date if available, otherwise returns a default
 */
const tianji_solar_term_t* tianji_get_current_term(void);

/**
 * @brief Get cultivation level for a given temperature
 * @param temp_tenth  Temperature in tenths of degree (e.g. 240 = 24.0 C)
 * @return Pointer to cultivation level struct
 */
const tianji_cultivation_t* tianji_get_cultivation(int16_t temp_tenth);

/**
 * @brief Format temperature string: "24.5"
 * @param temp_tenth  Temperature in tenths
 * @param buf         Output buffer (min 6 bytes)
 */
void tianji_format_temp(int16_t temp_tenth, char *buf);

/**
 * @brief Get lunar/干支 date string
 * @param buf  Output buffer (min 32 bytes)
 */
void tianji_get_lunar_date(char *buf);

/**
 * @brief Get color gradient between cold (blue) and warm (gold)
 * @param percent  0.0=cold, 1.0=warm
 * @return RGB565 color
 */
uint16_t tianji_temp_gradient(float percent);

/**
 * @brief Quick sin approximation (Q15 fixed-point like)
 * @param angle  Angle in degrees (0-360)
 * @return sin value scaled by 256 (-256 to 256)
 */
int16_t tianji_sin(int16_t angle);

/**
 * @brief Quick cos approximation
 * @param angle  Angle in degrees (0-360)
 * @return cos value scaled by 256 (-256 to 256)
 */
int16_t tianji_cos(int16_t angle);

#ifdef __cplusplus
}
#endif

#endif /* TIANJI_DATA_H */
