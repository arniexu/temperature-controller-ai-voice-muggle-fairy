/**
 * @file tianji_data.c
 * @brief Implementation of solar terms, trigrams, wuxing data and helpers
 */

#include "tianji_data.h"
#include <stdio.h>
#include <string.h>

/*=============================
 * SOLAR TERMS (24 JIEQI)
 *=============================*/
const tianji_solar_term_t tianji_solar_terms[] = {
    { "\xE7\xAB\x8B\xE6\x98\xA5", "\xE4\xB8\x80\xE5\x80\x99\xE4\xB8\x9C\xE9\xA3\x8E\xE8\xA7\xA3\xE5\x86\xBB",  2,  3,  "\xE6\x9C\xA8", "\xE9\x9C\x87", "\xE9\x98\xB3" },
    { "\xE9\x9B\xA8\xE6\xB0\xB4", "\xE4\xB8\x80\xE5\x80\x99\xE7\x8D\xAD\xE7\xA5\xAD\xE9\xB1\xBC",  2, 18,  "\xE6\x9C\xA8", "\xE9\x9C\x87", "\xE9\x98\xB3" },
    { "\xE6\x83\x8A\xE8\x9B\xB0", "\xE4\xB8\x80\xE5\x80\x99\xE6\xA1\x83\xE5\xA7\x8B\xE5\x8D\x8E",  3,  5,  "\xE6\x9C\xA8", "\xE9\x9C\x87", "\xE9\x98\xB3" },
    { "\xE6\x98\xA5\xE5\x88\x86", "\xE4\xB8\x80\xE5\x80\x99\xE7\x8E\x84\xE9\xB8\x9F\xE8\x87\xB3",  3, 20,  "\xE6\x9C\xA8", "\xE9\x9C\x87", "\xE5\xB9\xB3" },
    { "\xE6\xB8\x85\xE6\x98\x8E", "\xE4\xB8\x80\xE5\x80\x99\xE6\xA1\x90\xE5\xA7\x8B\xE5\x8D\x8E",  4,  4,  "\xE6\x9C\xA8", "\xE7\xBD\x8B", "\xE9\x98\xB3" },
    { "\xE8\xB0\xB7\xE9\x9B\xA8", "\xE4\xB8\x80\xE5\x80\x99\xE8\x90\x8D\xE5\xA7\x8B\xE7\x94\x9F",  4, 20,  "\xE6\x9C\xA8", "\xE7\xBD\x8B", "\xE9\x98\xB3" },
    { "\xE7\xAB\x8B\xE5\xA4\x8F", "\xE4\xB8\x80\xE5\x80\x99\xE8\x9D\xBC\xE8\x9D\xBC\xE9\xB8\xA3",  5,  5,  "\xE7\x81\xAB", "\xE7\xA6\xBB", "\xE9\x98\xB3" },
    { "\xE5\xB0\x8F\xE6\xBB\xA1", "\xE4\xB8\x80\xE5\x80\x99\xE8\x8B\xA6\xE8\x8F\x9C\xE7\xA7\x80",  5, 21,  "\xE7\x81\xAB", "\xE7\xA6\xBB", "\xE9\x98\xB3" },
    { "\xE8\x8A\x92\xE7\xA7\x8D", "\xE4\xB8\x80\xE5\x80\x99\xE8\x9E\xB3\xE8\x9E\x82\xE7\x94\x9F",  6,  5,  "\xE7\x81\xAB", "\xE7\xA6\xBB", "\xE9\x98\xB3" },
    { "\xE5\xA4\x8F\xE8\x87\xB3", "\xE4\xB8\x80\xE5\x80\x99\xE9\xB9\xBF\xE8\xA7\x92\xE8\xA7\xA3",  6, 21,  "\xE7\x81\xAB", "\xE7\xA6\xBB", "\xE6\x9E\x81\xE9\x98\xB3" },
    { "\xE5\xB0\x8F\xE6\x9A\x91", "\xE4\xB8\x80\xE5\x80\x99\xE6\xB8\xA9\xE9\xA3\x8E\xE8\x87\xB3",  7,  7,  "\xE7\x81\xAB", "\xE7\xA6\xBB", "\xE9\x98\xB3" },
    { "\xE5\xA4\xA7\xE6\x9A\x91", "\xE4\xB8\x80\xE5\x80\x99\xE8\x85\x90\xE8\x8D\x89\xE4\xB8\xBA\xE8\x90\xA4",  7, 22,  "\xE7\x81\xAB", "\xE7\xA6\xBB", "\xE9\x98\xB3" },
    { "\xE7\xAB\x8B\xE7\xA7\x8B", "\xE4\xB8\x80\xE5\x80\x99\xE5\x87\x89\xE9\xA3\x8E\xE8\x87\xB3",  8,  7,  "\xE9\x87\x91", "\xE5\x85\x91", "\xE9\x98\xB4" },
    { "\xE5\xA4\x84\xE6\x9A\x91", "\xE4\xB8\x80\xE5\x80\x99\xE9\xB9\xB0\xE4\xB9\x83\xE7\xA5\xAD\xE9\xB8\x9F",  8, 23,  "\xE9\x87\x91", "\xE5\x85\x91", "\xE9\x98\xB4" },
    { "\xE7\x99\xBD\xE9\x9C\xB2", "\xE4\xB8\x80\xE5\x80\x99\xE9\xB8\xBF\xE9\x9B\x81\xE6\x9D\xA5",  9,  7,  "\xE9\x87\x91", "\xE5\x85\x91", "\xE9\x98\xB4" },
    { "\xE7\xA7\x8B\xE5\x88\x86", "\xE4\xB8\x80\xE5\x80\x99\xE9\x9B\xB7\xE5\xA7\x8B\xE6\x94\xB6\xE5\xA3\xB0",  9, 23,  "\xE9\x87\x91", "\xE4\xB9\xBE", "\xE5\xB9\xB3" },
    { "\xE5\xAF\x92\xE9\x9C\xB2", "\xE4\xB8\x80\xE5\x80\x99\xE9\xB8\xBF\xE9\x9B\x81\xE6\x9D\xA5\xE5\xAE\xBE", 10,  8,  "\xE9\x87\x91", "\xE4\xB9\xBE", "\xE9\x98\xB4" },
    { "\xE9\x9C\x9C\xE9\x99\x8D", "\xE4\xB8\x80\xE5\x80\x99\xE8\xB1\xBA\xE4\xB9\x83\xE7\xA5\xAD\xE5\x85\xBD", 10, 23,  "\xE9\x87\x91", "\xE4\xB9\xBE", "\xE9\x98\xB4" },
    { "\xE7\xAB\x8B\xE5\x86\xAC", "\xE4\xB8\x80\xE5\x80\x99\xE6\xB0\xB4\xE5\xA7\x8B\xE5\x86\xB0", 11,  7,  "\xE6\xB0\xB4", "\xE5\x9D\x8E", "\xE9\x98\xB4" },
    { "\xE5\xB0\x8F\xE9\x9B\xAA", "\xE4\xB8\x80\xE5\x80\x99\xE8\x99\xB9\xE8\x97\x8F\xE4\xB8\x8D\xE8\xA7\x81", 11, 22,  "\xE6\xB0\xB4", "\xE5\x9D\x8E", "\xE9\x98\xB4" },
    { "\xE5\xA4\xA7\xE9\x9B\xAA", "\xE4\xB8\x80\xE5\x80\x99\xE9\xB9\x96\xE9\xB4\xA0\xE4\xB8\x8D\xE9\xB8\xA3", 12,  6,  "\xE6\xB0\xB4", "\xE5\x9D\x8E", "\xE9\x98\xB4" },
    { "\xE5\x86\xAC\xE8\x87\xB3", "\xE4\xB8\x80\xE5\x80\x99\xE8\x9A\xAF\xE8\x9A\xB0\xE7\xBB\x93", 12, 21,  "\xE6\xB0\xB4", "\xE5\x9D\x8E", "\xE6\x9E\x81\xE9\x98\xB4" },
    { "\xE5\xB0\x8F\xE5\xAF\x92", "\xE4\xB8\x80\xE5\x80\x99\xE9\x9B\x81\xE5\x8C\x97\xE4\xB9\xA1",  1,  5,  "\xE6\xB0\xB4", "\xE5\x9D\x8E", "\xE9\x98\xB4" },
    { "\xE5\xA4\xA7\xE5\xAF\x92", "\xE4\xB8\x80\xE5\x80\x99\xE9\xB8\xA1\xE4\xB9\xB3",  1, 20,  "\xE6\xB0\xB4", "\xE5\x9D\x8E", "\xE9\x98\xB4" },
};
const uint8_t tianji_solar_term_count = sizeof(tianji_solar_terms) / sizeof(tianji_solar_terms[0]);

