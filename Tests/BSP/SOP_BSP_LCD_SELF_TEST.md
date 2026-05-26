# BSP/LCD 自检 SOP

## 1. 目的

用于验证 STM32F407 板卡上的 LCD 硬件链路与 BSP 驱动是否正常，覆盖以下关键路径：

- GPIO + FSMC 初始化
- ILI9341 初始化
- LCD 清屏/填充/字符串显示
- 背光控制与 DisplayOn/DisplayOff 切换

本 SOP 对应当前工程中的独立测试入口 `Tests/BSP/main_bsp_test.c`。

## 2. 前置条件

- 硬件：目标板、LCD 屏、下载器（ST-Link/J-Link）
- 软件：Keil uVision5（UV4）
  - 已知路径：`C:\Users\xuqianjx\AppData\Local\Keil_v5\UV4\UV4.exe`
- 工程文件：`MDK-ARM/ai_tempconroler_bsp_test.uvprojx`
- 连接正确：LCD 数据/控制线与 FSMC/背光引脚匹配当前硬件连线

## 3. 测试入口说明

测试目标与入口：

- Target：`bsp_test`（在 `MDK-ARM/ai_tempconroler_bsp_test.uvprojx`）
- 入口文件：`Tests/BSP/main_bsp_test.c`

入口行为摘要：

1. `HAL_Init()` + `SystemClock_Config()`（HSE + PLL 到 168MHz）
2. `MX_GPIO_Init()` + `MX_FSMC_Init()`
3. 拉高背光引脚 `LCD_BL_Pin`
4. 调用 `BSP_LCD_Init()`
5. 依次全屏清为黑/红/绿/蓝（每步延时 200ms）
6. 黑底绘制测试文字 `BSP TEST: LCD OK`
7. 死循环中持续执行 DisplayOn/DisplayOff + 背光开关（500ms/200ms）

## 4. 执行步骤

1. 打开 Keil 工程 `MDK-ARM/ai_tempconroler_bsp_test.uvprojx`。
2. 在 Target 下拉框中确认选择 `bsp_test`。
3. 先执行 Rebuild（或 Build）确保无编译/链接错误。
4. 连接下载器并确认调试器可识别目标芯片。
5. 点击 Download/Load 将固件烧录到板卡。
6. 复位后观察 LCD 显示现象并记录结果。

## 5. 通过标准（Pass Criteria）

满足以下全部条件判定通过：

1. 上电/复位后可看到黑、红、绿、蓝四色切换。
2. 可显示字符串 `BSP TEST: LCD OK`。
3. 屏幕与背光按周期闪烁（亮屏 + 熄屏交替）。
4. 运行过程中无卡死、无异常花屏、无明显撕裂。

## 6. 失败判定与快速排查

### 6.1 白屏/黑屏无任何变化

- 检查是否真的下载的是 `bsp_test` 目标产物。
- 检查 `MX_FSMC_Init()` 是否执行，LCD 是否挂在正确 FSMC Bank。
- 检查背光引脚电平是否变化（示波器或万用表）。

### 6.2 能亮但颜色异常/显示错位

- 检查 FSMC 时序配置（读写时序、扩展模式）。
- 检查 ILI9341 初始化序列与屏幕方向配置。
- 检查命令/数据地址线（A16）映射是否正确。

### 6.3 周期闪烁不符合预期

- 检查 `HAL_Delay()` 基准是否正确（系统时钟配置）。
- 检查 `BSP_LCD_DisplayOn/Off()` 与背光 GPIO 是否同时生效。

## 7. 测试记录模板

建议每次执行保留一条记录，便于回归对比：

- 日期时间：
- 板卡版本：
- LCD 型号/批次：
- 固件分支/提交：
- 构建结果：通过 / 失败
- 现象描述：
- 结论：PASS / FAIL
- 备注（修复动作/后续计划）：

## 8. 回归建议

以下变更后建议重新跑本 SOP：

- `Core/Src/fsmc.c`
- `Core/Src/gpio.c`
- `Drivers/BSP/Components/ili9341/*`
- `Drivers/BSP/Adafruit_Shield/stm32_adafruit_lcd.c`
- `Tests/BSP/main_bsp_test.c`
