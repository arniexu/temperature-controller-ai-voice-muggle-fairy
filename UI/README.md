# 关于 UI 文件夹

从设备角度来看，这个文件夹和项目 build 无关。但从中控机的角度看，他可以和 stm32 通信，并给出更好看的 UI 去操控，所以这个文件夹本质上是 STM32 的桥接中控程序。开发迁移到了这里专门实现 UI 功能 [desktop-temperature-controller](https://github.com/arniexu/desktop-temperature-controller)。

---

## HIL 测试体系

本项目的 HIL (Hardware-in-the-Loop) 测试从 Level 1 到 Level 5 逐步递进。详见 [HIL/README.md](../HIL/README.md)。

### Level 1: PC 模拟显示 (无需硬件)

| 方案 | 实现 | 状态 |
|:-----|:-----|:-----|
| **Level 1a: Python 截图模拟** | `HIL/lvgl_sim_display.py` — tkinter Canvas + PNG 素材贴图 + pyautogui 断言 | ✅ 已完成, CI 就绪 |
| **Level 1b: 原生 C SDL2 模拟器** | `PC_Simulator/` — LVGL 真渲染, RGB565 flush_cb → SDL Texture, 鼠标转触摸 | ⬜ 计划中 |

Level 1b 方案直接将 STM32 固件中的 `tianji_*` C 代码编译为 Windows 原生 exe，通过 SDL2 后端渲染 LVGL UI。优势：

- **真 UI 渲染**：与真机跑完全相同的 LVGL 对象树和绘制管线
- **无需截图**：pyautogui 直接读取 LVGL 对象属性 (label 文本、arc 值、页面 ID)
- **交互可靠**：SDL 鼠标事件 → LVGL indev，触摸交互逻辑与真机一致

### Level 2-5

| Level | 说明 | 状态 |
|:------|:-----|:-----|
| Level 2 | 串口遥测 + Web 仪表盘 (`web_dashboard.py`) | ✅ 已完成 |
| Level 3 | pyOCD SWD 内存注入 (`hil_harness.py`) | ✅ 已完成 |
| Level 4 | 真机触摸 + 温度设定回归 | ⬜ 计划中 |
| Level 5 | 全闭环 HIL (传感器模拟 + 特性化) | ⬜ 计划中 |

---

## 参考资料

- [What is Hardware-in-the-Loop (HIL) Testing](https://www.qt.io/what-is-hardware-in-the-loop-testing)
- [Hardware in the Loop Testing](https://www.vector.com/us/en/products/testing-solutions/hardware-in-the-loop-testing/)
- [HIL Testing for Battery Management Systems (BMS)](https://www.typhoon-hil.com/products/hil-testing-solutions/hil-testing-for-bms)
- [LVGL PC Simulator 官方文档](https://docs.lvgl.io/8.3/porting/pc-simulator.html)
- [lv_port_pc_eclipse — LVGL SDL2 模拟器参考](https://github.com/lvgl/lv_port_pc_eclipse)