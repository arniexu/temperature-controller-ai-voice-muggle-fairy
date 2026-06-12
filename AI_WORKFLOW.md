# AI 辅助嵌入式开发工作流

> 本项目约 40% 的代码由 AI 辅助生成。本文档记录我使用的 AI 工具链、架构设计思路、典型工作流和经验教训。如果你也想把 AI 接入自己的嵌入式开发流程，本文可作为起点。

---

## 目录

- [动机：为什么要用 AI 写 STM32](#动机为什么要用-ai-写-stm32)
- [工具链总览](#工具链总览)
- [四层 AI 架构](#四层-ai-架构)
- [典型工作流](#典型工作流)
- [实战案例](#实战案例)
- [搭建你自己的环境](#搭建你自己的环境)
- [经验教训](#经验教训)
- [FAQ](#faq)

---

## 动机：为什么要用 AI 写 STM32

传统 STM32 开发的痛点与 AI 解决方案：

| 痛点 | 传统方式 | AI 辅助 |
|------|----------|---------|
| **外设驱动迁移** | 查手册 → 对照寄存器 → 手写配置 → 调试 2-3 天 | AI 从 SVD 文件生成寄存器映射 + 配置代码，人工验证即可 |
| **多工具链适配** | Keil / GCC / IAR 各写一遍链接脚本、启动文件 | AI 生成 CMakeLists、链接脚本、startup，支持多编译器 |
| **驱动测试** | 手动编写测试用例，覆盖不全 | AI 生成 27 个 BSP 冒烟测试函数，逐个验证 |
| **Bug 定位** | 看寄存器 dump、猜测、反复烧录 | AI 分析 ELF / 编译日志 → 定性定位，减少盲猜 |
| **重复代码** | 手写大量结构体映射、回调注册 | AI 完成 80% 的模板代码，人只写核心逻辑 |
| **文档注释** | 事后补、敷衍、过时 | AI 边写边生成，与代码同步 |

**本质：** AI 不是替代你的判断力，而是把"从想法到代码"的机械劳动压缩到秒级。你花 20% 时间做架构设计和最终验证，剩下 80% 的搬运活交给 AI。

---

## 工具链总览

| 工具 | 类型 | 作用 | 是否必须 | 获取方式 |
|------|------|------|----------|----------|
| **GitHub Copilot / Cursor** | IDE 插件 | 代码补全、函数生成、重构建议 | 🟢 核心 | Cursor IDE 内置 / Copilot 订阅 |
| **Continuity** | 项目记忆 | 自动记录每个设计决策，下次 AI 会话自动注入上下文 | 🟡 推荐 | `npm i -g continuity-cli` |
| **KeilBridge** | 工程诊断 | 解析 `.uvprojx`，提取源文件、宏定义、编译选项供 AI 分析 | 🟡 推荐 | 开源工具 |
| **Embedder** | 上下文注入 | 将 SVD 外设寄存器描述注入 AI 上下文，让 AI 认识具体芯片 | 🟡 推荐 | 开源工具 |
| **VS Code + C/C++ 插件** | 编辑器 | IntelliSense、代码跳转、错误提示（不改 Keil 编译流程，只看代码用） | 🟡 推荐 | VS Code 免费 |

> **关系：** Copilot/Cursor 是"大脑"，Continuity 是"长期记忆"，KeilBridge 是"工程体检报告"，Embedder 是"芯片说明书翻译官"。四者配合才能发挥最大效力。

---

## 四层 AI 架构

每个 AI 工具在本项目中扮演不同角色：

```
┌─────────────────────────────────────────────────────────┐
│  L1 · 代码生成层                                         │
│  GitHub Copilot / Cursor                                │
│  – 函数级代码补全、多文件重构、测试生成                  │
│  – 输出：驱动桩、LVGL 控件、CMake 脚本                   │
├─────────────────────────────────────────────────────────┤
│  L2 · 项目记忆层                                         │
│  Continuity                                             │
│  – 自动捕获每次"为什么这样设计"的设计决策（34 条记录）    │
│  – 每次新 AI 会话自动注入项目上下文 + 近期决策 + 工作树状态 │
│  – 输出：decisions.json、SESSION_NOTES.md                │
├─────────────────────────────────────────────────────────┤
│  L3 · 工程诊断层                                         │
│  KeilBridge                                              │
│  – 解析 .uvprojx → 提取源文件列表、宏定义、Include 路径  │
│  – ELF 分析：段大小、符号表、内存布局                    │
│  – 输出：结构化诊断报告（JSON + Markdown）                │
├─────────────────────────────────────────────────────────┤
│  L4 · 芯片知识层                                         │
│  Embedder + SVD                                          │
│  – 将 STM32F407 的 SVD 文件（寄存器映射）注入 AI 上下文   │
│  – AI 看到 `FSMC_Bank1->BTCR[4]` 就知道这是什么寄存器     │
│  – 输出：精准的外设寄存器操作代码（零手册查表）           │
└─────────────────────────────────────────────────────────┘
```

**举例：** 当 AI 需要生成 FSMC 8080 并口配置代码时——
1. **L4** Embedder 把 SVD 中的 `FSMC_Bank1E->BTCR` 寄存器定义注入
2. **L3** KeilBridge 把当前工程的宏定义（`DATA_IN_16BIT_MODE` 等）告知 AI
3. **L2** Continuity 把之前关于"为什么选 FSMC 而非 SPI"的决策告知 AI
4. **L1** Copilot 基于以上上下文生成精准的配置代码

---

## 典型工作流

### 新增功能的标准流程

```
1. 我在脑海里构思功能需求（5 分钟）
        ↓
2. 用自然语言描述给 AI："在 UI 层新增一个旋钮控件，用 LVGL Arc..."（2 分钟）
        ↓
3. AI 生成初始代码（30 秒）
        ↓
4. 我在 Keil 中编译（5 分钟）
        ↓
5. 如果有编译错误，把错误日志复制给 AI（1 分钟）
        ↓
6. AI 分析错误并给出修复（30 秒）
        ↓
7. 烧录到板子上验证（5 分钟）
        ↓
8. 如果有 Bug，描述现象给 AI，生成修复（5 分钟）
        ↓
9. 跑通后，用 Continuity 记录关键决策（1 分钟）
        ↓
10. 更新 SESSION_NOTES.md（1 分钟）
```

**总计：约 25 分钟完成一个新控件，** 传统手写可能需要 2-3 小时。

### 跨工具链迁移的真实流程

本项目最初只有 Keil 工程，后来需要支持 CMake + GCC 工具链。传统迁移需要：
1. 手写 CMakeLists.txt
2. 手写 GCC 链接脚本
3. 处理 startup 文件差异
4. 调试编译选项

**AI 辅助流程：**
1. 把 `ai_tempconroler.uvprojx` 通过 KeilBridge 解析为结构化数据
2. 把源文件列表、Include 路径、宏定义传给 AI
3. AI 生成 `CMakeLists.txt` + `STM32F407ZGTX_FLASH.ld` + `cmake/armclang-toolchain.cmake`
4. 人工验证 3 个编译错误 → AI 修复 → 通过
5. **耗时：约 2 小时**（传统方式至少 1 天）

---

## 实战案例

### 案例 1：LVGL 集成 — 从 0 到 Smoke Test

**背景：** 第一次使用 LVGL，需要将它集成到 FreeRTOS 工程中。

**AI 做了什么：**
1. 生成 `lv_conf.h`（根据 240×320 ILI9341 配置）
2. 生成 `display_drv.c` + `touch_drv.c`（LVGL HAL 适配器）
3. 生成 `bsp_stubs.c`（BSP 抽象层，隔离硬件操作）
4. 生成 27 个测试函数（`Tests/BSP/` 下的 LCD/触摸冒烟测试）
5. 生成 `ui_main.c`（初始化流程 + lv_timer_handler 循环）

**我做了什么：**
1. 定义五层架构的分层设计
2. 在 Keil 中编译并分析每次编译错误
3. 把错误日志传给 AI 修复
4. 最终在硬件上验证跑到 Smoke Test 通过
5. 把所有设计决策记录到 Continuity

**关键决策：**
- 选择 FSMC 而非 SPI 驱动 LCD（因为全屏刷新 240×320 像素时 SPI 太慢）
- LVGL 行缓冲策略：20 行 × 240 像素 ≈ 9.4 KB（平衡内存与性能）
- 粒子动画使用查表法 sin/cos，零浮点运算（因为 F407 的 FPU 是单精度，LVGL 本身不需要浮点运算参与渲染）

### 案例 2：CMake 工具链迁移

**背景：** 项目原本只有 Keil 工程。为方便用 VS Code 开发的人，需要支持 CMake + GCC。

**AI 做了什么：**
1. 解析 `.uvprojx` 中的源文件列表（通过 KeilBridge）
2. 生成 `CMakeLists.txt` + `cmake/project-sources.cmake`
3. 生成 GCC 链接脚本 `STM32F407ZGTX_FLASH.ld`、`STM32F407ZGTX_RAM.ld`
4. 生成 ARMCLANG 工具链文件 `cmake/armclang-toolchain.cmake`

**我做了什么：**
1. 验证链接脚本的内存布局与芯片匹配
2. 解决 FreeRTOS + newlib 锁冲突（写了 `stm32_lock.h` + `newlib_lock_glue.c`）
3. 修正启动文件中的弱符号覆盖问题

### 案例 3：粒子特效 — 从图片变成 0 浮点动画

**背景：** 界面需要动态双螺旋粒子动画。最初试过逐像素画，但 MCU 算不动。

**AI 做了什么：**
1. 生成 360 度的 sin/cos 查表（纯整数，Q15.16 定点）
2. 生成粒子物理引擎（位置/颜色/大小更新逻辑）
3. 生成 LVGL 画布绑定代码

**我做了什么：**
1. 设计双螺旋的运动轨迹公式
2. 调试颜色渐变和运动参数
3. 优化内存占用（画布 ≈58 KB，放在外部 SRAM）

---

## 搭建你自己的环境

### 最小可用环境（只需 30 分钟）

| 步骤 | 操作 | 开销 |
|------|------|------|
| 1 | 安装 Cursor IDE（或用 VS Code + Copilot 插件） | 免费 |
| 2 | 安装 VS Code C/C++ 插件，配置 `c_cpp_properties.json` | 15 分钟 |
| 3 | 把项目的 `.vscode/settings.json` 中的 `compilerPath` 改为你的 armclang 路径 | 1 分钟 |
| 4 | 在 VS Code 中打开仓库，开始用 Copilot 写代码 | 即时 |

### 进阶：搭建完整四层架构

**L2 · Continuity（项目记忆）：**
```bash
npm i -g continuity-cli
cd your-project
continuity init
# 此后每次做架构决策后执行：
continuity log "为什么选方案A？" "因为... 对比方案B和C..."
```

**L3 · KeilBridge（工程诊断）：**
```bash
# 解析你的 .uvprojx 工程
keilbridge inspect MDK-ARM/your_project.uvprojx
# 生成结构化报告供 AI 分析
```

**L4 · Embedder（SVD 上下文注入）：**
```bash
# 下载你的 MCU 的 SVD 文件（通常从 Keil 安装目录或 CMSIS-SVD 仓库获取）
# Embedder 会自动缓存和注入
embedder cache STM32F407.svd
```

### VS Code 配置模板

参考本项目根目录的 `.vscode/settings.json`，最小配置：

```json
{
  "C_Cpp.default.includePath": [
    "Core/Inc",
    "Drivers/STM32F4xx_HAL_Driver/Inc",
    "Drivers/CMSIS/Include",
    "Middlewares/Third_Party/lvgl",
    "UI/Core/Inc",
    "UI/LVGL",
    "Drivers/BSP/Components/ili9341",
    "Drivers/BSP/Components/xpt2046",
    "FATFS/App",
    "FATFS/Target",
    "LIBJPEG/App",
    "LIBJPEG/Target",
    "Utilities/Fonts",
    "Utilities/Log"
  ],
  "C_Cpp.default.defines": [
    "DEBUG",
    "USE_HAL_DRIVER",
    "STM32F407xx",
    "LV_CONF_INCLUDE_SIMPLE"
  ],
  "C_Cpp.default.compilerPath": "<改为你的 armclang.exe 路径>",
  "C_Cpp.default.compilerArgs": [
    "--target=arm-arm-none-eabi",
    "-mcpu=cortex-m4",
    "-mthumb",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=hard"
  ],
  "C_Cpp.default.intelliSenseMode": "armclang"
}
```

---

## 经验教训

### ✅ 做得好的

1. **先架构后代码。** 五层分层不是 AI 生成的，是我设计的。这决定了后续 AI 生成代码的质量上限。
2. **每一次编译错误都记录。** AI 不是一次性写对的，而是通过"编译 → 错误 → AI 修复 → 再编译"迭代，平均每个功能 3-5 轮。
3. **Continuity 记录决策是长期投资。** 比如"为什么用 FSMC 不用 SPI"——2 周后完全忘了原因，但决策记录还在。
4. **BSP 自测先于应用。** 在调 UI 之前先把 LCD/触摸的冒烟测试跑通，大大减少了后续定位问题的难度。

### ⚠️ 踩过的坑

1. **AI 生成的初始化顺序偶尔反了。** 比如先初始化 LCD 再初始化 FSMC 时钟——这种低级错误必须人工审查。
2. **AI 不熟悉你的具体芯片的 Errata。** STM32F407 有些外设的 Errata（比如 FSMC 时序限制），AI 不知道，需要人工标注。
3. **不要一次给 AI 太多上下文。** 把整个项目 dump 给 AI 会降低质量。最好按"功能"拆解，每次只给相关 3-5 个文件。
4. **AI 注释有时太啰嗦或无意义。** 比如 `// Set variable x to 5` ——需要删掉冗余注释。
5. **Continuity 的自动生成文件不要提交到公开仓库。** `.continuity/`、`.keilbridge/generated/` 等应加入 `.gitignore`——它们含个人路径和会话数据。

### 🎯 核心原则

1. **你是架构师，AI 是码农。** 分层设计、模块接口、性能约束——这些你来定。AI 只负责填充实现。
2. **编译是唯一真正的 Lint。** AI 生成的代码必须通过 keil build `0 Error(s)` 才算数。
3. **每个功能跑通立刻记录决策。** 不拖——一拖就忘。
4. **可复现比巧妙更重要。** 宁愿用查表法 sin/cos 做粒子（代码多点但绝对可复现），也不要用浮点运算依赖 FPU 配置正确性。

---

## FAQ

<details>
<summary><b>Q: AI 生成的代码质量到底怎么样？</b></summary>

对于模板代码（初始化序列、寄存器映射、回调注册）——质量很好，甚至比手写更规范（缩进、命名一致）。对于算法逻辑（粒子运动轨迹、触控滤波）——框架 OK，但参数需要人工调。对于架构设计——AI 能给建议，但不能替你做决定。
</details>

<details>
<summary><b>Q: 会不会害怕 AI 写了隐藏 Bug？</b></summary>

关键路径（FSMC 初始化、中断优先级、FreeRTOS 任务栈大小）我会仔细审查。非关键路径（UI 配色、数据表填充）可以信任 AI。原则：**能导致硬件损坏或系统卡死的代码 = 必须人工逐行审查。**
</details>

<details>
<summary><b>Q: Continuity 是什么？一定要用吗？</b></summary>

Continuity 是项目记忆工具。每次你做架构选择时，它自动记录 Q&A。下一次启动 Cursor 时，它会注入所有历史决策让 AI 知道上下文。不是必须的，但如果不用——每次新 AI 会话就是"失忆"状态，你得重新解释项目背景。
</details>

<details>
<summary><b>Q: Copilot vs Cursor 选哪个？</b></summary>

Cursor 是独立 IDE（基于 VS Code），Copilot 是 VS Code 插件。Cursor 的上下文窗口更大，更适合嵌入式这种多文件项目。Copilot 在单文件补全上更快。我两个都用：Copilot 做日常补全，Cursor 做大规模重构和生成。
</details>

<details>
<summary><b>Q: 我的项目能用同样的工作流吗？</b></summary>

只要满足三个条件：
1. 你的项目有明确的分层架构（或愿意先设计一个）
2. 你熟悉项目的编译流程，能判断 AI 代码是否通过
3. 你愿意每做完一个功能就记录决策（1 分钟/次）
</details>

<details>
<summary><b>Q: 如果我不用 AI，只看这个项目的代码能学到东西吗？</b></summary>

能。本项目的五层架构、BSP 抽象、LVGL 集成是独立于 AI 的。AI 工作流是锦上添花，不是必需品。所有代码都是可以脱离任何 AI 工具编译运行的。
</details>

---

## 相关链接

- [README.md](README.md) — 项目主文档
- [UI/README.md](UI/README.md) — UI 层完整指南
- [.vscode/settings.json](.vscode/settings.json) — VS Code 配置模板（参考，使用前需修改 compilerPath）
- [pin mapping.md](pin mapping.md) — 完整管脚映射

---

<p align="center">
  <i>AI 是杠杆，你是支点。没有支点，再长的杠杆也翘不起任何东西。</i>
</p>