/*=============================
 * CULTIVATION LEVELS (修仙境界)
 *=============================*/
const tianji_cultivation_t tianji_cultivation_levels[] = {
    { "\xE7\x82\xBC\xE6\xB0\x94", 160, 180, TIANJI_COLOR_ICE_BLUE },
    { "\xE7\xAD\x91\xE5\x9F\xBA", 180, 200, 0x5FFF }, /* lighter cyan */
    { "\xE9\x87\x91\xE4\xB8\xB9", 200, 220, TIANJI_COLOR_NEON_GREEN },
    { "\xE5\x85\x83\xE5\xA9\xB4", 220, 240, 0x57E0 }, /* lime */
    { "\xE5\x8C\x96\xE7\xA5\x9E", 240, 260, TIANJI_COLOR_GOLD },
    { "\xE7\x82\xBC\xE8\x99\x9A", 260, 280, TIANJI_COLOR_ORANGE },
    { "\xE5\x90\x88\xE4\xBD\x93", 280, 310, TIANJI_COLOR_FIRE },
};
const uint8_t tianji_cultivation_count = sizeof(tianji_cultivation_levels) / sizeof(tianji_cultivation_levels[0]);

/*=============================
 * WUXING ELEMENTS
 *=============================*/
const tianji_wuxing_t tianji_wuxing_elements[] = {
    { "\xE9\x87\x91", TIANJI_COLOR_GOLD,     0   },
    { "\xE6\x9C\xA8", TIANJI_COLOR_NEON_GREEN, 72  },
    { "\xE6\xB0\xB4", TIANJI_COLOR_ICE_BLUE,  144 },
    { "\xE7\x81\xAB", TIANJI_COLOR_FIRE,     216 },
    { "\xE5\x9C\x9F", 0xA8A0,                   288 }, /* brown-ish */
};
const uint8_t tianji_wuxing_count = sizeof(tianji_wuxing_elements) / sizeof(tianji_wuxing_elements[0]);

