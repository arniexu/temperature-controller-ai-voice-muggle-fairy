# 天机恒温器 · Tianji Thermostat — STM32F407 Edition

A **cyber-xianxia** themed AI-voice temperature controller ported to the **STM32F407** platform.  
Combines cultivation-world aesthetics (solar terms, five elements, bagua, realm progression) with practical smart-home thermostat functionality, featuring an AI voice assistant and an animated double-helix particle UI.

## UI Design

Interactive prototype:  
👉 https://miqm4hlcmthn4.ok.kimi.link/?sharetype=link

### Screen Layout (240 × 320 portrait)

```text
+----------------------------+  y=0
| Grain in Ear · Status      |  h=30   Status bar — solar term, date, current temp
| Year / Date         28°C   |
+----------------------------+  y=30
|                            |
|    ~ Particle Canvas ~     |
|   +--------------+         |
|   | Temperature  |         |  h=195  Main control area
|   |   24.0°C     |         |         – Animated double-helix particles
|   | Nascent Soul |         |         – LVGL Arc temperature dial
|   +--------------+         |         – Cultivation-realm label
|                            |
+----------------------------+  y=225
| Qi Refine / Foundation...  |  h=22   Realm-progression indicator
+----------------------------+  y=247
|    [Spirit Orb]  Voice     |  h=35   AI voice-core button
+----------------------------+  y=282
| [Mode]  [Wind]  [Timer]    |  h=38   Bottom controls
+----------------------------+  y=320
| Solar term marquee ...     |         Scrolling solar-term ticker
+----------------------------+
```

### Key UI Features

| Feature | Description |
|---------|-------------|
| **Double-helix particles** | 20-particle animated canvas with lookup-table sin/cos — no floating point |
| **Temperature dial** | Drag gesture on the LVGL Arc widget; sensitivity `x×0.3 − y×0.2` |
| **Cultivation realms** | Temperature ranges map to Qi Refining → Foundation → Core Formation → Nascent Soul … |
| **Solar-term status bar** | Displays the current of 24 solar terms, date and real-time temperature |
| **AI voice core** | Tap the Spirit Orb to activate; results update dial & realm label via `tianji_ui_voice_result()` |

## Hardware

| Component | Specification |
|-----------|---------------|
| MCU | STM32F407VG — 168 MHz, 1 MB Flash, 192 KB SRAM |
| Display | 240×320 TFT LCD, 16-bit RGB565 (ILI9341 / ST7789) |
| Touch | XPT2046 resistive touch, SPI |
| Backlight | PWM-adjustable LED |

## Software Stack

- **STM32CubeF4 HAL** — hardware abstraction layer
- **LVGL v8.3** — graphics & UI framework
- **FreeRTOS** (optional) — real-time OS
- **GCC ARM Toolchain** or **Keil MDK** / **STM32CubeIDE**

## Full Documentation

See **[UI/README.md](UI/README.md)** for the complete guide: file structure, BSP driver integration, build instructions, performance tuning, and memory estimates.
