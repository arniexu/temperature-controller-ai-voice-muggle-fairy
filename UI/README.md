# Tianji Thermostat - STM32F407 Edition

## Project Overview

This project ports the PC version of the **Tianji Thermostat** cyber-xianxia temperature controller to the **STM32F407** platform, targeting a **240x320 touch display** and built on the **LVGL v8.3** GUI framework.

## UI Design

Shared UI design link:

- https://miqm4hlcmthn4.ok.kimi.link/?sharetype=link

## Hardware Requirements

| Component | Specification |
|------|------|
| MCU | STM32F407VG (168MHz, 1MB Flash, 192KB SRAM) |
| Display | 240x320 TFT LCD, 16-bit RGB565 |
| Display Controller | ILI9341 or ST7789 (SPI or 8080 interface) |
| Touch Panel | XPT2046 resistive touch (SPI interface) |
| Backlight | PWM-adjustable LED backlight |

## Software Stack

- **STM32CubeF4 HAL** - low-level hardware abstraction
- **FreeRTOS** (optional) - real-time operating system
- **LVGL v8.3** - graphics/UI library
- **GCC ARM Toolchain** or **Keil MDK** / **STM32CubeIDE**

## File Structure

```text
stm32_tianji/
├── Core/
│   ├── Inc/
│   │   ├── tianji_data.h         # Solar terms, five elements, bagua, and cultivation stage data
│   │   ├── tianji_particles.h    # Double-helix particle effect
│   │   ├── tianji_dial.h         # Temperature dial widget
│   │   ├── tianji_voice.h        # AI voice interaction core
│   │   ├── tianji_ui.h           # Main UI assembly
│   │   ├── display_drv.h         # Display driver interface
│   │   └── touch_drv.h           # Touch driver interface
│   └── Src/
│       ├── main.c                # Program entry point
│       ├── tianji_data.c         # Data tables and helper functions
│       ├── tianji_particles.c    # Particle effect implementation (lookup-table sin/cos)
│       ├── tianji_dial.c         # Dial widget (LVGL Arc)
│       ├── tianji_voice.c        # Voice core UI
│       ├── tianji_ui.c           # Main UI assembly
│       ├── display_drv.c         # LVGL display driver integration
│       ├── touch_drv.c           # LVGL touch driver integration
│       └── bsp_stubs.c           # BSP stubs (replace with real drivers)
├── LVGL/
│   └── lv_conf.h                 # LVGL configuration file
└── README.md
```

## Key Design Decisions

### 1. GUI Library Choice: LVGL v8.3

**Why LVGL:**
- Open source and free under the MIT license
- Reasonable resource usage for STM32F407 (~32KB RAM, ~150KB Flash)
- Complete animation, style, and event systems
- Built-in Canvas support, ideal for particle effects
- Mature touch support
- Active community and strong documentation

### 2. Particle Effect Optimization

The PC version uses Canvas 2D + `globalCompositeOperation='lighter'` to create a glowing double-helix effect.

Optimization strategy for STM32:
- **Particle count**: reduced from 100 to 20
- **Math operations**: use a 256-point lookup table instead of `math.h` sin/cos
- **Rendering method**: LVGL Canvas + semi-transparent rectangles (rounded rectangles used to simulate circles)
- **Trail effect**: semi-transparent background overlay (`LV_OPA_30` clear pass)
- **Coordinate calculations**: integer-only math, no floating point

### 3. 240x320 Portrait Layout

```text
+----------------------------+  y=0
| Grain in Ear · Status      |  h=30  Status bar
| Year / Date         28°C   |
+----------------------------+  y=30
|                            |
|      ~ Particle Canvas ~   |
|     +--------------+       |
|     | Temperature  |       |  h=195 Main control area
|     |    24.0°C    |       |
|     | Nascent Soul |       |
|     +--------------+       |
|                            |
+----------------------------+  y=225
| Qi Refine / Foundation...  |  h=22  Cultivation stage indicator
+----------------------------+  y=247
|     [Spirit Orb] Voice     |  h=35  Voice core
+----------------------------+  y=282
| [Mode] [Wind] [Timer]      |  h=38  Bottom controls
+----------------------------+  y=320
| Solar term marquee ...     |       Solar-term ticker
+----------------------------+
```

### 4. Touch Interaction

- **Dial drag**: swipe right/left to raise temperature, swipe down/up to lower temperature
  - Sensitivity: `x*0.3 - y*0.2` mapped to temperature change
  - Damping: physical snap/adsorption feel achieved through incremental calculation