/*=============================
 * TRIGRAMS (八卦)
 *=============================*/
const tianji_trigram_t tianji_trigrams[] = {
    { "\xE4\xB9\xBE", "\xE2\x98\xB0", 0   },
    { "\xE5\x85\x91", "\xE2\x98\xB1", 45  },
    { "\xE7\xA6\xBB", "\xE2\x98\xB2", 90  },
    { "\xE9\x9C\x87", "\xE2\x98\xB3", 135 },
    { "\xE7\xBD\x8B", "\xE2\x98\xB4", 180 },
    { "\xE5\x9D\x8E", "\xE2\x98\xB5", 225 },
    { "\xE8\x89\xAE", "\xE2\x98\xB6", 270 },
    { "\xE5\x9D\xA4", "\xE2\x98\xB7", 315 },
};
const uint8_t tianji_trigram_count = sizeof(tianji_trigrams) / sizeof(tianji_trigrams[0]);

/*=============================
 * LOOKUP TABLES for sin/cos
 *=============================*/
/* sin_table[i] = sin(i * 360 / 256) * 256, for i = 0..255 */
static const int16_t sin_table[256] = {
      0,    6,   12,   18,   25,   31,   37,   44,
     50,   56,   62,   68,   74,   80,   86,   92,
     98,  104,  109,  115,  121,  126,  132,  137,
    142,  147,  152,  157,  162,  167,  171,  176,
    180,  184,  188,  192,  196,  199,  203,  206,
    209,  212,  215,  218,  220,  223,  225,  227,
    229,  231,  232,  234,  235,  236,  237,  238,
    239,  239,  240,  240,  240,  240,  239,  239,
    238,  237,  236,  235,  234,  232,  231,  229,
    227,  225,  223,  220,  218,  215,  212,  209,
    206,  203,  199,  196,  192,  188,  184,  180,
    176,  171,  167,  162,  157,  152,  147,  142,
    137,  132,  126,  121,  115,  109,  104,   98,
     92,   86,   80,   74,   68,   62,   56,   50,
     44,   37,   31,   25,   18,   12,    6,    0,
     -6,  -12,  -18,  -25,  -31,  -37,  -44,  -50,
    -56,  -62,  -68,  -74,  -80,  -86,  -92,  -98,
   -104, -109, -115, -121, -126, -132, -137, -142,
   -147, -152, -157, -162, -167, -171, -176, -180,
   -184, -188, -192, -196, -199, -203, -206, -209,
   -212, -215, -218, -220, -223, -225, -227, -229,
   -231, -232, -234, -235, -236, -237, -238, -239,
   -239, -240, -240, -240, -240, -239, -239, -238,
   -237, -236, -235, -234, -232, -231, -229, -227,
   -225, -223, -220, -218, -215, -212, -209, -206,
   -203, -199, -196, -192, -188, -184, -180, -176,
   -171, -167, -162, -157, -152, -147, -142, -137,
   -132, -126, -121, -115, -109, -104,  -98,  -92,
    -86,  -80,  -74,  -68,  -62,  -56,  -50,  -44,
    -37,  -31,  -25,  -18,  -12,   -6,    0
};

