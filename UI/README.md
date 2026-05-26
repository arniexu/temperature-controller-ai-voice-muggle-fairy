# 天机微控 (Tianji Thermostat) - STM32F407 版

## 项目概述

将PC端的"天机微控"赛博修仙温控仪完整移植到 **STM32F407** 平台，适配 **240x320 触摸屏**，基于 **LVGL v8.3** GUI框架。

## 硬件需求

| 组件 | 规格 |
|------|------|
| MCU | STM32F407VG (168MHz, 1MB Flash, 192KB SRAM) |
| 屏幕 | 240x320 TFT LCD, 16-bit RGB565 |
| 屏幕控制器 | ILI9341 或 ST7789 (SPI或8080接口) |
| 触摸屏 | XPT2046 电阻式触摸 (SPI接口) |
| 背光 | PWM可调LED背光 |

## 软件栈

- **STM32CubeF4 HAL** - 底层硬件抽象
- **FreeRTOS** (可选) - 实时操作系统
- **LVGL v8.3** - 图形界面库
- **GCC ARM Toolchain** 或 **Keil MDK** / **STM32CubeIDE**

## 文件结构

```
stm32_tianji/
├── Core/
│   ├── Inc/
│   │   ├── tianji_data.h         # 节气、五行、八卦、修仙境界数据
│   │   ├── tianji_particles.h    # 双螺旋粒子特效
│   │   ├── tianji_dial.h         # 温控旋钮控件
│   │   ├── tianji_voice.h        # AI语音交互核心
│   │   ├── tianji_ui.h           # 主UI组装
│   │   ├── display_drv.h         # 显示驱动接口
│   │   └── touch_drv.h           # 触摸驱动接口
│   └── Src/
│       ├── main.c                # 程序入口
│       ├── tianji_data.c         # 数据表与工具函数
│       ├── tianji_particles.c    # 粒子特效实现(查表法sin/cos)
│       ├── tianji_dial.c         # 旋钮控件(LVGL Arc)
│       ├── tianji_voice.c        # 语音核心UI
│       ├── tianji_ui.c           # 主UI组装
│       ├── display_drv.c         # LVGL显示驱动集成
│       ├── touch_drv.c           # LVGL触摸驱动集成
│       └── bsp_stubs.c           # BSP存根(需替换为实际驱动)
├── LVGL/
│   └── lv_conf.h                 # LVGL配置文件
└── README.md
```

## 关键设计决策

### 1. GUI库选型: LVGL v8.3

**为什么选LVGL:**
- 开源免费，MIT许可证
- 对STM32F407资源占用合理 (~32KB RAM, ~150KB Flash)
- 完善的动画系统(lv_anim)、样式系统、事件系统
- 内置Canvas支持，方便实现粒子特效
- 成熟的触摸支持
- 活跃的社区和中文文档

### 2. 粒子特效优化

PC端使用Canvas 2D + globalCompositeOperation='lighter'实现双螺旋光效。
STM32版优化策略:
- **粒子数量**: 从100减至20
- **数学计算**: 使用256点查表法替代math.h的sin/cos
- **渲染方式**: LVGL Canvas + 半透明矩形(圆形用圆角矩形模拟)
- **拖尾效果**: 半透明背景叠加(LV_OPA_30清屏)
- **坐标计算**: 纯整数运算，无浮点

### 3. 240x320竖屏布局

```
+----------------------------+  y=0
| 芒种 · 一候螳螂生   优    |  h=30  状态栏
| 己巳年 · 5月24日    28°C |        
+----------------------------+  y=30
|                            |        
|       ~ 粒子画布 ~         |        
|     +--------------+       |        
|     |   温控罗盘    |       |  h=195 主控区
|     |    24.0°C    |       |        
|     |    元婴期     |       |        
|     +--------------+       |        
|                            |        
+----------------------------+  y=225
| 炼气 筑基 金丹 元婴 化神..|  h=22  修仙境界指示
+----------------------------+  y=247
|      [灵珠] 气聚神凝      |  h=35  语音核心
+----------------------------+  y=282
| [修仙] [巽风] [甲子]       |  h=38  底部控制
+----------------------------+  y=320
| 立春 · 雨水 · 惊蛰 ...(滚动) |       节气跑马灯
+----------------------------+
```

### 4. 触摸交互

- **旋钮拖拽**: 水平滑动增温，垂直滑动降温
  - 灵敏度: x*0.3 - y*0.2 映射到温度变化
  - 阻尼: 物理吸附感(通过增量计算自然实现)
