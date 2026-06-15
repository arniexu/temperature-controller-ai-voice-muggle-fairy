/**
  ******************************************************************************
  * @file           : hil_telemetry.h
  * @brief          : HIL Telemetry JSON-over-UART module
  * @description    : Sends structured telemetry JSON frames over UART1 at 100ms
  *                   intervals. Compatible with Serial Studio data frame protocol.
  * @protocol       : Serial Studio JSON Frame ({timestamp,channels}[,...])
  *                   Each frame: [{"timestamp":ms, "channels":[v1,v2,...]},...]
  ******************************************************************************
  */

#ifndef __HIL_TELEMETRY_H
#define __HIL_TELEMETRY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/* ----------------------------------------------------------------
 * HIL Telemetry Channel Map (index → meaning)
 * ----------------------------------------------------------------
 * [0]  System tick (ms)
 * [1]  Boot stage
 * [2]  Target temperature (°C × 10, e.g. 255 = 25.5°C)
 * [3]  Current temperature (°C × 10)
 * [4]  ADC raw (sensor voltage, mV)
 * [5]  Heater state (0=off, 1=on)
 * [6]  FreeRTOS heap free (bytes)
 * [7]  UI screen ID
 * [8]  Error flags bitmask
 * [9]  CPU usage % (estimated via Idle hook)
 * ---------------------------------------------------------------- */
#define HIL_CH_TICK          0
#define HIL_CH_BOOT_STAGE    1
#define HIL_CH_TARGET_TEMP   2
#define HIL_CH_CURRENT_TEMP  3
#define HIL_CH_ADC_MV        4
#define HIL_CH_HEATER_STATE  5
#define HIL_CH_HEAP_FREE     6
#define HIL_CH_UI_SCREEN     7
#define HIL_CH_ERROR_FLAGS   8
#define HIL_CH_CPU_USAGE     9
#define HIL_CHANNEL_COUNT    10

/* ----------------------------------------------------------------
 * HIL Control Block (placed at fixed SRAM address for pyOCD access)
 * ----------------------------------------------------------------
 * Reserved at 0x2001FC00 (last 1KB of SRAM2 on STM32F407ZGT6).
 * pyOCD reads/writes this block to inject test vectors or extract
 * state without stopping the CPU.
 * ---------------------------------------------------------------- */
typedef struct __attribute__((packed)) {
    uint32_t magic;             /* 0x48494C42 "HILB" — validates block integrity */
    uint32_t version;           /* Protocol version (1) */

    /* Telemetry (written by firmware, read by pyOCD) */
    volatile int32_t target_temp_x10;
    volatile int32_t current_temp_x10;
    volatile uint32_t adc_mv;
    volatile uint32_t heater_state;
    volatile uint32_t ui_screen;
    volatile uint32_t error_flags;
    volatile uint32_t cpu_usage_pct;

    /* Injection (written by pyOCD, read by firmware) */
    volatile uint32_t inject_enable;      /* 1 = injection active */
    volatile int32_t  inject_sensor_temp; /* Override sensor temperature × 10 */
    volatile uint32_t inject_touch_x;     /* Simulated touch X (0 = no touch) */
    volatile uint32_t inject_touch_y;     /* Simulated touch Y */
    volatile uint32_t inject_button;      /* Simulated button press mask */

    volatile uint32_t sequence;           /* Monotonic counter (tears detect) */
} hil_control_block_t;

/* Fixed SRAM address of the HIL control block */
#define HIL_CONTROL_BLOCK_ADDR  0x2001FC00UL

/* Magic value for validation */
#define HIL_MAGIC               0x48494C42UL  /* "HILB" */

/* ----------------------------------------------------------------
 * API
 * ---------------------------------------------------------------- */

/**
  * @brief  Initialize HIL telemetry module
  * @note   Called once at boot after UART init. Writes HIL_MAGIC to the
  *         control block to signal readiness to pyOCD.
  */
void HIL_Telemetry_Init(void);

/**
  * @brief  Send a single telemetry frame over UART1
  * @note   Called from hilTelemetryTask at 100ms intervals.
  *         Format: [{"timestamp":ms,"channels":[...]}]\n
  */
void HIL_Telemetry_SendFrame(void);

/**
  * @brief  Update telemetry channel values (called from app logic)
  * @param  channel: channel index (HIL_CH_xxx)
  * @param  value: channel value
  */
void HIL_Telemetry_SetChannel(uint32_t channel, int32_t value);

#ifdef __cplusplus
}
#endif

#endif /* __HIL_TELEMETRY_H */