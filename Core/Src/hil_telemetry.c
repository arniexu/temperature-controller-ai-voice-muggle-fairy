/**
  ******************************************************************************
  * @file           : hil_telemetry.c
  * @brief          : HIL Telemetry JSON-over-UART implementation
  ******************************************************************************
  */

#include "hil_telemetry.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

/* ----------------------------------------------------------------
 * Static variables
 * ---------------------------------------------------------------- */

/* Pointer to the fixed-address HIL control block in SRAM */
static hil_control_block_t *hil_block = NULL;

/* Channel data buffer (populated by app logic via HIL_Telemetry_SetChannel) */
static int32_t hil_channels[HIL_CHANNEL_COUNT];

/* ----------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------- */

/**
  * @brief  Initialize HIL telemetry module
  */
void HIL_Telemetry_Init(void)
{
    /* Initialize UART1 hardware (PA9/PA10, 115200-8N1) */
    MX_USART1_UART_Init();

    /* Point to the fixed SRAM address reserved for HIL control block */
    hil_block = (hil_control_block_t *)HIL_CONTROL_BLOCK_ADDR;

    /* Write magic + version to signal pyOCD that we're ready */
    hil_block->magic   = HIL_MAGIC;
    hil_block->version = 1;
    hil_block->sequence = 0;

    /* Clear telemetry fields */
    hil_block->target_temp_x10  = 0;
    hil_block->current_temp_x10 = 0;
    hil_block->adc_mv           = 0;
    hil_block->heater_state     = 0;
    hil_block->ui_screen        = 0;
    hil_block->error_flags      = 0;
    hil_block->cpu_usage_pct    = 0;

    /* Clear injection fields */
    hil_block->inject_enable     = 0;
    hil_block->inject_sensor_temp = 0;
    hil_block->inject_touch_x    = 0;
    hil_block->inject_touch_y    = 0;
    hil_block->inject_button     = 0;

    /* Clear channel buffer */
    memset(hil_channels, 0, sizeof(hil_channels));

    /* Send initial sync frame */
    HIL_Telemetry_SendFrame();
}

/**
  * @brief  Update a single telemetry channel
  */
void HIL_Telemetry_SetChannel(uint32_t channel, int32_t value)
{
    if (channel < HIL_CHANNEL_COUNT)
    {
        hil_channels[channel] = value;
    }
}

/**
  * @brief  Send a telemetry JSON frame over UART1
  * @note   Format: [{"t":<ms>,"c":[v0,v1,...]}]\n
  *         Compatible with Serial Studio "JSON Frame" data format.
  *         Maximum frame size ~200 bytes → fits in one UART burst.
  */
void HIL_Telemetry_SendFrame(void)
{
    char buf[256];
    int len;
    uint32_t tick = HAL_GetTick();

    /* Update telemetry fields in the shared HIL control block */
    if (hil_block != NULL)
    {
        hil_block->target_temp_x10  = hil_channels[HIL_CH_TARGET_TEMP];
        hil_block->current_temp_x10 = hil_channels[HIL_CH_CURRENT_TEMP];
        hil_block->adc_mv           = (uint32_t)hil_channels[HIL_CH_ADC_MV];
        hil_block->heater_state     = (uint32_t)hil_channels[HIL_CH_HEATER_STATE];
        hil_block->ui_screen        = (uint32_t)hil_channels[HIL_CH_UI_SCREEN];
        hil_block->error_flags      = (uint32_t)hil_channels[HIL_CH_ERROR_FLAGS];
        hil_block->cpu_usage_pct    = (uint32_t)hil_channels[HIL_CH_CPU_USAGE];
        hil_block->sequence++;
    }

    /* Format the JSON frame directly via snprintf (avoids float overhead) */
    len = snprintf(buf, sizeof(buf),
        "[{\"t\":%lu,\"c\":[%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld]}]\r\n",
        (unsigned long)tick,
        (long)hil_channels[0],   /* tick */
        (long)hil_channels[1],   /* boot stage */
        (long)hil_channels[2],   /* target temp ×10 */
        (long)hil_channels[3],   /* current temp ×10 */
        (long)hil_channels[4],   /* ADC mV */
        (long)hil_channels[5],   /* heater state */
        (long)hil_channels[6],   /* heap free */
        (long)hil_channels[7],   /* UI screen */
        (long)hil_channels[8],   /* error flags */
        (long)hil_channels[9]    /* CPU usage */
    );

    /* Send over UART1 */
    if (len > 0 && len < (int)sizeof(buf))
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, 100);
    }
}