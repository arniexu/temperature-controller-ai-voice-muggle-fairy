#ifndef __CFONT_H
#define __CFONT_H

#include <stdint.h>

/*
 * Compatibility ASCII font tables for legacy devboard LCD reference code.
 * The current project does not provide the original cfont package, so these
 * zero-initialized tables keep compilation/linking stable.
 */
static const uint8_t char_1206[95][12] = {0};
static const uint8_t char_1608[95][16] = {0};
static const uint8_t char_2412[95][48] = {0};

#endif /* __CFONT_H */
