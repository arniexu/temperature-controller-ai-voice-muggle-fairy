# 启明欣欣 STM32F407 开发板（高配版 V6.1）管脚与外设连接关系

> **主控芯片**: STM32F407ZGT6 (LQFP144封装)  
> **文档版本**: V6.1  
> **日期**: 2024-12-19

---

## 目录

- [一、外设模块与引脚对应总览](#一外设模块与引脚对应总览)
- [二、GPIO端口详细映射](#二gpio端口详细映射)
  - [PA 端口](#pa-端口)
  - [PB 端口](#pb-端口)
  - [PC 端口](#pc-端口)
  - [PD 端口](#pd-端口)
  - [PE 端口](#pe-端口)
  - [PF 端口](#pf-端口)
  - [PG 端口](#pg-端口)
  - [PH 端口及其他](#ph-端口及其他)
- [三、FSMC总线引脚分配](#三fsmc总线引脚分配)
- [四、以太网(RMII)引脚分配](#四以太网rmii引脚分配)
- [五、外设接口定义](#五外设接口定义)

---

## 一、外设模块与引脚对应总览

| 外设模块 | 使用引脚 | 说明 |
|:---------|:---------|:-----|
| **LED0** | PE3 | 用户LED0 |
| **LED1** | PE4 | 用户LED1 |
| **LED2** | PG9 | 用户LED2 |
| **KEY0** | PF9 | 五向按键-上 |
| **KEY1** | PF8 | 五向按键-下 |
| **KEY2** | PF7 | 五向按键-左 |
| **KEY3** | PF6 | 五向按键-右 |
| **BEEP** | PG7 | 蜂鸣器 |
| **HS0038** | PA8 | 红外接收头 |
| **18B20/DHT11** | PD3 | 温湿度/温度传感器接口 |
| **W25Q128** | PG8(CS), PB3(CLK), PB4(MISO), PB5(MOSI) | 16MB SPI Flash |
| **AT24C02** | PB6(SCL), PB7(SDA) | EEPROM(I2C1) |
| **WM8978** | PB6, PB7, PC0-PC3, PC6, PC13, PB12-PB15 | I2S音频编解码器 |
| **VS1053** | PG8, PB3, PB4, PB5, PC6-PC13 | MP3音频模块 |
| **LAN8720A** | PA1, PA2, PA7, PC1, PC4, PC5, PG11-PG14 | 以太网PHY(RMII) |
| **TFTLCD** | PF10(BL), PF11(T_PEN), PD4, PD5, PD7, PD11-PD15, PE7-PE15, PF12-PF15, PG0-PG5, PG10, PG12 | FSMC接口LCD+触摸屏 |
| **SRAM** | PE0, PE1, PD4, PD5, PD7, PD11-PD15, PE7-PE15, PF0-PF15, PG0-PG15 | IS62WV51216外部SRAM |
| **OV2640摄像头** | PA4-PA8, PA9-PA10, PA15, PC6-PC9, PC10-PC12, PD6-PD7, PE5-PE6, PB6, PB7, PG15, PB5 | DCMI接口 |
| **SD卡(SDIO)** | PC8-PC12, PD2 | SDIO接口TF卡槽 |
| **USB_OTG** | PA11(DM), PA12(DP) | USB OTG FS |
| **CAN1** | PA11(RX), PA12(TX), PD0(RX), PD1(TX) | CAN总线1 |
| **CAN2** | PB12(RX), PB13(TX), PB5(RX), PB6(TX) | CAN总线2 |
| **RS485** | PC6(TX), PC7(RX), PG6(RE) | MAX485 |
| **UART1** | PA9(TX), PA10(RX) | 串口1(带MAX232) |
| **UART2** | PA2(TX), PA3(RX) | 串口2 |
| **UART3** | PB10(TX), PB11(RX) | 串口3(蓝牙模块) |
| **UART4** | PA0(TX), PA1(RX) | 串口4(WiFi模块) |
| **UART5** | PC12(TX), PD2(RX) | 串口5 |
| **UART6** | PC6(TX), PC7(RX), PG8(CS)/PG9(CTS) | 串口6 |
| **JTAG/SWD** | PA13(TMS/SWDIO), PA14(TCK/SWCLK), PA15(TDI), PB3(TDO), PB4(JRST) | 调试接口 |

---

## 二、GPIO端口详细映射

### PA 端口

| 引脚 | 引脚号 | 可用功能 | 连接外设/用途 |
|:----:|:------:|:---------|:--------------|
| **PA0** | 34 | WKUP/U2_CTS/U4_TX/TIM2_CH1_ETR/TIM5_CH1/TIM8_ETR/ADC123_IN0 | **U4_TX** (WiFi模块) |
| **PA1** | 35 | U2_RTS/U4_RX/TIM2_CH2/TIM5_CH2/ADC123_IN1 | **U4_RX** (WiFi模块) / **RMII_REF_CLK** |
| **PA2** | 36 | U2_TX/TIM2_CH3/TIM5_CH3/TIM9_CH1/**ETH_MDIO**/ADC123_IN2 | **RMII_MDIO** (LAN8720) |
| **PA3** | 37 | U2_RX/TIM2_CH4/TIM5_CH4/TIM9_CH2/OTG_HS_ULPI_D0/ETH_MII_COL/ADC123_IN3 | 未连接主要外设 |
| **PA4** | 40 | SPI1_NSS/U2_CK/DCMI_HSYNC/OTG_HS_SOF/I2S3_WS/DAC1_OUT/ADC12_IN4 | **DCMI_HREF** (摄像头) |
| **PA5** | 41 | SPI1_SCK/OTG_HS_ULPI_CK/TIM2_CH1_ETR/TIM8_CH1N/DAC2_OUT/ADC12_IN5 | **T_CLK** (LCD触摸) |
| **PA6** | 42 | SPI1_MISO/TIM1_BKIN/TIM3_CH1/TIM8_BKIN/TIM13_CH1/DCMI_PIXCLK/ADC12_IN6 | **DCMI_PCLK** (摄像头) |
| **PA7** | 43 | SPI1_MOSI/TIM1_CH1N/TIM3_CH2/TIM8_CH1N/TIM14_CH1/**ETH_RMII_CRS_DV**/ADC12_IN7 | **RMII_CRS_DV** / LCD |
| **PA8** | 100 | TIM1_CH1/U1_CK/I2C3_SCL/MCO1/OTG_FS_SOF | **HS0038** 红外接收 / **DCMI_XCLK** |
| **PA9** | 101 | TIM1_CH2/**U1_TX**/I2C3_SMBA/OTG_FS_VBUS/DCMI_D0 | **U1_TX** (串口1) |
| **PA10** | 102 | TIM1_CH3/**U1_RX**/OTG_FS_ID/DCMI_D1 | **U1_RX** (串口1) |
| **PA11** | 103 | TIM1_CH4/U1_CTS/**CAN1_RX**/OTG_FS_DM | **USB_D-** / **CAN1_RX** |
| **PA12** | 104 | TIM1_ETR/U1_RTS/**CAN1_TX**/OTG_FS_DP | **USB_D+** / **CAN1_TX** |
| **PA13** | 105 | **JTMS_SWDIO** | JTAG/SWD调试 |
| **PA14** | 109 | **JTCK_SWCLK** | JTAG/SWD调试 |
| **PA15** | 110 | JTDI/SPI1_NSS/SPI3_NSS/TIM2_CH1_ETR/I2S3_WS | JTAG调试 |

### PB 端口

| 引脚 | 引脚号 | 可用功能 | 连接外设/用途 |
|:----:|:------:|:---------|:--------------|
| **PB0** | 46 | TIM1_CH2N/TIM3_CH3/TIM8_CH2N/OTG_HS_ULPI_D1/ETH_MII_RXD2/ADC12_IN8 | **T_CS** (LCD触摸片选) |
| **PB1** | 47 | TIM1_CH3N/TIM3_CH4/TIM8_CH3N/OTG_HS_INTN/ETH_MII_RXD3/ADC12_IN9 | **T_MOSI** (LCD触摸) |
| **PB2** | 48 | **BOOT1** | 启动模式选择引脚 |
| **PB3** | 133 | JTDO/TRACESWO/TIM2_CH2/**SPI1_SCK**/SPI3_SCK/I2S3_CK | **SPI1_CLK** / JTAG_TDO |
| **PB4** | 134 | JNTRST/TIM3_CH1/**SPI1_MISO**/SPI3_MISO/I2S3ext_SD | **SPI1_MISO** / JTAG_JRST |
| **PB5** | 135 | TIM3_CH2/**SPI1_MOSI**/SPI3_MOSI/CAN2_RX/I2C1_SMBA/OTG_HS_ULPI_D7/ETH_PPS_OUT/DCMI_D10/I2S3_SD | **SPI1_MOSI** / DCMI_D10 |
| **PB6** | 136 | TIM4_CH1/U1_TX/**CAN2_TX**/**I2C1_SCL**/DCMI_D5 | **IIC_SCL** / DCMI_D5 |
| **PB7** | 137 | TIM4_CH2/U1_RX/FSMC_NL/**I2C1_SDA**/DCMI_VSYNC | **IIC_SDA** / DCMI_VSYNC |
| **PB8** | 139 | TIM4_CH3/TIM10_CH1/CAN1_RX/SDIO_D4/ETH_MII_TXD3/**I2C1_SCL**/DCMI_D6 | **IIC_SCL** |
| **PB9** | 140 | TIM4_CH4/TIM11_CH1/CAN1_TX/SDIO_D5/SPI2_NSS/**I2C1_SDA**/DCMI_D7/I2S2_WS | **IIC_SDA** |
| **PB10** | 69 | SPI2_SCK/TIM2_CH3/**U3_TX**/I2C2_SCL/OTG_HS_ULPI_D3/I2S2_CK | **U3_TX** (蓝牙模块) |
| **PB11** | 70 | TIM2_CH4/**U3_RX**/I2C2_SDA/OTG_HS_ULPI_D4/**ETH_RMII_TX_EN** | **U3_RX** / **RMII_TX_EN** |
| **PB12** | 73 | SPI2_NSS/TIM1_BKIN/U3_CK/**CAN2_RX**/I2C2_SMBA/OTG_HS_ULPI_D5/OTG_HS_ID/**ETH_RMII_TXD0**/I2S2_WS | **I2S_LRCK** / CAN2_RX |
| **PB13** | 74 | SPI2_SCK/TIM1_CH1N/U3_CTS/**CAN2_TX**/OTG_HS_ULPI_D6/OTG_HS_VBUS/**ETH_RMII_TXD1**/I2S2_CK | **SPI2_CLK/I2S_SCLK** |
| **PB14** | 75 | SPI2_MISO/TIM1_CH2N/TIM8_CH2N/TIM12_CH1/U3_RTS/OTG_HS_DM/I2S2ext_SD | **USB_HS_DM** |
| **PB15** | 76 | SPI2_MOSI/TIM1_CH3N/TIM8_CH3N/TIM12_CH2/OTG_HS_DP/I2S2_SD | **USB_HS_DP** |

### PC 端口

| 引脚 | 引脚号 | 可用功能 | 连接外设/用途 |
|:----:|:------:|:---------|:--------------|
| **PC0** | 26 | OTG_HS_ULPI_STP/ADC123_IN10 | **I2S_SCK** (WM8978) |
| **PC1** | 27 | **ETH_MDC**/ADC123_IN11 | **RMII_MDC** (LAN8720) |
| **PC2** | 28 | **SPI2_MISO**/OTG_HS_ULPI_DIR/ETH_MII_TXD2/I2S2ext_SD/ADC123_IN12 | **SPI2_MISO/I2S_SDOUT** |
| **PC3** | 29 | **SPI2_MOSI**/OTG_HS_ULPI_NXT/ETH_MII_TX_CLK/I2S2_SD/ADC123_IN13 | **SPI2_MOSI/I2S_SDIN** |
| **PC4** | 44 | **ETH_RMII_RXD0**/ADC12_IN14 | **RMII_RXD0** (LAN8720) |
| **PC5** | 45 | **ETH_RMII_RXD1**/ADC12_IN15 | **RMII_RXD1** (LAN8720) |
| **PC6** | 96 | TIM3_CH1/TIM8_CH1/**U6_TX**/SDIO_D6/DCMI_D0/I2S2_MCK | **U6_TX** / **DCMI_D0** / **I2S_MCLK** |
| **PC7** | 97 | TIM3_CH2/TIM8_CH2/**U6_RX**/SDIO_D7/DCMI_D1/I2S3_MCK | **U6_RX** / **DCMI_D1** |
| **PC8** | 98 | TIM3_CH3/TIM8_CH3/U6_CK/**SDIO_D0**/DCMI_D2 | **SDIO_D0** / **DCMI_D2** |
| **PC9** | 99 | TIM3_CH4/TIM8_CH4/I2C3_SDA/**SDIO_D1**/DCMI_D3/MCO2/I2S_CKIN | **SDIO_D1** / **DCMI_D3** |
| **PC10** | 111 | SPI3_SCK/U3_TX/U4_TX/**SDIO_D2**/DCMI_D8/I2S3_CK | **SDIO_D2** |
| **PC11** | 112 | SPI3_MISO/U3_RX/U4_RX/**SDIO_D3**/DCMI_D4/I2S3ext_SD | **SDIO_D3** / **DCMI_D4** |
| **PC12** | 113 | SPI3_MOSI/U3_CK/**U5_TX**/**SDIO_CK**/DCMI_D9/I2S3_SD | **SDIO_CLK** / **U5_TX** |
| **PC13** | 7 | RTC_AF1 | **I2S_SDA** (WM8978) |
| **PC14** | 8 | OSC32_IN | 32.768KHz晶振 |
| **PC15** | 9 | OSC32_OUT | 32.768KHz晶振 |

### PD 端口

| 引脚 | 引脚号 | 可用功能 | 连接外设/用途 |
|:----:|:------:|:---------|:--------------|
| **PD0** | 114 | **FSMC_D2**/**CAN1_RX** | **FSMC_D2** (SRAM/LCD) / **CAN1_RX** |
| **PD1** | 115 | **FSMC_D3**/**CAN1_TX** | **FSMC_D3** (SRAM/LCD) / **CAN1_TX** |
| **PD2** | 116 | TIM3_ETR/**U5_RX**/**SDIO_CMD**/DCMI_D11 | **SDIO_CMD** / **U5_RX** |
| **PD3** | 117 | FSMC_CLK/U2_CTS | **18B20/DHT11** / **DCMI_PWDN** |
| **PD4** | 118 | **FSMC_NOE**/U2_RTS | **FSMC_NOE** (读使能) |
| **PD5** | 119 | **FSMC_NWE**/U2_TX | **FSMC_NWE** (写使能) |
| **PD6** | 122 | FSMC_NWAIT/U2_RX | **DCMI_SCL** (摄像头I2C) |
| **PD7** | 123 | **FSMC_NE1**/FSMC_NCE2/U2_CK | **FSMC_NE1** (片选) / **DCMI_SDA** |
| **PD8** | 77 | **FSMC_D13**/U3_TX | **FSMC_D13** |
| **PD9** | 78 | **FSMC_D14**/U3_RX | **FSMC_D14** |
| **PD10** | 79 | **FSMC_D15**/U3_CK | **FSMC_D15** |
| **PD11** | 80 | **FSMC_A16**/FSMC_CLE/U3_CTS | **FSMC_A16** |
| **PD12** | 81 | **FSMC_A17**/FSMC_ALE/TIM4_CH1/U3_RTS | **FSMC_A17** |
| **PD13** | 82 | **FSMC_A18**/TIM4_CH2 | **FSMC_A18** |
| **PD14** | 85 | **FSMC_D0**/TIM4_CH3 | **FSMC_D0** |
| **PD15** | 86 | **FSMC_D1**/TIM4_CH4 | **FSMC_D1** |

### PE 端口

| 引脚 | 引脚号 | 可用功能 | 连接外设/用途 |
|:----:|:------:|:---------|:--------------|
| **PE0** | 141 | TIM4_ETR/**FSMC_NBL0**/DCMI_D2 | **FSMC_NBL0** (SRAM字节使能) |
| **PE1** | 142 | **FSMC_NBL1**/DCMI_D3 | **FSMC_NBL1** (SRAM字节使能) |
| **PE2** | 1 | TRACECLK/FSMC_A23/ETH_MII_TXD3 | **ETH_RESET** (LAN8720复位) |
| **PE3** | 2 | TRACED0/FSMC_A19 | **LED0** |
| **PE4** | 3 | TRACED1/FSMC_A20/DCMI_D4 | **LED1** |
| **PE5** | 4 | TRACED2/FSMC_A21/TIM9_CH1/DCMI_D6 | **DCMI_D6** (摄像头) |
| **PE6** | 5 | TRACED3/FSMC_A22/TIM9_CH2/DCMI_D7 | **DCMI_D7** (摄像头) |
| **PE7** | 58 | **FSMC_D4**/TIM1_ETR | **FSMC_D4** |
| **PE8** | 59 | **FSMC_D5**/TIM1_CH1N | **FSMC_D5** |
| **PE9** | 60 | **FSMC_D6**/TIM1_CH1 | **FSMC_D6** |
| **PE10** | 63 | **FSMC_D7**/TIM1_CH2N | **FSMC_D7** |
| **PE11** | 64 | **FSMC_D8**/TIM1_CH2 | **FSMC_D8** |
| **PE12** | 65 | **FSMC_D9**/TIM1_CH3N | **FSMC_D9** |
| **PE13** | 66 | **FSMC_D10**/TIM1_CH3 | **FSMC_D10** |
| **PE14** | 67 | **FSMC_D11**/TIM1_CH4 | **FSMC_D11** |
| **PE15** | 68 | **FSMC_D12**/TIM1_BKIN | **FSMC_D12** |

### PF 端口

| 引脚 | 引脚号 | 可用功能 | 连接外设/用途 |
|:----:|:------:|:---------|:--------------|
| **PF0** | 10 | **FSMC_A0**/I2C2_SDA | **FSMC_A0** (地址总线) |
| **PF1** | 11 | **FSMC_A1**/I2C2_SCL | **FSMC_A1** |
| **PF2** | 12 | **FSMC_A2**/I2C2_SMBA | **FSMC_A2** |
| **PF3** | 13 | **FSMC_A3**/ADC3_IN9 | **FSMC_A3** |
| **PF4** | 14 | **FSMC_A4**/ADC3_IN14 | **FSMC_A4** |
| **PF5** | 15 | **FSMC_A5**/ADC3_IN15 | **FSMC_A5** |
| **PF6** | 18 | TIM10_CH1/FSMC_NIORD/ADC3_IN4 | **KEY3** (五向按键) |
| **PF7** | 19 | TIM11_CH1/FSMC_NREG/ADC3_IN5 | **KEY2** (五向按键) |
| **PF8** | 20 | TIM13_CH1/FSMC_NIOWR/ADC3_IN6 | **KEY1** (五向按键) |
| **PF9** | 21 | TIM14_CH1/FSMC_CD/ADC3_IN7 | **KEY0** (五向按键) |
| **PF10** | 22 | FSMC_INTR/ADC3_IN8 | **LCD_BL** (LCD背光) |
| **PF11** | 49 | DCMI_D12 | **T_PEN** (LCD触摸屏) |
| **PF12** | 50 | **FSMC_A6** | **FSMC_A6** |
| **PF13** | 53 | **FSMC_A7** | **FSMC_A7** |
| **PF14** | 54 | **FSMC_A8** | **FSMC_A8** |
| **PF15** | 55 | **FSMC_A9** | **FSMC_A9** |

### PG 端口

| 引脚 | 引脚号 | 可用功能 | 连接外设/用途 |
|:----:|:------:|:---------|:--------------|
| **PG0** | 56 | **FSMC_A10** | **FSMC_A10** |
| **PG1** | 57 | **FSMC_A11** | **FSMC_A11** |
| **PG2** | 87 | **FSMC_A12** | **FSMC_A12** |
| **PG3** | 88 | **FSMC_A13** | **FSMC_A13** |
| **PG4** | 89 | **FSMC_A14** | **FSMC_A14** |
| **PG5** | 90 | **FSMC_A15** | **FSMC_A15** |
| **PG6** | 91 | FSMC_INT2 | **485_RE** (RS485使能) |
| **PG7** | 92 | FSMC_INT3/U6_CK | **BEED** (蜂鸣器) |
| **PG8** | 93 | U6_RTS/ETH_PPS_OUT | **W25_CS** (W25Q128片选) |
| **PG9** | 124 | **FSMC_NE2**/FSMC_NCE3/U6_RX | **LED2** |
| **PG10** | 125 | **FSMC_NCE4_1**/**FSMC_NE3** | **FSMC_NE3** |
| **PG11** | 126 | FSMC_NCE4_2/**ETH_RMII_TX_EN** | **RMII_TX_EN** |
| **PG12** | 127 | **FSMC_NE4**/U6_RTS | **FSMC_NE4** |
| **PG13** | 128 | FSMC_A24/U6_CTS/**ETH_RMII_TXD0** | **RMII_TXD0** |
| **PG14** | 129 | FSMC_A25/U6_TX/**ETH_RMII_TXD1** | **RMII_TXD1** |
| **PG15** | 132 | U6_CTS/DCMI_D13 | **DCMI_RESET** (摄像头复位) |

### PH 端口及其他

| 引脚 | 引脚号 | 可用功能 | 连接外设/用途 |
|:----:|:------:|:---------|:--------------|
| **PH0** | 23 | OSC_IN | 8MHz外部晶振(Y2) |
| **PH1** | 24 | OSC_OUT | 8MHz外部晶振(Y2) |
| **NRST** | 25 | 复位 | 复位按键 + 复位电路 |
| **BOOT0** | 138 | BOOT0 | 启动模式选择(P1跳线) |
| **VBAT** | 6 | 备份电源 | 电池接口(BT1) |
| **VDDA** | 32/33 | 模拟电源 | 3.3V模拟供电 |
| **Vcap1** | 71 | 稳压输出 | 2.2uF稳压电容(C5) |
| **Vcap2** | 106 | 稳压输出 | 2.2uF稳压电容(C6) |

---

## 三、FSMC总线引脚分配

FSMC(灵活的静态存储器控制器)用于连接 **TFTLCD**、**外部SRAM(IS62WV51216)** 等外设。

### 数据总线

| FSMC信号 | STM32引脚 | 用途 |
|:---------|:----------|:-----|
| FSMC_D0 | PD14 | 数据线0 |
| FSMC_D1 | PD15 | 数据线1 |
| FSMC_D2 | PD0 | 数据线2 |
| FSMC_D3 | PD1 | 数据线3 |
| FSMC_D4 | PE7 | 数据线4 |
| FSMC_D5 | PE8 | 数据线5 |
| FSMC_D6 | PE9 | 数据线6 |
| FSMC_D7 | PE10 | 数据线7 |
| FSMC_D8 | PE11 | 数据线8 |
| FSMC_D9 | PE12 | 数据线9 |
| FSMC_D10 | PE13 | 数据线10 |
| FSMC_D11 | PE14 | 数据线11 |
| FSMC_D12 | PE15 | 数据线12 |
| FSMC_D13 | PD8 | 数据线13 |
| FSMC_D14 | PD9 | 数据线14 |
| FSMC_D15 | PD10 | 数据线15 |

### 地址总线

| FSMC信号 | STM32引脚 | 用途 |
|:---------|:----------|:-----|
| FSMC_A0 | PF0 | 地址线0 |
| FSMC_A1 | PF1 | 地址线1 |
| FSMC_A2 | PF2 | 地址线2 |
| FSMC_A3 | PF3 | 地址线3 |
| FSMC_A4 | PF4 | 地址线4 |
| FSMC_A5 | PF5 | 地址线5 |
| FSMC_A6 | PF12 | 地址线6 |
| FSMC_A7 | PF13 | 地址线7 |
| FSMC_A8 | PF14 | 地址线8 |
| FSMC_A9 | PF15 | 地址线9 |
| FSMC_A10 | PG0 | 地址线10 |
| FSMC_A11 | PG1 | 地址线11 |
| FSMC_A12 | PG2 | 地址线12 / LCD_RS |
| FSMC_A13 | PG3 | 地址线13 |
| FSMC_A14 | PG4 | 地址线14 |
| FSMC_A15 | PG5 | 地址线15 |
| FSMC_A16 | PD11 | 地址线16 |
| FSMC_A17 | PD12 | 地址线17 |
| FSMC_A18 | PD13 | 地址线18 |

### 控制信号

| FSMC信号 | STM32引脚 | 用途 |
|:---------|:----------|:-----|
| FSMC_NOE | PD4 | 读使能(OE) |
| FSMC_NWE | PD5 | 写使能(WE) |
| FSMC_NE3 | PG10 | 片选3 |
| FSMC_NE4 | PG12 | 片选4(LCD_CS) |
| FSMC_NBL0 | PE0 | 字节使能0(UB) |
| FSMC_NBL1 | PE1 | 字节使能1(LB) |

---

## 四、以太网(RMII)引脚分配

以太网 PHY 芯片为 **LAN8720A**，通过 RMII 接口连接。

| RMII信号 | STM32引脚 | LAN8720引脚 | 说明 |
|:---------|:----------|:------------|:-----|
| RMII_REF_CLK | PA1 | PIN1 | 50MHz参考时钟 |
| RMII_MDIO | PA2 | PIN12 | 管理数据(MDIO) |
| RMII_MDC | PC1 | PIN13 | 管理时钟(MDC) |
| RMII_CRS_DV | PA7 | PIN11 | 载波侦听/数据有效 |
| RMII_RXD0 | PC4 | PIN8 | 接收数据0 |
| RMII_RXD1 | PC5 | PIN7 | 接收数据1 |
| RMII_TX_EN | PG11 | PIN16 | 发送使能 |
| RMII_TXD0 | PG13 | PIN17 | 发送数据0 |
| RMII_TXD1 | PG14 | PIN18 | 发送数据1 |
| ETH_RESET | PE2 | PIN15 | PHY复位控制 |

**注意**: LAN8720A 的 PIN14(nINT/REFCLKO) 用于输出50MHz时钟，通过R30(1M)电阻配置。

---

## 五、外设接口定义

### 5.1 JTAG/SWD调试接口 (J1)

| 引脚 | 信号 | 连接 |
|:----:|:-----|:-----|
| 1 | VDD | 3.3V |
| 3 | TRST | PB4 (JNTRST) |
| 5 | TMS/SWDIO | PA13 |
| 7 | TCK/SWCLK | PA14 |
| 9 | TDO | PB3 (JTDO) |
| 11 | TDI | PA15 |
| 15 | RESET | NRST |

### 5.2 UART串口接口

| 串口 | TX引脚 | RX引脚 | 用途 |
|:----:|:-------|:-------|:-----|
| UART1 | PA9 | PA10 | RS232(MAX232) |
| UART2 | PA2 | PA3 | 引出接口 |
| UART3 | PB10 | PB11 | 蓝牙模块 |
| UART4 | PA0 | PA1 | WiFi模块(ESP8266) |
| UART5 | PC12 | PD2 | 引出接口 |
| UART6 | PC6 | PC7 | RS485(MAX485) |

### 5.3 摄像头接口(OV2640, P18)

| 引脚号 | 信号 | STM32引脚 |
|:------:|:-----|:----------|
| 1 | DCMI_VSYNC | PB7 |
| 3 | DCMI_HREF | PA4 |
| 5 | DCMI_RESET | PG15 |
| 7 | DCMI_D0 | PC6 |
| 9 | DCMI_D1 | PC7 |
| 11 | DCMI_D2 | PC8 |
| 13 | DCMI_D3 | PC9 |
| 15 | DCMI_D4 | PC11 |
| 17 | DCMI_D5 | PB6 |
| 19 | DCMI_D6 | PE5 |
| 21 | DCMI_D7 | PE6 |
| 15 | DCMI_PCLK | PA6 |
| 17 | DCMI_XCLK | PA8 |
| 18 | DCMI_PWDN | PD3 |
| 3 | DCMI_SCL | PD6 |
| 4 | DCMI_SDA | PD7 |

### 5.4 SD卡槽(J8, SDIO模式)

| SD信号 | STM32引脚 | TF卡引脚 |
|:-------|:----------|:---------|
| SDIO_D0 | PC8 | DAT0 |
| SDIO_D1 | PC9 | DAT1 |
| SDIO_D2 | PC10 | DAT2 |
| SDIO_D3 | PC11 | DAT3/CD |
| SDIO_CK | PC12 | CLK |
| SDIO_CMD | PD2 | CMD |

### 5.5 I2S音频(WM8978) / VS1053模块接口

| I2S信号 | STM32引脚 | VS1053对应 |
|:--------|:----------|:-----------|
| I2S_SCK | PC0 | VS_SCK |
| I2S_SDIN | PC3 | VS_MOSI |
| I2S_SDOUT | PC2 | VS_MISO |
| I2S_SCLK | PB13 | VS_SCLK |
| I2S_LRCK | PB12 | VS_XDCS |
| I2S_MCLK | PC6 | VS_XCS |
| I2S_SDA | PC13 | VS_DREQ |

### 5.6 LCD触摸屏信号

| 信号 | STM32引脚 | 说明 |
|:-----|:----------|:-----|
| T_CLK | PA5 | 触摸时钟(SPI1_SCK) |
| T_CS | PB0 | 触摸片选(SPI1_NSS) |
| T_MOSI | PB1 | 触摸MOSI(SPI1_MOSI) |
| T_MISO | PB4 | 触摸MISO(SPI1_MISO) |
| T_PEN | PF11 | 触摸中断 |
| LCD_BL | PF10 | LCD背光控制 |

### 5.7 CAN总线接口

| CAN | TX引脚 | RX引脚 | 接口芯片 |
|:---:|:-------|:-------|:---------|
| CAN1 | PA12 / PD1 | PA11 / PD0 | TJA1050 (P6) |
| CAN2 | PB13 / PB6 | PB12 / PB5 | TJA1050 (P15) |

### 5.8 RS485接口

| 信号 | STM32引脚 | MAX485引脚 |
|:-----|:----------|:-----------|
| 485_TX | PC6 (U6_TX) | DI |
| 485_RX | PC7 (U6_RX) | RO |
| 485_RE | PG6 | RE/DE |

### 5.9 W25Q128 SPI Flash

| SPI信号 | STM32引脚 | W25Q128引脚 |
|:--------|:----------|:------------|
| SPI1_CLK | PB3 | CLK |
| SPI1_MISO | PB4 | DO |
| SPI1_MOSI | PB5 | DI |
| W25_CS | PG8 | CS |

### 5.10 AT24C02 EEPROM

| I2C信号 | STM32引脚 | AT24C02引脚 |
|:--------|:----------|:------------|
| I2C_SCL | PB6 | SCL |
| I2C_SDA | PB7 | SDA |

---

## 附：启动模式配置

| BOOT1 | BOOT0 | 启动模式 |
|:-----:|:-----:|:---------|
| X | 0 | 从主闪存存储器启动(默认) |
| 0 | 1 | 从系统存储器启动(串口ISP下载) |
| 1 | 1 | 从内置SRAM启动 |

**开发板默认配置**: BOOT0=0 (从Flash启动)，通过P1跳线可切换。