- **Button tap**: mode / fan speed / timer buttons
- **Spirit orb wake-up**: tap the voice core button to activate voice input

## Quick Start

### Step 1: Prepare the Development Environment

1. Install **STM32CubeIDE** or **Keil MDK**
2. Download **LVGL v8.3** source code: `git clone -b release/v8.3 https://github.com/lvgl/lvgl.git`
3. Create an F407 project with **STM32CubeMX** and configure:
   - External 8MHz HSE crystal
   - SYSCLK 168MHz
   - SPI1 (LCD + touch)
   - TIM3 (backlight PWM)
   - FreeRTOS (optional)

### Step 2: Add Files to the Project

1. Add `stm32_tianji/Core/Inc/*.h` to the include path
2. Add `stm32_tianji/Core/Src/*.c` to the build sources
3. Place `lv_conf.h` in the LVGL root directory or include path
4. Make sure LVGL's `lvgl.h` is in the include path

### Step 3: Implement BSP Drivers (Critical)

Edit `bsp_stubs.c` and replace the stubs with real hardware functions:

```c
// Required functions:
void lcd_init(void);                    // Initialize the LCD controller
void lcd_set_window(x1,y1,x2,y2);      // Set drawing window
void lcd_write_pixels(data,count);     // Write pixel buffer (DMA recommended)
void lcd_write_pixel(color);           // Write a single pixel
void lcd_set_backlight_hw(percent);    // PWM backlight control
void lcd_enter_sleep(void);            // Enter sleep mode
void lcd_exit_sleep(void);             // Exit sleep mode

void touch_init_hw(void);              // Initialize touch controller
uint8_t touch_is_pressed(void);        // Check whether the panel is pressed
void touch_read_coords(x,y);           // Read touch coordinates
```

### Step 4: Build and Flash

```bash
# With STM32CubeIDE: import the project -> build -> debug
# Or with a Makefile:
make clean
make -j$(nproc)
st-flash write build/tianji.bin 0x8000000
```

## Key API Usage

### Initialize the UI
```c
#include "tianji_ui.h"

tianji_ui_t *ui = tianji_ui_init();
```

### Set the Temperature Externally (for example, from a voice command)
```c
tianji_ui_set_temp(ui, 260);  // 26.0°C
```

### Handle Voice Recognition Results
```c
tianji_ui_voice_result(ui, "Increase temperature", 280);  // 28°C
```

### Main Loop
```c
while (1) {
    lv_timer_handler();  // LVGL task handling, about every 5 ms
    HAL_Delay(5);
}
```

## Performance Optimization Suggestions

1. **SPI speed**: set the SPI baud rate to PCLK/2 (up to 42MHz for SPI1 on APB2)
2. **DMA transfer**: use SPI + DMA for pixel transfer, and call `lv_disp_flush_ready()` in the DMA transfer-complete interrupt
3. **Particle frame rate**: if rendering stutters, reduce `TJ_PARTICLE_COUNT` (20 -> 12)
4. **LVGL refresh period**: tune `LV_DISP_DEF_REFR_PERIOD` in `lv_conf.h` (default 16ms = 60 FPS)
5. **Flash usage**: LVGL default fonts are large; consider keeping only montserrat 8, 10, 12, 14, 16, and 20
6. **SRAM layout**: define a `.sram` section in the linker script for DMA buffers

## Estimated Memory Usage

| Component | Flash | RAM |
|------|-------|-----|
| LVGL core | ~80KB | ~32KB |
| Fonts | ~60KB | - |
| Particle canvas (170x170x2) | - | ~58KB |
| UI code + data | ~30KB | ~2KB |
| DMA buffers (2x) | - | ~150KB (in SRAM) |
| **Total** | **~170KB** | **~32KB + canvas** |

## Advanced Customization

### Add Custom Fonts
Use the LVGL font converter: https://lvgl.io/tools/fontconverter
- Convert the preferred display font for solar-term labels and themed text
- Include only the characters you actually need to reduce memory usage

### Add a Voice Module
- Connect an LD3320 or SU-03T voice module over UART
- Send the wake-up command inside the `voice_activated()` callback
- Call `tianji_ui_voice_result()` after receiving recognition results

### Add Wi-Fi / Bluetooth
- Use an ESP8266 or ESP32 as a coprocessor
- Retrieve weather data over UART
- Update outdoor temperature and AQI in the status bar

## License

MIT License, consistent with LVGL.
