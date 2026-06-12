# 天机恒温器 · Tianji Thermostat — STM32F407 版

一个**赛博仙侠**主题的 AI 语音温控器，移植到 **STM32F407** 平台。  
将修仙世界观（二十四节气、五行八卦、境界晋升）与实用智能温控功能融为一体，配有 AI 语音助手和动态双螺旋粒子特效 UI。

---

## 目录

- [项目背景](#项目背景)
- [开发故事](#开发故事)
- [硬件环境](#硬件环境)
- [软件环境](#软件环境)
- [目录结构](#目录结构)
- [软件架构](#软件架构)
- [UI 设计](#ui-设计)
- [快速开始](#快速开始)
- [常见问题](#常见问题)
- [预编译固件](#预编译固件)
- [完整文档](#完整文档)
- [许可证](#许可证)

---

## 项目背景

### 为什么要做这个温控器？

市面上的温控器面板千篇一律——冷冰冰的工业风、单调的数字显示。我在想：**如果一个智能温控器不再像无聊的工业面板，而是融入中国传统宇宙观——二十四节气、五行、八卦、修仙境界，会是什么样子？**

于是就有了这个"赛博仙侠"温控器。你可以：

- 🗣️ **用语音和它对话**，查询节气、用声音设定温度
- 🌡️ **拖动温度旋钮**，直观调节目标温度
- ✨ **看动态双螺旋粒子动画**——温度变化时，粒子会随之律动
- 🧘 **感受境界晋升**——温度范围映射到修仙境界：炼气 → 筑基 → 金丹 → 元婴……

### 这个项目适合谁？

| 受众 | 你能从中学到什么 |
|------|-----------------|
| **嵌入式初学者（< 1 年经验）** | 完整的 STM32 项目结构、LVGL 图形框架入门、FSMC 驱动 LCD 的实战、FreeRTOS 任务设计 |
| **中级开发者（1-3 年）** | 五层分层架构设计、BSP 抽象与硬件解耦、LVGL 自定义 Widget 开发、粒子特效优化技巧 |
| **嵌入式老手（3 年+）** | 可直接复用 BSP/驱动层代码、参考架构设计思路、快速移植到自己的硬件 |

> **我本人也是第一次使用本项目中的几乎所有组件**（F407、FreeRTOS、LVGL、FSMC），所以这个仓库的组织方式就是"我希望当初有人这样教我"的方式。如果你是初学者，不要怕——往下看，一步步来。

---

## 开发故事

这个项目最初在 STM32F103 上实验，但 UI（240×320、动画、粒子）太重了。于是换到了一块**启明欣欣 STM32F407 开发板（高配版 V6.1）**——它集成了本项目需要的所有硬件：LCD、触摸、SD 卡槽、充足的 RAM/Flash。

**这是我第一次使用以下所有技术：**
- 第一次使用 **STM32F407**（此前只有 F103 经验）
- 第一次在实际项目中使用 **FreeRTOS**
- 第一次使用 **LVGL** 做 GUI
- 第一次使用 **FSMC** 驱动 LCD

尽管全是新东西，利用业余时间（晚上和周末）大约 3 周就让系统跑起来了。

**开发方式：**
- 大量初始代码（工具链迁移、驱动桩、LVGL 集成）由 AI 生成——我专注于调试、集成和架构设计
- 同时支持 ARM Compiler (AC6) 和 Clang 两种工具链
- 五层软件架构是代码跑通之后，我自己设计并重构的

---

## 硬件环境

### 开发板

| 项目 | 型号 |
|------|------|
| **开发板** | 启明欣欣 STM32F407 开发板（高配版 V6.1） |
| **主控芯片** | STM32F407ZGT6（LQFP144，168 MHz，1 MB Flash，192 KB SRAM） |

> **没有这块开发板？** 不用急。本项目的 BSP 层通过 `bsp_stubs.c` 做了抽象，你只需要替换这一个文件就能适配你自己的硬件。详见 [UI/README.md](UI/README.md) 中的移植指南。

### 本项目使用的外设

| 外设模块 | 型号/规格 | 连接方式 | 用途 |
|----------|----------|----------|------|
| **TFT LCD** | 240×320 RGB565 | FSMC 8080 并口 | 主显示屏 |
| **LCD 控制器** | ILI9341 | FSMC（NE4 片选，A12=RS） | 显示控制 |
| **触摸屏** | XPT2046 电阻式 | SPI1 | 触摸输入 |
| **LCD 背光** | PWM 可调 LED | PF10 (TIM) | 屏幕亮度 |
| **外部 SRAM** | IS62WV51216 (1 MB) | FSMC | LVGL 帧缓冲 / DMA 缓冲 |
| **串口** | UART1 (PA9/PA10) | MAX232 | 调试日志 / 语音模块通信 |

> 完整管脚映射请参考 [pin mapping.md](pin mapping.md)

---

## 软件环境

### 必备工具链（二选一）

#### 方式一：Keil MDK（推荐，适合初学者）

| 软件 | 版本要求 |
|------|---------|
| **Keil MDK** | MDK5（uVision 5） |
| **ARM Compiler** | ARMCLANG V6.24（AC6） |
| **器件包** | Keil.STM32F4xx_DFP.2.17.1 或更高 |
| **烧录器** | ST-Link / J-Link / DAP-Link |

> 工程文件位于 `MDK-ARM/ai_tempconroler.uvprojx`，双击即可打开。

#### 方式二：CMake + GCC（适合进阶用户）

| 软件 | 版本要求 |
|------|---------|
| **ARM GCC** | arm-none-eabi-gcc 10.3+ |
| **CMake** | 3.20+ |
| **烧录工具** | OpenOCD / ST-Flash |

```bash
# 在仓库根目录执行
cmake -B build -G "Ninja"
cmake --build build
```

### 依赖库

| 库 | 版本 | 用途 |
|----|------|------|
| **LVGL** | v8.3 | 图形界面框架 |
| **FreeRTOS** | CMSIS-RTOS v1 封装 | 实时操作系统 |
| **FatFs** | R0.12c | FAT 文件系统（SD 卡） |
| **LibJPEG** | - | JPEG 图片解码 |
| **STM32CubeF4 HAL** | - | 硬件抽象层 |

> LVGL 以 git submodule 形式引入，仓库 clone 后需执行 `git submodule update --init`。

---

## 目录结构

```
ai_tempconroler/
├── Core/                            # 应用层核心代码
│   ├── Inc/                         # 头文件（main.h, freertos.h 等）
│   │   ├── main.h                   # 主程序头文件
│   │   ├── freertos.h               # FreeRTOS 配置
│   │   └── stm32f4xx_hal_conf.h     # HAL 库配置
│   └── Src/                         # 源文件（CubeMX 自动生成）
│       ├── main.c                   # ★ 程序入口，初始化与启动调度器
│       ├── freertos.c               # ★ FreeRTOS 任务定义
│       ├── fsmc.c                   # FSMC 总线配置（LCD + SRAM）
│       └── gpio.c                   # GPIO 配置
│
├── Drivers/                         # 驱动层
│   ├── BSP/                         # 板级支持包（BSP）
│   │   └── Components/              # 外设驱动
│   │       ├── ili9341/             # ILI9341 LCD 控制器驱动
│   │       └── xpt2046/             # XPT2046 触摸控制器驱动
│   ├── STM32F4xx_HAL_Driver/        # STM32 HAL 库
│   └── CMSIS/                       # ARM CMSIS 核心文件
│
├── UI/                              # 用户界面层
│   ├── Core/
│   │   ├── Inc/                     # UI 头文件
│   │   │   ├── tianji_ui.h          # ★ 主 UI 组装接口
│   │   │   ├── tianji_data.h        # 节气/五行/八卦/境界数据
│   │   │   ├── tianji_particles.h   # 双螺旋粒子特效
│   │   │   ├── tianji_dial.h        # 温度旋钮控件
│   │   │   ├── tianji_voice.h       # AI 语音核心 UI
│   │   │   ├── display_drv.h        # LVGL 显示驱动接口
│   │   │   └── touch_drv.h          # LVGL 触摸驱动接口
│   │   └── Src/                     # UI 源文件
│   │       ├── tianji_ui.c          # 组装所有控件到一个屏幕
│   │       ├── tianji_data.c        # 数据表与辅助函数
│   │       ├── tianji_particles.c   # 粒子特效（查表法 sin/cos）
│   │       ├── tianji_dial.c        # 旋钮实现（LVGL Arc）
│   │       ├── tianji_voice.c       # 语音核心 UI
│   │       ├── display_drv.c        # LVGL 显示驱动注册
│   │       ├── touch_drv.c          # LVGL 触摸驱动注册
│   │       └── bsp_stubs.c          # ★ BSP 桩函数（替换此文件适配新硬件）
│   ├── LVGL/                        # LVGL 配置文件
│   │   └── lv_conf.h                # LVGL 配置（分辨率、缓冲、字体等）
│   └── README.md                    # UI 层完整文档
│
├── Middlewares/                     # 中间件
│   └── Third_Party/
│       └── lvgl/                    # LVGL 源码（git submodule）
│
├── Tests/                           # 测试代码
│   ├── BSP/                         # BSP 自测（LCD/触摸冒烟测试）
│   └── LVGL/                        # LVGL 功能测试
│
├── MDK-ARM/                         # Keil MDK 工程文件
│   ├── ai_tempconroler.uvprojx      # ★ Keil 工程（双击打开）
│   └── ai_tempconroler.uvoptx       # 工程选项
│
├── FATFS/                           # FatFs 文件系统
├── LIBJPEG/                         # LibJPEG 解码库
├── Utilities/                       # 工具类
│   ├── Fonts/                       # 自定义字体
│   └── Log/                         # 日志模块
│
├── pin mapping.md                   # ★ 完整管脚映射表
├── ai_tempconroler.ioc              # CubeMX 工程文件
├── CMakeLists.txt                   # CMake 构建配置（GCC 工具链）
└── README.md                        # ★ 本文件
```

> 标记 ★ 的是你可能最先需要打开或修改的文件。

---

## 软件架构

### 五层分层设计

代码遵循严格的五层设计。每一层**只依赖它下面的一层**，这使得硬件更换和单元测试变得非常简单。

```
┌─────────────────────────────────────────────────────────────────┐
│  应用层 / RTOS 层                                                │
│  Core/Src/freertos.c  ·  UI/Core/Src/ui_main.c                 │
│  – FreeRTOS 任务：初始化流程、lv_timer_handler 循环、            │
│    200ms 状态同步、g_boot_stage 启动诊断                         │
├─────────────────────────────────────────────────────────────────┤
│  UI 组装层                                                       │
│  UI/Core/Src/tianji_ui.c  (tianji_ui_t)                        │
│  – 将所有控件组合到一个屏幕                                       │
│  – 对外暴露 set_temp / get_temp / voice_result 接口              │
├──────────────────────────────┬──────────────────────────────────┤
│  UI 控件层                   │  领域数据层                       │
│  tianji_dial.c   (Arc+拖拽)  │  tianji_data.c                  │
│  tianji_particles.c (画布)   │  – 24 节气                      │
│  tianji_voice.c  (灵珠+光环) │  – 修仙境界表                   │
│                              │  – 五行/八卦表                   │
│                              │  – sin/cos 查表（纯整数运算）    │
├──────────────────────────────┴──────────────────────────────────┤
│  驱动集成层  (LVGL HAL 适配器)                                   │
│  UI/Core/Src/display_drv.c  ·  touch_drv.c                     │
│  – 向 LVGL 注册 lv_disp_drv_t / lv_indev_drv_t                 │
│  – 行缓冲策略：20 行 × 240 像素（≈9.4 KB）                       │
├─────────────────────────────────────────────────────────────────┤
│  BSP 组件层                                                      │
│  Drivers/BSP/Components/ili9341  ·  xpt2046                     │
│  UI/Core/Src/bsp_stubs.c  (替换此文件即可适配新硬件)             │
│  – lcd_init / lcd_set_window / lcd_write_pixels                 │
│  – touch_init_hw / touch_is_pressed / touch_read_coords         │
├─────────────────────────────────────────────────────────────────┤
│  HAL / CMSIS / 硬件                                              │
│  Drivers/STM32F4xx_HAL_Driver  ·  CMSIS                        │
│  FSMC (8080 并口接 ILI9341)、SPI (XPT2046)、TIM、GPIO           │
└─────────────────────────────────────────────────────────────────┘
```

### 架构评估

| 关注点 | 实现方式 |
|--------|---------|
| **硬件可移植性** | `bsp_stubs.c` 是唯一直接操作 LCD/触摸寄存器的文件；替换它即可适配整栈到新板子 |
| **LVGL 解耦** | `display_drv` / `touch_drv` 是纯粹的 LVGL HAL 适配器；控件代码绝不直接调用 BSP 函数 |
| **领域数据隔离** | `tianji_data.c` 零 LVGL 依赖——数据表和辅助函数可以在 PC 上脱离 MCU 测试 |
| **控件封装** | 每个控件（dial、particles、voice）拥有自己的 LVGL 对象，对外暴露清晰的 create/destroy + setter API |
| **RTOS 集成** | 单个 FreeRTOS 任务同时驱动 `lv_timer_handler`（5ms 周期）和定时状态更新（200ms），避免多任务下 LVGL 同步问题 |
| **内存策略** | 20 行的 LVGL draw buffer 控制堆使用；粒子画布是最大的单次分配（≈58 KB），通过链接脚本放在 SRAM 中 |

### 设计亮点

- **FSMC 8080 并口** — ILI9341 通过 FSMC 而非 SPI 连接，像素吞吐量远超 SPI（对全屏刷新至关重要）。
- **FatFs + LibJPEG** — 文件系统和 JPEG 解码已集成，后续可从 SD 卡加载自定义背景图或固件资源而无需修改 UI 层代码。
- **启动阶段诊断** — `g_boot_stage` 是一个 `volatile uint32_t`，在每一步初始化时递增。调试时放在 Watch 窗口即可精确定位启动卡死位置。
- **触摸校准钩子** — `touch_drv_calibrate()` 已预留框架，可在首次启动或通过恢复出厂手势触发，校准数据写入 FatFs。
- **BSP 自测** — `Tests/BSP/main_bsp_test.c` 和 `SOP_BSP_LCD_SELF_TEST.md` 提供了独立的 LCD/触摸冒烟测试，可在烧录完整应用前验证硬件。
- **双构建系统** — 同时提供 `CMakeLists.txt`（GCC + CMake）和 Keil `.uvprojx` 工程，用任一工具链编译都无需改动。

---

## UI 设计

### 交互原型

👉 https://miqm4hlcmthn4.ok.kimi.link/?sharetype=link

### 屏幕布局（240 × 320 竖屏）

```text
+----------------------------+  y=0
| 芒种 · 状态                |  h=30   状态栏 — 节气、日期、当前温度
| 2024年/6月/5日      28°C   |
+----------------------------+  y=30
|                            |
|    ~ 粒子画布 ~            |
|   +--------------+         |
|   |   温 度      |         |  h=195  主控区域
|   |   24.0°C     |         |          – 动态双螺旋粒子
|   |   元婴境界    |         |          – LVGL Arc 温度旋钮
|   +--------------+         |          – 修仙境界标签
|                            |
+----------------------------+  y=225
| 炼气 / 筑基 / 金丹 / 元婴… |  h=22   境界进度条
+----------------------------+  y=247
|     [灵珠]  语音助手       |  h=35   AI 语音核心按钮
+----------------------------+  y=282
| [模式]  [风速]  [定时]     |  h=38   底部控制按钮
+----------------------------+  y=320
| 二十四节气滚动播报 ……      |         节气滚动条
+----------------------------+
```

### 核心 UI 功能

| 功能 | 描述 |
|------|------|
| **双螺旋粒子** | 20 粒子的动态画布，使用查表法 sin/cos——零浮点运算 |
| **温度旋钮** | 在 LVGL Arc 控件上拖拽手势；灵敏度 `x×0.3 − y×0.2` |
| **修仙境界** | 温度范围映射到 炼气 → 筑基 → 金丹 → 元婴 → 化神…… |
| **节气状态栏** | 显示当前二十四节气、日期和实时温度 |
| **AI 语音核心** | 点击灵珠激活；结果通过 `tianji_ui_voice_result()` 更新旋钮和境界标签 |

---

## 快速开始

### 第 0 步：准备硬件

1. 确认你用的是**启明欣欣 STM32F407 开发板（高配版 V6.1）**
2. LCD 模块已插接到板上的 TFT 接口
3. 用 ST-Link（或 J-Link/DAP-Link）连接开发板和电脑
4. 给开发板上电

> 如果你用的是其他开发板，请阅读 [UI/README.md](UI/README.md) 中的移植指南。

### 第 1 步：克隆仓库

```bash
git clone https://github.com/arniexu/temperature-controller-ai-voice-muggle-fairy.git
cd temperature-controller-ai-voice-muggle-fairy

# ★ 重要：一定要拉取 LVGL 子模块！
git submodule update --init --recursive
```

### 第 2 步：安装软件

| 步骤 | 操作 |
|------|------|
| 1 | 安装 [Keil MDK v5](https://www.keil.com/download/product/) |
| 2 | 打开 Keil，点击 Pack Installer，安装 **Keil.STM32F4xx_DFP.2.17.1**（或更高版本） |
| 3 | 确认 ARMCLANG V6.24 编译器已安装（在 Keil 安装目录下） |

### 第 3 步：打开工程并编译

1. 双击 `MDK-ARM/ai_tempconroler.uvprojx` 打开工程
2. 在 Keil 顶部选择 `Project → Build Target`（或按 F7）
3. 等待编译完成，应看到 `0 Error(s), 0 Warning(s)`

### 第 4 步：烧录并验证

1. 在 Keil 中点击 `Flash → Download`（或按 F8）
2. 烧录完成后，按下开发板复位键
3. **验收现象：**
   - ✅ LCD 屏幕亮起，背光正常
   - ✅ 屏幕上显示状态栏（节气、日期、温度）
   - ✅ 可以看到双螺旋粒子动画
   - ✅ 触摸旋钮可以拖拽调节温度
   - ✅ 底部按钮可以点击

> 如果屏幕没有显示，请检查：LCD 排线是否插紧？背光跳线是否接好？

### 第 5 步：连接串口查看日志（可选）

1. 用 USB 转串口模块连接开发板的 UART1（PA9/PA10 经过 MAX232）
2. 串口参数：**115200 波特率、8 数据位、1 停止位、无校验**
3. 复位开发板，应看到启动日志输出

---

## 常见问题

<details>
<summary><b>Q: 编译时报 "cannot open source file lvgl.h"？</b></summary>

A: 你没有拉取 LVGL 子模块。请在仓库根目录执行：
```bash
git submodule update --init --recursive
```
</details>

<details>
<summary><b>Q: 编译时报 Pack 相关错误？</b></summary>

A: 你可能没安装 STM32F4xx_DFP 器件包。打开 Keil → Pack Installer → 搜索 STM32F4 → 安装最新版。
</details>

<details>
<summary><b>Q: 编译时报编译器版本错误？</b></summary>

A: 工程默认使用 ARMCLANG V6.24（AC6）。如果你的 Keil 只有 AC5，需要：
1. 安装 ARMCLANG V6（从 Keil 官网下载）
2. 或在工程选项中切换到 AC5 并修复兼容性问题
</details>

<details>
<summary><b>Q: 烧录后屏幕白屏/花屏？</b></summary>

A: 可能原因：FSMC 配置与你的 LCD 不匹配。
1. 首先运行 `Tests/BSP/main_bsp_test.c` 测试——这是独立的 LCD 冒烟测试
2. 检查 LCD 排线方向是否正确
3. 确认 LCD 控制器型号（ILI9341 vs ST7789，读丝印）
</details>

<details>
<summary><b>Q: 我没有这块开发板，能用吗？</b></summary>

A: 能！本项目 BSP 层通过 `bsp_stubs.c` 抽象。你只需要：
1. 替换 `bsp_stubs.c` 中的 LCD 和触摸驱动函数
2. 修改 `lv_conf.h` 中的分辨率配置
3. 调整 FSMC/SPI 管脚配置
详见 [UI/README.md](UI/README.md) 移植指南。
</details>

<details>
<summary><b>Q: 如何运行 BSP 自测？</b></summary>

A: `Tests/BSP/` 目录下有独立的 LCD/触摸冒烟测试代码。在 Keil 中将 `main_bsp_test.c` 替换 `main.c` 编译烧录即可。串口输出 "PASS" 表示 BSP 驱动正常。
</details>

---

## 预编译固件

如果你暂时不想安装 Keil 编译环境，想先烧录看效果：

1. 编译后会在 `MDK-ARM/ai_tempconroler/` 目录下生成 `ai_tempconroler.hex` 或 `ai_tempconroler.bin`
2. 用 ST-Link Utility（或 STM32CubeProgrammer）烧录到开发板
3. 烧录地址：`0x08000000`

> 预编译固件会随 Release 版本发布，请关注 [Releases](https://github.com/arniexu/temperature-controller-ai-voice-muggle-fairy/releases) 页面。

---

## 完整文档

| 文档 | 内容 |
|------|------|
| **[UI/README.md](UI/README.md)** | UI 层完整指南：文件结构、BSP 驱动集成、构建说明、性能优化、内存估算 |
| [pin mapping.md](pin mapping.md) | 启明欣欣 STM32F407 开发板完整管脚映射表 |

---

## 许可证

MIT License，与 LVGL 保持一致。

---

<p align="center">
  <i>代码质量是骨架，文档是血肉。这个仓库是我"希望当初有人这样教我"的方式写出来的。</i><br>
  <i>如果你觉得有帮助，请给个 ⭐ Star ~</i>
</p>