# 天机微控 (Tianji Thermostat) - LVGL 元素属性配置手册

**屏幕规格**: 240 x 320 像素竖屏 | **平台**: STM32F407VG | **GUI 库**: LVGL v8.3 | **颜色格式**: RGB565

---

## 目录

- [一、概述](#一概述)
- [二、颜色常量表](#二颜色常量表)
- [三、屏幕布局网格](#三屏幕布局网格)
- [四、顶部状态栏 (Status Bar)](#四顶部状态栏-status-bar)
- [五、粒子画布 (Particle Canvas)](#五粒子画布-particle-canvas)
- [六、温控罗盘 (Thermostat Dial)](#六温控罗盘-thermostat-dial)
- [七、修仙境界指示条 (Cultivation Bar)](#七修仙境界指示条-cultivation-bar)
- [八、语音交互核心 (Voice Core)](#八语音交互核心-voice-core)
- [九、底部控制按钮 (Bottom Controls)](#九底部控制按钮-bottom-controls)
- [十、节气跑马灯 (Marquee)](#十节气跑马灯-marquee)
- [十一、事件处理与回调](#十一事件处理与回调)
- [附录 A：完整初始化模板](#附录-a完整初始化模板)
- [附录 B：资源占用](#附录-b资源占用)

---

## 一、概述

### 1.1 组件清单

本文档覆盖以下 7 个 UI 组件，每个组件包含完整的 LVGL v8.3 属性配置（可直接用于 STM32F407 项目）：

| 组件 | LVGL 类型 | Z 层级 | 坐标 (Y) | 高度 |
|------|-----------|--------|----------|------|
| 状态栏 | `lv_obj_t` + `lv_label_t` x4 | 10 | 0 | 30 |
| 粒子画布 | `lv_canvas_t` | 4 | 30 | 180 |
| 温控罗盘 | `lv_obj_t` + SVG Arc | 10 | 30 | 180 |
| 境界指示条 | `lv_obj_t` + dot x7 | 10 | 210 | 22 |
| 语音核心 | `lv_btn_t` + `lv_label_t` x2 | 10 | 232 | 38 |
| 底部按钮 | `lv_btn_t` x3 | 10 | 270 | 38 |
| 节气跑马灯 | `lv_label_t` | 10 | 308 | 12 |

### 1.2 字体配置

| 用途 | 字体 | 大小 | 字重 |
|------|------|------|------|
| 中文显示文字 | CJK 扩展字体 | 10-14px | Bold |
| 温度数值 | `lv_font_montserrat_36` | 36px | Bold |
| 小数位/单位 | `lv_font_montserrat_12` | 12px | Normal |
| 状态文字 | `lv_font_montserrat_14` | 14px | Normal |
| 八卦/境界文字 | `lv_font_montserrat_8` | 8px | Normal |
| 跑马灯 | `lv_font_montserrat_8` | 8px | Normal |

---

## 二、颜色常量表

所有颜色以 RGB565 格式定义，适用于 STM32F407 + ILI9341 16 位屏幕。

### 2.1 核心颜色

| 宏定义 | 十六进制 | RGB565 | 用途 |
|--------|----------|--------|------|
| `TIANJI_COLOR_BG` | `#05050A` | `0x080C` | 背景色 |
| `TIANJI_COLOR_ICE_BLUE` | `#A3D5FF` | `0x9EBF` | 冰蓝色（冷气/光晕） |
| `TIANJI_COLOR_NEON_GREEN` | `#39FF14` | `0x07E0` | 翠绿色（激活/轨迹） |
| `TIANJI_COLOR_MYSTIC_PURPLE` | `#7B2D8E` | `0x7195` | 魅紫色（高能预警） |
| `TIANJI_COLOR_WHITE` | `#FFFFFF` | `0xFFFF` | 高亮文字 |
| `TIANJI_COLOR_GRAY` | `#7B7B7B` | `0x7BEF` | 次级文字 |
| `TIANJI_COLOR_DARK_GRAY` | `#3A3A5A` | `0x39E7` | 暗色文字/刻度 |
| `TIANJI_COLOR_GOLD` | `#FFD700` | `0xFE00` | 高温指示 |
| `TIANJI_COLOR_FIRE` | `#FF4500` | `0xFA20` | 烈火（离火风速） |
| `TIANJI_COLOR_ORANGE` | `#FF8C00` | `0xFD20` | 练虚境界 |
| `TIANJI_COLOR_PANEL_BG` | `rgba(10,10,18)` | `0x1082` | 面板背景 |
| `TIANJI_COLOR_CYAN` | `#00FFFF` | `0x07FF` | 青色（备用） |

### 2.2 温度渐变函数

输入 `0.0` = 冷（蓝），`1.0` = 热（金），返回 RGB565 颜色值。

```c
uint16_t tianji_temp_gradient(float percent) {
    if (percent <= 0.0f) return TIANJI_COLOR_ICE_BLUE;
    if (percent >= 1.0f) return TIANJI_COLOR_GOLD;

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
```

### 2.3 修仙境界颜色映射

| 境界 | 温度范围 | RGB565 | 颜色 |
|------|----------|--------|------|
| 炼气 | 16.0-18.0 C | `0x9EBF` | #A3D5FF |
| 筑基 | 18.0-20.0 C | `0x5FFF` | #5FDDFF |
| 金丹 | 20.0-22.0 C | `0x07E0` | #39FF14 |
| 元婴 | 22.0-24.0 C | `0x57E0` | #57FF00 |
| 化神 | 24.0-26.0 C | `0xFE00` | #FFD700 |
| 练虚 | 26.0-28.0 C | `0xFD20` | #FF8C00 |
| 合体 | 28.0-30.0 C | `0xFA20` | #FF4500 |

---

## 三、屏幕布局网格

```c
#define TIANJI_SCREEN_W     240
#define TIANJI_SCREEN_H     320

#define TIANJI_STATUS_H      30   /* y: 0~30     */
#define TIANJI_DIAL_Y        30   /* y: 30~210   */
#define TIANJI_DIAL_H       180
#define TIANJI_REALM_Y      210   /* y: 210~232  */
#define TIANJI_REALM_H       22
#define TIANJI_VOICE_Y      232   /* y: 232~270  */
#define TIANJI_VOICE_H       38
#define TIANJI_BOTTOM_Y     270   /* y: 270~308  */
#define TIANJI_BOTTOM_H      38
```

### 视觉分区

```
+----------------------------+  y=0
| 状态栏 (240x30)            |  节气 + 农历 + 室外温度 + AQI
+----------------------------+  y=30
|                            |
| 温控罗盘区 (240x180)       |  八卦环绕 + 渐变弧 + 温度数字
| 粒子画布 (160x160) 在底层  |  双螺旋粒子特效
|                            |
+----------------------------+  y=210
| 境界指示条 (240x22)        |  炼气 筑基 金丹 元婴 化神 练虚 合体
+----------------------------+  y=232
| 语音核心 (240x38)          |  轨道环 + 灵珠 + 状态文字
+----------------------------+  y=270
| 底部按钮 (240x38)          |  修仙 | 巽风 | 甲子
+----------------------------+  y=308
| 节气跑马灯 (240x12)        |  立春 · 雨水 · 惊蛰 · ...
+----------------------------+  y=320
```

---

## 四、顶部状态栏 (Status Bar)

**区域**: y:0~30, h:30px | **容器**: `lv_obj_t` | **Z**: 10

### 4.1 容器

```c
lv_obj_t *status_bar = lv_obj_create(lv_scr_act());
lv_obj_set_size(status_bar, 240, 30);
lv_obj_set_pos(status_bar, 0, 0);
lv_obj_set_style_bg_color(status_bar, lv_color_hex(TIANJI_COLOR_BG), 0);
lv_obj_set_style_bg_opa(status_bar, LV_OPA_80, 0);
lv_obj_set_style_border_width(status_bar, 0, 0);
lv_obj_set_style_pad_all(status_bar, 2, 0);
lv_obj_clear_flag(status_bar, LV_OBJ_FLAG_SCROLLABLE);
```

| 属性 | 值 |
|------|-----|
| 尺寸 | 240 x 30 |
| 位置 | (0, 0) |
| 背景色 | `0x080C` (#05050A) |
| 背景透明度 | LV_OPA_80 (约 31%) |
| 边框 | 0px |
| 内边距 | 2px |
| 滚动 | 禁用 |

### 4.2 节气标签

```c
lv_obj_t *term_label = lv_label_create(status_bar);
lv_label_set_text(term_label, "芒种 · 一候螳螂生");
lv_obj_set_style_text_font(term_label, &lv_font_montserrat_10, 0);
lv_obj_set_style_text_color(term_label, lv_color_hex(TIANJI_COLOR_NEON_GREEN), 0);
lv_obj_set_pos(term_label, 4, 2);
```

| 属性 | 值 |
|------|-----|
| 父对象 | status_bar |
| 文本 | "芒种 · 一候螳螂生" (动态读取当前节气) |
| 字体 | `lv_font_montserrat_10` |
| 颜色 | `0x07E0` (#39FF14) |
| 位置 | (4, 2) |
| 文字阴影 | `text-shadow: 0 0 6px rgba(57,255,20,0.5)` (CSS 对应) |

### 4.3 农历日期标签

```c
lv_obj_t *lunar_label = lv_label_create(status_bar);
lv_label_set_text(lunar_label, "己巳年 · 5月24日");
lv_obj_set_style_text_font(lunar_label, &lv_font_montserrat_8, 0);
lv_obj_set_style_text_color(lunar_label, lv_color_hex(0x6A6A8A), 0);
lv_obj_set_pos(lunar_label, 4, 16);
```

| 属性 | 值 |
|------|-----|
| 文本 | "己巳年 · 5月24日" (动态读取 RTC) |
| 字体 | `lv_font_montserrat_8` |
| 颜色 | `0x6A6A8A` |
| 位置 | (4, 16) |

### 4.4 室外温度标签

```c
lv_obj_t *outdoor_label = lv_label_create(status_bar);
lv_label_set_text(outdoor_label, "外 28°C · 52%");
lv_obj_set_style_text_font(outdoor_label, &lv_font_montserrat_8, 0);
lv_obj_set_style_text_color(outdoor_label, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
lv_obj_align(outdoor_label, LV_ALIGN_TOP_RIGHT, -30, 2);
```

| 属性 | 值 |
|------|-----|
| 文本 | "外 28°C · 52%" |
| 字体 | `lv_font_montserrat_8` |
| 颜色 | `0x9EBF` (#A3D5FF) |
| 对齐 | 右上角偏移 (-30, 2) |

### 4.5 AQI 徽章

```c
lv_obj_t *aqi_badge = lv_obj_create(status_bar);
lv_obj_set_size(aqi_badge, 28, 16);
lv_obj_align(aqi_badge, LV_ALIGN_TOP_RIGHT, -2, 4);
lv_obj_set_style_radius(aqi_badge, 8, 0);
lv_obj_set_style_bg_color(aqi_badge, lv_color_hex(0x0A1A0A), 0);
lv_obj_set_style_bg_opa(aqi_badge, LV_OPA_COVER, 0);
lv_obj_set_style_border_color(aqi_badge, lv_color_hex(TIANJI_COLOR_NEON_GREEN), 0);
lv_obj_set_style_border_width(aqi_badge, 1, 0);
lv_obj_set_style_border_opa(aqi_badge, LV_OPA_40, 0);

/* 内部文字 */
lv_obj_t *aqi_text = lv_label_create(aqi_badge);
lv_label_set_text(aqi_text, "优");
lv_obj_set_style_text_font(aqi_text, &lv_font_montserrat_8, 0);
lv_obj_set_style_text_color(aqi_text, lv_color_hex(TIANJI_COLOR_NEON_GREEN), 0);
lv_obj_center(aqi_text);
```

| 属性 | 值 |
|------|-----|
| 尺寸 | 28 x 16 |
| 圆角 | 8px |
| 背景色 | `0x0A1A0A` |
| 边框色 | `0x07E0` (#39FF14) |
| 边框透明度 | LV_OPA_40 |
| 内部文字 | "优" (根据 AQI 值动态: 优/良/差) |
| 文字颜色 | `0x07E0` |

---

## 五、粒子画布 (Particle Canvas)

**区域**: y:30~210 (居中 160x160) | **容器**: `lv_canvas_t` | **Z**: 4 (在罗盘下方)

### 5.1 容器与 Canvas 创建

```c
/* 透明容器 (pointer-events: none) */
lv_obj_t *canvas_wrap = lv_obj_create(lv_scr_act());
lv_obj_set_size(canvas_wrap, 160, 160);
lv_obj_set_pos(canvas_wrap, (240-160)/2, 30 + (180-160)/2); /* (40, 40) */
lv_obj_set_style_bg_opa(canvas_wrap, LV_OPA_TRANSP, 0);
lv_obj_add_flag(canvas_wrap, LV_OBJ_FLAG_CLICKABLE);

/* Canvas 帧缓冲 (160*160*2 = 51200 bytes) */
static lv_color_t canvas_buf[160 * 160];
lv_obj_t *canvas = lv_canvas_create(canvas_wrap);
lv_canvas_set_buffer(canvas, canvas_buf, 160, 160, LV_IMG_CF_TRUE_COLOR);
lv_canvas_fill_bg(canvas, lv_color_hex(TIANJI_COLOR_BG), LV_OPA_COVER);
```

| 属性 | 值 |
|------|-----|
| Canvas 尺寸 | 160 x 160 |
| Canvas 位置 | (40, 40) 相对屏幕 |
| 缓冲区 | `static lv_color_t canvas_buf[160*160]` (51.2KB) |
| 颜色格式 | LV_IMG_CF_TRUE_COLOR (RGB565) |
| 初始填充 | `0x080C` (#05050A), 完全不透明 |

### 5.2 粒子参数定义

```c
#define TJ_PARTICLE_COUNT   20      /* 粒子总数 (左右各10) */
```

| 参数 | 值 | 说明 |
|------|-----|------|
| `particleCount` | 20 | 粒子总数 (每束 helix 10 个) |
| `color1` | `#39FF14` (0x07E0) | 翠绿色光带 |
| `color2` | `#A3D5FF` (0x9EBF) | 冰蓝色光带 |
| `speed` | 0.5 ~ 2.0 | 随温度变化: `0.5 + (temp-16)/14 * 1.5` |
| `spread` | 45px | 垂直散射范围 |
| `size` | 1~3px | 粒子半径 (随机) |
| `alpha` | 30~200 | 透明度范围 (呼吸效果) |
| `strand` | +1 / -1 | helix 方向 |

### 5.3 渲染循环 (每帧)

```c
/* 1. 半透明清屏 → 拖尾效果 */
lv_canvas_fill_bg(canvas, lv_color_hex(TIANJI_COLOR_BG), LV_OPA_30);

/* 2. 混合模式: lighter (加法混合) */
/* LVGL: 通过 lv_obj_set_style_blend_mode() 设置 */

/* 3. 绘制每个粒子 */
for (int i = 0; i < TJ_PARTICLE_COUNT; i++) {
    tj_particle_t *pt = &particles[i];
    pt->t += pt->speed;  /* 角度累加 */
    
    /* 位置计算 (查表法 sin/cos) */
    int16_t s = fast_sin(pt->t);
    int16_t c = fast_cos((uint16_t)((int16_t)pt->t * pt->strand));
    int16_t px = cx + (c * pt->radius) >> 8;
    int16_t py = cy + (s * pt->radius) >> 7 + pt->y_offset;
    
    /* 呼吸透明度 */
    uint8_t a = (pt->alpha * (128 + (s >> 1))) >> 8;
    if (a < 30) a = 30;
    
    /* 绘制圆角矩形模拟粒子 */
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = lv_color_hex(pt->strand > 0 ? color1 : color2);
    rect_dsc.bg_opa = (lv_opa_t)((a * LV_OPA_COVER) / 255);
    rect_dsc.radius = pt->size;
    lv_canvas_draw_rect(canvas, px - pt->size, py - pt->size, 
                        pt->size * 2, pt->size * 2, &rect_dsc);
}

/* 4. 暗化叠加层 */
lv_draw_rect_dsc_t overlay;
lv_draw_rect_dsc_init(&overlay);
overlay.bg_color = lv_color_hex(TIANJI_COLOR_BG);
overlay.bg_opa = LV_OPA_20;
lv_canvas_draw_rect(canvas, 0, 0, 160, 160, &overlay);
```

| 绘制步骤 | 操作 | 效果 |
|----------|------|------|
| 1 | LV_OPA_30 填充背景 | 留下残影/拖尾 |
| 2 | 加法混合模式 | 绿+蓝叠加发亮 |
| 3 | 绘制20个粒子 | 双 helix 形态 |
| 4 | LV_OPA_20 暗化覆盖 | 稳定拖尾衰减 |

---

## 六、温控罗盘 (Thermostat Dial)

**区域**: y:30~210, h:180px | **容器**: `lv_obj_t` | **Z**: 10 | **中心**: cx=120, cy=120, r=78

### 6.1 容器

```c
lv_obj_t *dial = lv_obj_create(lv_scr_act());
lv_obj_set_pos(dial, 0, 30);
lv_obj_set_size(dial, 240, 180);
lv_obj_set_style_bg_opa(dial, LV_OPA_TRANSP, 0);
lv_obj_set_style_border_width(dial, 0, 0);
lv_obj_add_event_cb(dial, dial_drag_cb, LV_EVENT_ALL, user_data);
lv_obj_add_flag(dial, LV_OBJ_FLAG_CLICKABLE);
```

| 属性 | 值 |
|------|-----|
| 尺寸 | 240 x 180 |
| 位置 | (0, 30) |
| 背景 | 完全透明 |
| 事件 | `dial_drag_cb` (PRESS/PRESSING/RELEASED) |
| 触摸 | 可点击/可拖拽 |

### 6.2 外圈装饰环

```c
lv_obj_t *outer_ring = lv_obj_create(dial);
lv_obj_set_size(outer_ring, 156, 156);       /* r=78 */
lv_obj_center(outer_ring);
lv_obj_set_style_radius(outer_ring, LV_RADIUS_CIRCLE, 0);
lv_obj_set_style_bg_opa(outer_ring, LV_OPA_TRANSP, 0);
lv_obj_set_style_border_color(outer_ring, lv_color_hex(0x1A1A2E), 0);
lv_obj_set_style_border_width(outer_ring, 1, 0);
lv_obj_set_style_shadow_color(outer_ring, lv_color_hex(TIANJI_COLOR_ICE_BLUE), 0);
lv_obj_set_style_shadow_width(outer_ring, 8, 0);
lv_obj_set_style_shadow_opa(outer_ring, LV_OPA_10, 0);
```

| 属性 | 值 |
|------|-----|
| 尺寸 | 156 x 156 (半径 78) |
| 形状 | 正圆 |
| 边框色 | `0x1A1A2E` |
| 边框宽 | 1px |
| 阴影色 | `0x9EBF` (#A3D5FF) |
| 阴影宽 | 8px |
| 阴影透明度 | LV_OPA_10 |

### 6.3 八卦文字标签 (8个)

```c
const char *trigrams[8] = { "乾","兑","离","震","巽","坎","艮","坤" };
for (int i = 0; i < 8; i++) {
    lv_obj_t *lbl = lv_label_create(dial);
    lv_label_set_text(lbl, trigrams[i]);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_8, 0);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0x4A4A6A), 0);
    float a = (i * 45 - 90) * PI / 180;
    int tx = 120 + 68 * cos(a) - 6;
    int ty = 120 + 68 * sin(a) - 5;
    lv_obj_set_pos(lbl, tx, ty);
}
```

| 属性 | 值 |
|------|-----|
| 文本 | 乾、兑、离、震、巽、坎、艮、坤 |
| 字体 | `lv_font_montserrat_8` |
| 颜色 | `0x4A4A6A` |
| 分布半径 | 68px |
| 角度间隔 | 45 度 |
| 起始角度 | -90 度 (12 点钟) |

### 6.4 SVG 进度弧

**背景弧 (270 度)**:

```
<path d="M... A 68 68 0 1 1 ..."
    fill="none"
    stroke="rgba(163,213,255,0.06)"
    stroke-width="2.5"
    stroke-linecap="round" />
```

**进度弧 (随温度变化)**:

```
<path d="M... A 68 68 0 1 1 ..."
    fill="none"
    stroke="url(#tempGradient)"
    stroke-width="3"
    stroke-linecap="round"
    filter="drop-shadow(0 0 4px gradColor)" />

<linearGradient id="tempGradient">
    <stop offset="0%" stop-color="#A3D5FF" />
    <stop offset="50%" stop-color="#39FF14" />
    <stop offset="100%" stop-color="#FFD700" />
</linearGradient>
```

| 参数 | 值 |
|------|-----|
| 圆心 (cx, cy) | (120, 120) |
| 半径 (r) | 68 |
| 起始角度 | -135 度 (7 点钟) |
| 终止角度 | -135 + tempPercent * 270 度 |
| 背景弧颜色 | `rgba(163,213,255,0.06)` |
| 进度弧颜色 | `url(#tempGradient)` |
| 进度弧宽度 | 3px |
| 背景弧宽度 | 2.5px |

### 6.5 中心温度显示

**中心圆背景**:

```c
lv_obj_t *center = lv_obj_create(dial);
lv_obj_set_size(center, 84, 84);
lv_obj_center(center);
lv_obj_set_style_radius(center, LV_RADIUS_CIRCLE, 0);
lv_obj_set_style_bg_color(center, lv_color_hex(0x0D0D18), 0);
lv_obj_set_style_bg_opa(center, LV_OPA_90, 0);
lv_obj_set_style_border_color(center, lv_color_hex(cultivation_color), 0);
lv_obj_set_style_border_width(center, 1, 0);
lv_obj_set_style_border_opa(center, LV_OPA_15, 0);
lv_obj_set_style_shadow_color(center, lv_color_hex(cultivation_color), 0);
lv_obj_set_style_shadow_width(center, 15, 0);
lv_obj_set_style_shadow_opa(center, LV_OPA_10, 0);
```

| 属性 | 值 |
|------|-----|
| 尺寸 | 84 x 84 |
| 形状 | 正圆 |
| 背景色 | `0x0D0D18` |
| 背景透明度 | LV_OPA_90 |
| 边框色 | cultivation_color (随温度变) |
| 边框宽 | 1px |
| 边框透明度 | LV_OPA_15 |
| 阴影色 | cultivation_color |
| 阴影宽 | 15px |

**温度数字 (36px)**:

```c
lv_obj_t *temp_lbl = lv_label_create(center);
lv_label_set_text(temp_lbl, "24");
lv_obj_set_style_text_font(temp_lbl, &lv_font_montserrat_36, 0);
lv_obj_set_style_text_color(temp_lbl, lv_color_hex(cultivation_color), 0);
lv_obj_center(temp_lbl);
```

| 属性 | 值 |
|------|-----|
| 字体 | `lv_font_montserrat_36` |
| 颜色 | cultivation_color (渐变) |
| 对齐 | 居中 |
| 文字阴影 | `0 0 12px cultivation_color` |

**小数位 + 度符号 (10px)**:

```c
lv_obj_t *unit_lbl = lv_label_create(center);
lv_label_set_text(unit_lbl, ".0°");
lv_obj_set_style_text_font(unit_lbl, &lv_font_montserrat_10, 0);
lv_obj_set_style_text_color(unit_lbl, lv_color_hex(0xA3D5FF80), 0);
lv_obj_align_to(unit_lbl, temp_lbl, LV_ALIGN_OUT_RIGHT_TOP, 2, 2);
```

| 属性 | 值 |
|------|-----|
| 文本 | ".0°" |
| 字体 | `lv_font_montserrat_10` |
| 颜色 | `0xA3D5FF80` (半透明冰蓝) |
| 对齐 | 温度数字右上角偏移 (2, 2) |

**修仙境界文字 (9px)**:

```c
lv_obj_t *realm_lbl = lv_label_create(center);
lv_label_set_text(realm_lbl, "元婴期");
lv_obj_set_style_text_font(realm_lbl, &lv_font_montserrat_8, 0);  /* 实际用 9px */
lv_obj_set_style_text_color(realm_lbl, lv_color_hex(cultivation_color), 0);
lv_obj_set_style_text_letter_space(realm_lbl, 2, 0);
lv_obj_align(realm_lbl, LV_ALIGN_BOTTOM_MID, 0, -8);
```

### 6.6 旋钮指示器 (Knob)

```c
lv_obj_t *knob = lv_obj_create(dial);
lv_obj_set_size(knob, 6, 6);
lv_obj_set_style_radius(knob, LV_RADIUS_CIRCLE, 0);
lv_obj_set_style_bg_color(knob, lv_color_hex(grad_color), 0);
lv_obj_set_style_shadow_color(knob, lv_color_hex(grad_color), 0);
lv_obj_set_style_shadow_width(knob, 6, 0);
lv_obj_set_style_border_width(knob, 0, 0);

/* 弧线上的位置 */
float a = (arcEnd - 90) * PI / 180;
lv_obj_set_pos(knob, 120 + 68 * cos(a) - 3, 120 + 68 * sin(a) - 3);
```

| 属性 | 值 |
|------|-----|
| 尺寸 | 6 x 6 |
| 形状 | 正圆 |
| 背景色 | grad_color (温度渐变) |
| 阴影色 | grad_color |
| 阴影宽 | 6px |
| 位置 | 弧线终点上 (r=68) |

### 6.7 温度范围参数

| 参数 | 值 | 说明 |
|------|-----|------|
| `TIANJI_TEMP_MIN` | 160 | 16.0 C (tenths) |
| `TIANJI_TEMP_MAX` | 300 | 30.0 C (tenths) |
| `TIANJI_TEMP_DEFAULT` | 240 | 24.0 C (tenths) |
| `tempPercent` | 0.0~1.0 | `(temp - 160) / (300 - 160)` |
| `dialRotation` | -135~+135 | `tempPercent * 270 - 135` |

---

## 七、修仙境界指示条 (Cultivation Bar)

**区域**: y:210~232, h:22px | **容器**: `lv_obj_t` | **Z**: 10

### 7.1 容器

```c
lv_obj_t *realm_bar = lv_obj_create(lv_scr_act());
lv_obj_set_pos(realm_bar, 0, 210);
lv_obj_set_size(realm_bar, 240, 22);
lv_obj_set_style_bg_opa(realm_bar, LV_OPA_TRANSP, 0);
lv_obj_set_style_border_width(realm_bar, 0, 0);
lv_obj_set_style_pad_all(realm_bar, 2, 0);
```

### 7.2 7 个境界状态点 + 标签

```c
const char *realms[7] = { "炼气","筑基","金丹","元婴","化神","练虚","合体" };
uint16_t realm_colors[7] = { 0x9EBF, 0x5FFF, 0x07E0, 0x57E0, 0xFE00, 0xFD20, 0xFA20 };
int start_x = 8, spacing = 32;

for (int i = 0; i < 7; i++) {
    bool is_active = (current_temp >= realm_levels[i].min && 
                      current_temp < realm_levels[i].max);
    
    /* 状态点 */
    lv_obj_t *dot = lv_obj_create(realm_bar);
    lv_obj_set_size(dot, 2, is_active ? 14 : 5);
    lv_obj_set_pos(dot, start_x + i * spacing + 8, 2);
    lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(dot, 
        lv_color_hex(is_active ? realm_colors[i] : 0x2A2A4A), 0);
    if (is_active) {
        lv_obj_set_style_shadow_color(dot, lv_color_hex(realm_colors[i]), 0);
        lv_obj_set_style_shadow_width(dot, 4, 0);
    }
    
    /* 标签 */
    lv_obj_t *lbl = lv_label_create(realm_bar);
    lv_label_set_text(lbl, realms[i]);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_8, 0);
    lv_obj_set_style_text_color(lbl,
        lv_color_hex(is_active ? realm_colors[i] : 0x3A3A5A), 0);
    lv_obj_set_pos(lbl, start_x + i * spacing, 10);
}
```

| 属性 | 激活态 | 非激活态 |
|------|--------|----------|
| 点高度 | 14px | 5px |
| 点颜色 | realm_colors[i] | `0x2A2A4A` |
| 点阴影 | 4px + 对应色 | 无 |
| 标签颜色 | realm_colors[i] | `0x3A3A5A` |
| 标签字体 | `lv_font_montserrat_8` | `lv_font_montserrat_8` |

---

## 八、语音交互核心 (Voice Core)

**区域**: y:232~270, h:38px | **容器**: `lv_obj_t` | **Z**: 10

### 8.1 轨道环 (Orbital Rings)

```c
/* Ring 1 - 外层 */
lv_obj_t *ring1 = lv_obj_create(parent);
lv_obj_set_size(ring1, 70, 70);
lv_obj_center(ring1);
lv_obj_set_style_radius(ring1, LV_RADIUS_CIRCLE, 0);
lv_obj_set_style_bg_opa(ring1, LV_OPA_TRANSP, 0);
lv_obj_set_style_border_color(ring1, lv_color_hex(0x1A1A3E), 0);
lv_obj_set_style_border_width(ring1, 1, 0);
lv_obj_set_style_border_opa(ring1, LV_OPA_50, 0);
lv_obj_set_style_transform_width(ring1, -23, 0);  /* scaleX(0.55) */
lv_obj_clear_flag(ring1, LV_OBJ_FLAG_CLICKABLE);

/* Ring 2 - 内层 (反向旋转) */
lv_obj_t *ring2 = lv_obj_create(parent);
lv_obj_set_size(ring2, 52, 52);
lv_obj_center(ring2);
lv_obj_set_style_radius(ring2, LV_RADIUS_CIRCLE, 0);
lv_obj_set_style_bg_opa(ring2, LV_OPA_TRANSP, 0);
lv_obj_set_style_border_color(ring2, lv_color_hex(0x1A1A3E), 0);
lv_obj_set_style_border_width(ring2, 1, 0);
lv_obj_set_style_border_opa(ring2, LV_OPA_30, 0);
lv_obj_set_style_transform_width(ring2, -17, 0);
lv_obj_clear_flag(ring2, LV_OBJ_FLAG_CLICKABLE);
```

| 属性 | Ring 1 (外) | Ring 2 (内) |
|------|-------------|-------------|
| 尺寸 | 70 x 70 | 52 x 52 |
| 边框色 | `0x1A1A3E` | `0x1A1A3E` |
| 边框透明度 | LV_OPA_50 | LV_OPA_30 |
| X 缩放 | 0.55 | 0.55 |
| 动画 | 50s 正转 | 40s 反转 |

### 8.2 灵珠按钮 (Voice Orb)

```c
lv_obj_t *orb_btn = lv_btn_create(parent);
lv_obj_set_size(orb_btn, 28, 28);
lv_obj_center(orb_btn);
lv_obj_set_style_radius(orb_btn, LV_RADIUS_CIRCLE, 0);

/* 背景 */
lv_obj_set_style_bg_color(orb_btn, 
    is_active ? lv_color_hex(0x0A1A0A) : lv_color_hex(0x0A0A12), 0);
lv_obj_set_style_bg_opa(orb_btn, LV_OPA_COVER, 0);

/* 边框 */
lv_obj_set_style_border_color(orb_btn,
    is_active ? lv_color_hex(0x39FF1480) : lv_color_hex(0xA3D5FF30), 0);
lv_obj_set_style_border_width(orb_btn, 1, 0);

/* 阴影 */
lv_obj_set_style_shadow_color(orb_btn,
    is_active ? lv_color_hex(0x39FF14) : lv_color_hex(0xA3D5FF), 0);
lv_obj_set_style_shadow_width(orb_btn, is_active ? 12 : 8, 0);
lv_obj_set_style_shadow_opa(orb_btn,
    is_active ? LV_OPA_40 : LV_OPA_10, 0);

/* 内部发光球 */
lv_obj_t *orb = lv_obj_create(orb_btn);
lv_obj_set_size(orb, 10, 10);
lv_obj_center(orb);
lv_obj_set_style_radius(orb, LV_RADIUS_CIRCLE, 0);
lv_obj_set_style_bg_color(orb,
    is_active ? lv_color_hex(0x39FF14) : lv_color_hex(0xA3D5FF), 0);
lv_obj_set_style_bg_opa(orb, LV_OPA_70, 0);
```

| 属性 | 激活态 (LISTENING) | 待机态 (IDLE) |
|------|-------------------|---------------|
| 背景色 | `0x0A1A0A` | `0x0A0A12` |
| 边框色 | `0x39FF1480` | `0xA3D5FF30` |
| 阴影色 | `#39FF14` | `#A3D5FF` |
| 阴影宽 | 12px | 8px |
| 内球颜色 | `#39FF14` | `#A3D5FF` |
| 动画 | voice-wave 0.8s | pulse-orb 3s |

### 8.3 状态文字

**主状态文字**:

```c
lv_obj_t *status_lbl = lv_label_create(parent);
lv_label_set_text(status_lbl, "气聚神凝");
lv_obj_set_style_text_font(status_lbl, &lv_font_montserrat_14, 0);
lv_obj_set_style_text_color(status_lbl,
    is_active ? lv_color_hex(0x39FF14) : lv_color_hex(0xA3D5FF), 0);
lv_obj_set_style_text_letter_space(status_lbl, 2, 0);
```

**提示文字**:

```c
lv_obj_t *hint_lbl = lv_label_create(parent);
lv_label_set_text(hint_lbl, is_active ? "识别中..." : "点击唤醒");
lv_obj_set_style_text_font(hint_lbl, &lv_font_montserrat_8, 0);
lv_obj_set_style_text_color(hint_lbl, lv_color_hex(0x4A4A6A), 0);
```

### 8.4 状态机

| 状态 | 触发条件 | 灵珠颜色 | 显示文字 | 动画 |
|------|----------|----------|----------|------|
| `IDLE` | 初始/完成 | #A3D5FF | 气聚神凝 | pulse-orb 3s ease |
| `LISTENING` | 点击灵珠 | #39FF14 | 聆听中... | voice-wave 0.8s |
| `PROCESSING` | 语音识别后 | #FFD700 | 处理中... | 环加速旋转 |
| `RESPONDING` | 命令执行后 | #39FF14 | 风随心动 | 常亮 |

---

## 九、底部控制按钮 (Bottom Controls)

**区域**: y:270~308, h:38px | **容器**: `lv_obj_t` | **Z**: 10

### 9.1 容器

```c
lv_obj_t *bottom_bar = lv_obj_create(lv_scr_act());
lv_obj_set_pos(bottom_bar, 0, 270);
lv_obj_set_size(bottom_bar, 240, 38);
lv_obj_set_style_bg_opa(bottom_bar, LV_OPA_TRANSP, 0);
lv_obj_set_style_border_width(bottom_bar, 0, 0);
lv_obj_set_flex_flow(bottom_bar, LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(bottom_bar, 
    LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_set_style_pad_gap(bottom_bar, 6, 0);
```

### 9.2 霓虹按钮模板

```c
lv_obj_t *create_neon_btn(lv_obj_t *parent, const char *label, 
                           uint16_t color, bool active) {
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 68, 32);
    lv_obj_set_style_radius(btn, 10, 0);
    
    /* 背景 */
    lv_obj_set_style_bg_color(btn, 
        active ? lv_color_hex(color) : lv_color_hex(0x0A0A12), 0);
    lv_obj_set_style_bg_opa(btn, 
        active ? LV_OPA_12 : LV_OPA_50, 0);
    
    /* 边框 */
    lv_obj_set_style_border_color(btn, 
        active ? lv_color_hex(color) : lv_color_hex(0xA3D5FF30), 0);
    lv_obj_set_style_border_width(btn, 1, 0);
    lv_obj_set_style_border_opa(btn, 
        active ? LV_OPA_40 : LV_OPA_15, 0);
    
    /* 阴影 (仅激活态) */
    if (active) {
        lv_obj_set_style_shadow_color(btn, lv_color_hex(color), 0);
        lv_obj_set_style_shadow_width(btn, 10, 0);
        lv_obj_set_style_shadow_opa(btn, LV_OPA_15, 0);
    }
    
    /* 标签 */
    lv_obj_t *lbl = lv_label_create(btn);
    lv_label_set_text(lbl, label);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(lbl, 
        active ? lv_color_hex(color) : lv_color_hex(0xA3D5FF80), 0);
    lv_obj_center(lbl);
    
    return btn;
}
```

| 属性 | 激活态 | 非激活态 |
|------|--------|----------|
| 背景色 | color (12% 透明度) | `0x0A0A12` (50% 透明度) |
| 边框色 | color (40% 透明度) | `0xA3D5FF30` (15% 透明度) |
| 阴影 | 10px + color | 无 |
| 文字色 | color | `0xA3D5FF80` |

### 9.3 三个按钮实例

```c
/* 模式按钮 */
lv_obj_t *btn_mode = create_neon_btn(bottom_bar, "修仙", 0x07E0, true);

/* 风速按钮 */
lv_obj_t *btn_wind = create_neon_btn(bottom_bar, "巽风", 0x07E0, true);

/* 定时按钮 */
lv_obj_t *btn_timer = create_neon_btn(bottom_bar, "甲子", 0x07E0, true);
```

| 按钮 | 文本 | 激活色 | 未激活色 | 功能 |
|------|------|--------|----------|------|
| 模式 | 修仙/凡尘 | #39FF14 | #A3D5FF80 | 切换运行模式 |
| 风速 | 巽风/离火 | #39FF14 | #FF450080 | 切换风速档位 |
| 定时 | 甲子/庚申 | #39FF14 | #A3D5FF80 | 切换定时模式 |

---

## 十、节气跑马灯 (Marquee)

**区域**: y:308~320, h:12px | **元素**: `lv_label_t` | **Z**: 10

```c
lv_obj_t *marquee = lv_label_create(lv_scr_act());
lv_label_set_text(marquee, 
    "立春 · 雨水 · 惊蛰 · 春分 · 清明 · 谷雨 · "
    "立夏 · 小满 · 芒种 · 夏至 · 小暑 · 大暑 · "
    "立秋 · 处暑 · 白露 · 秋分 · 寒露 · 霜降 · "
    "立冬 · 小雪 · 大雪 · 冬至 · 小寒 · 大寒 · ");

lv_obj_set_style_text_font(marquee, &lv_font_montserrat_8, 0);
lv_obj_set_style_text_color(marquee, lv_color_hex(0xA3D5FF), 0);
lv_obj_set_style_text_opa(marquee, LV_OPA_12, 0);
lv_obj_set_pos(marquee, 0, 308);

/* 滚动动画 */
lv_anim_t a;
lv_anim_init(&a);
lv_anim_set_var(&a, marquee);
lv_anim_set_values(&a, 0, -400);
lv_anim_set_time(&a, 20000);  /* 20秒一轮 */
lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
lv_anim_set_path_cb(&a, lv_anim_path_linear);
lv_anim_start(&a);
```

| 属性 | 值 |
|------|-----|
| 文本 | 24 节气完整名称 + 分隔符 |
| 字体 | `lv_font_montserrat_8` |
| 颜色 | `0x9EBF` (#A3D5FF) |
| 透明度 | LV_OPA_12 (约 5%) |
| 动画方向 | 左移 (-400px) |
| 动画时长 | 20s |
| 重复 | 无限循环 |
| 路径 | 线性 (linear) |

---

## 十一、事件处理与回调

### 11.1 温控罗盘拖拽事件

```c
static void dial_drag_cb(lv_event_t *e) {
    tj_dial_t *d = lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_PRESSED) {
        d->dragging = true;
        lv_indev_t *indev = lv_indev_get_act();
        lv_indev_get_vect(indev, &d->drag_start);
        d->temp_at_drag = d->temp;
    }
    else if (code == LV_EVENT_PRESSING && d->dragging) {
        lv_point_t vect;
        lv_indev_get_vect(lv_indev_get_act(), &vect);
        
        /* 灵敏度: 水平x0.3 + 垂直反向x0.2 */
        int16_t delta = vect.x * 0.3f - vect.y * 0.2f;
        int16_t new_temp = d->temp_at_drag + delta;
        
        /* 限制范围 */
        if (new_temp < 160) new_temp = 160;
        if (new_temp > 300) new_temp = 300;
        
        d->temp = new_temp;
        tj_dial_refresh(d);  /* 更新UI */
        tj_particles_set_speed(particles, 
            1 + (new_temp - 160) / 14);  /* 更新粒子速度 */
    }
    else if (code == LV_EVENT_RELEASED) {
        d->dragging = false;
    }
}
```

| 事件 | 处理 |
|------|------|
| `LV_EVENT_PRESSED` | 记录拖拽起始点和当前温度 |
| `LV_EVENT_PRESSING` | 计算位移delta，更新温度，刷新UI和粒子速度 |
| `LV_EVENT_RELEASED` | 结束拖拽状态 |

### 11.2 语音唤醒回调

```c
static void voice_activated(void *user_data) {
    tj_voice_t *vc = (tj_voice_t *)user_data;
    
    /* 切换到聆听状态 */
    tj_voice_set_state(vc, TJ_VOICE_LISTENING);
    tj_voice_set_text(vc, "聆听中...");
    
    /* 3秒后模拟处理完成 */
    lv_timer_t *timer = lv_timer_create(voice_result_cb, 3000, vc);
    lv_timer_set_repeat_count(timer, 1);
}

static void voice_result_cb(lv_timer_t *timer) {
    tj_voice_t *vc = timer->user_data;
    tj_voice_set_state(vc, TJ_VOICE_IDLE);
    tj_voice_set_text(vc, "气聚神凝");
    lv_timer_del(timer);
}
```

### 11.3 按钮点击回调

```c
static void btn_mode_cb(lv_event_t *e) {
    tianji_ui_t *ui = lv_event_get_user_data(e);
    ui->mode_idx = !ui->mode_idx;  /* 翻转状态 */
    
    const char *labels[2] = { "修仙", "凡尘" };
    uint32_t colors[2] = { 0x07E0, 0x9EBF };
    
    /* 更新按钮文字 */
    lv_obj_t *lbl = lv_obj_get_child(ui->btn_mode, 0);
    lv_label_set_text(lbl, labels[ui->mode_idx]);
    
    /* 更新颜色 */
    lv_obj_set_style_text_color(lbl, 
        lv_color_hex(colors[ui->mode_idx]), 0);
    lv_obj_set_style_border_color(ui->btn_mode,
        lv_color_hex(colors[ui->mode_idx]), 0);
}
```

---

## 附录 A：完整初始化模板

```c
#include "lvgl.h"
#include "tianji_data.h"
#include "tianji_ui.h"

void tianji_ui_init(void) {
    /* 1. 设置屏幕背景 */
    lv_obj_set_style_bg_color(lv_scr_act(),
        lv_color_hex(TIANJI_COLOR_BG), 0);
    
    /* 2. 创建状态栏 */
    create_status_bar();
    
    /* 3. 创建粒子画布并启动 */
    create_particle_canvas();
    tj_particles_start(particle_helix);
    
    /* 4. 创建温控罗盘 */
    create_thermostat_dial();
    tj_dial_set_temp(dial, 240);  /* 默认 24.0C */
    
    /* 5. 创建修仙境界指示条 */
    create_realm_bar();
    
    /* 6. 创建语音交互核心 */
    create_voice_core();
    
    /* 7. 创建底部控制按钮 */
    create_bottom_controls();
    
    /* 8. 创建节气跑马灯 */
    create_marquee();
}

/* 主循环 */
while (1) {
    lv_timer_handler();  /* LVGL 任务处理 */
    HAL_Delay(5);        /* 5ms 间隔 */
}
```

---

## 附录 B：资源占用

| 资源 | 大小 | 说明 |
|------|------|------|
| LVGL 核心 | ~80KB Flash | 含动画/绘图引擎 |
| 字体文件 | ~60KB Flash | Montserrat 8/10/12/14/36 |
| 粒子画布缓冲 | 51.2KB RAM | 160x160x2 字节 |
| DMA 双缓冲 | 150KB SRAM | 2 x (240x160x2) 字节 |
| UI 代码+数据 | ~30KB Flash | 全部组件实现 |
| **总计** | **~170KB Flash + ~32KB RAM** | |

---

*文档版本: 1.0 | 生成日期: 2025年 | 适配: LVGL v8.3 + STM32CubeF4 HAL*
