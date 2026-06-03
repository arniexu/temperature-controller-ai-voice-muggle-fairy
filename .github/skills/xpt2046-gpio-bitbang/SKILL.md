# xpt2046-gpio-bitbang

Scope: project-constraint

## Purpose
Maintain XPT2046 touch controller in GPIO bit-bang mode for this project.

## Rules
1. Keep `XPT2046_USE_HAL_SPI` disabled unless user explicitly requests migration.
2. Maintain pin mapping:
   - CS: PB0
   - MOSI: PB1
   - MISO: PB2
   - SCK: PA5
   - PEN: PF11
3. Ensure GPIO init config:
   - CS/MOSI/SCK output
   - MISO input
   - PEN input pull-up
4. Keep default output levels safe:
   - CS high, MOSI low, SCK low

## Validation
- Check `Drivers/BSP/Components/xpt2046/xpt2046.h`
- Check `Core/Src/gpio.c` init section