- **按钮点击**: 模式/风速/定时三按钮
- **灵珠唤醒**: 点击语音核心按钮激活语音输入

## 快速开始

### 步骤1: 准备开发环境

1. 安装 **STM32CubeIDE** 或 **Keil MDK**
2. 下载 **LVGL v8.3** 源码: `git clone -b release/v8.3 https://github.com/lvgl/lvgl.git`
3. 使用 **STM32CubeMX** 创建F407工程，配置:
   - HSE外部晶振8MHz
   - SYSCLK 168MHz
   - SPI1 (LCD + Touch)
   - TIM3 (背光PWM)
   - FreeRTOS (可选)

### 步骤2: 添加文件到工程

1. 将 `stm32_tianji/Core/Inc/*.h` 添加到Include路径
2. 将 `stm32_tianji/Core/Src/*.c` 添加到编译源文件
3. 将 `lv_conf.h` 放到LVGL根目录或include路径
4. 确保LVGL的 `lvgl.h` 在include路径中

### 步骤3: 实现BSP驱动 (关键!)

编辑 `bsp_stubs.c`，替换存根函数:

```c
// 必须实现的函数:
void lcd_init(void);                    // LCD控制器初始化
void lcd_set_window(x1,y1,x2,y2);     // 设置绘图窗口
void lcd_write_pixels(data,count);     // 写入像素数据(建议使用DMA)
void lcd_write_pixel(color);           // 写入单个像素
void lcd_set_backlight_hw(percent);    // PWM背光控制
void lcd_enter_sleep(void);            // 休眠
void lcd_exit_sleep(void);             // 唤醒

void touch_init_hw(void);              // 触摸控制器初始化
uint8_t touch_is_pressed(void);        // 检测触摸
void touch_read_coords(x,y);           // 读取坐标
```

### 步骤4: 编译烧录

```bash
# 使用STM32CubeIDE: 导入工程 -> 编译 -> Debug
# 或使用Makefile:
make clean
make -j$(nproc)
st-flash write build/tianji.bin 0x8000000
```

## 关键API使用

### 初始化UI
```c
#include "tianji_ui.h"

tianji_ui_t *ui = tianji_ui_init();
```

### 外部设置温度(如语音命令)
```c
tianji_ui_set_temp(ui, 260);  // 26.0°C
```

### 处理语音识别结果
```c
tianji_ui_voice_result(ui, "调高温度", 280);  // 28°C
```

### 主循环
```c
while (1) {
    lv_timer_handler();  // LVGL任务处理 ~每5ms
    HAL_Delay(5);
}
```

## 性能优化建议

1. **SPI速度**: 设置SPI波特率为PCLK/2 (最高42MHz for SPI1 on APB2)
2. **DMA传输**: 使用SPI+DMA传输像素数据，在DMA TC中断中调用 `lv_disp_flush_ready()`
3. **粒子帧率**: 如果卡顿，减少 `TJ_PARTICLE_COUNT` (20 -> 12)
4. **LVGL刷新周期**: 在 `lv_conf.h` 中调整 `LV_DISP_DEF_REFR_PERIOD` (默认16ms=60FPS)
5. **Flash占用**: LVGL默认字体较大，可只保留montserrat 8,10,12,14,16,20
6. **SRAM布局**: 在链接脚本中定义 `.sram` section 用于DMA缓冲区

## 内存占用预估

| 组件 | Flash | RAM |
|------|-------|-----|
| LVGL核心 | ~80KB | ~32KB |
| 字体 | ~60KB | - |
| 粒子画布 (170x170x2) | - | ~58KB |
| UI代码+数据 | ~30KB | ~2KB |
| DMA缓冲 (2x) | - | ~150KB (在sram) |
| **总计** | **~170KB** | **~32KB + 画布** |

## 进阶定制

### 添加自定义字体
使用LVGL字体转换器: https://lvgl.io/tools/fontconverter
- 转换"站酷庆科黄油体"用于节气显示
- 建议只包含需要的字符(节气名+八卦)

### 添加语音模块
- 连接LD3320或SU-03T语音模块到UART
- 在 `voice_activated()` 回调中发送唤醒命令
- 接收识别结果后调用 `tianji_ui_voice_result()`

### 添加WiFi/蓝牙
- 使用ESP8266/ESP32作为协处理器
- 通过UART获取天气数据
- 更新状态栏的室外温度和AQI

## License

MIT License - 与LVGL保持一致。