/*=============================
 * HELPER IMPLEMENTATIONS
 *=============================*/

const tianji_solar_term_t* tianji_get_current_term(void)
{
    /* TODO: Read RTC date here for production */
    /* For now, return芒种 as default */
    return &tianji_solar_terms[8]; /* 芒种 */
}

const tianji_cultivation_t* tianji_get_cultivation(int16_t temp_tenth)
{
    for (uint8_t i = 0; i < tianji_cultivation_count; i++) {
        if (temp_tenth >= tianji_cultivation_levels[i].minTemp &&
            temp_tenth < tianji_cultivation_levels[i].maxTemp) {
            return &tianji_cultivation_levels[i];
        }
    }
    if (temp_tenth < tianji_cultivation_levels[0].minTemp) {
        return &tianji_cultivation_levels[0];
    }
    return &tianji_cultivation_levels[tianji_cultivation_count - 1];
}

void tianji_format_temp(int16_t temp_tenth, char *buf)
{
    int16_t integer = temp_tenth / 10;
    int16_t frac = temp_tenth % 10;
    if (frac < 0) frac = -frac;
    sprintf(buf, "%d.%d", integer, frac);
}

void tianji_get_lunar_date(char *buf)
{
    /* TODO: Read RTC and convert to lunar/干支 */
    /* Using a static example for now */
    sprintf(buf, "\xE5\xB7\xB1\xE5\xB7\xB3\xE5\xB9\xB4 \xC2\xB7 5\xE6\x9C\x88" "24" "\xE6\x97\xA5");
}

uint16_t tianji_temp_gradient(float percent)
{
    if (percent <= 0.0f) return TIANJI_COLOR_ICE_BLUE;
    if (percent >= 1.0f) return TIANJI_COLOR_GOLD;

    /* Extract RGB components from RGB565 */
    uint8_t r1 = (TIANJI_COLOR_ICE_BLUE >> 11) & 0x1F;
    uint8_t g1 = (TIANJI_COLOR_ICE_BLUE >> 5) & 0x3F;
    uint8_t b1 = TIANJI_COLOR_ICE_BLUE & 0x1F;

    uint8_t r2 = (TIANJI_COLOR_GOLD >> 11) & 0x1F;
    uint8_t g2 = (TIANJI_COLOR_GOLD >> 5) & 0x3F;
    uint8_t b2 = TIANJI_COLOR_GOLD & 0x1F;

    uint8_t r = (uint8_t)(r1 + (r2 - r1) * percent);
    uint8_t g = (uint8_t)(g1 + (g2 - g1) * percent);
    uint8_t b = (uint8_t)(b1 + (b2 - b1) * percent);

    return (uint16_t)((r << 11) | (g << 5) | b);
}

int16_t tianji_sin(int16_t angle)
{
    /* Normalize angle to 0..359 */
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;
    /* Map 0..359 to 0..255 */
    uint8_t idx = (uint8_t)((angle * 256) / 360);
    return sin_table[idx];
}

int16_t tianji_cos(int16_t angle)
{
    return tianji_sin(angle + 90);
}
