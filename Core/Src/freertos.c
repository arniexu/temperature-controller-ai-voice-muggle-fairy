/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tianji_ui.h"
#include "tianji_dial.h"
#include "display_drv.h"
#include "touch_drv.h"
#include "lvgl.h"
#include "hil_telemetry.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
static tianji_ui_t *g_ui = NULL;
volatile uint32_t g_boot_stage = 0U;
volatile uint32_t g_diag_assert_count = 0U;
volatile uint32_t g_diag_hal_tick_last = 0U;
volatile uint32_t g_diag_rtos_tick_last = 0U;
volatile uint32_t g_diag_primask_last = 0U;
volatile uint32_t g_diag_basepri_last = 0U;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId hilTelemetryTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartHILTelemetryTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}

/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 4096);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* HIL Telemetry task — sends JSON frames over UART1 at 100ms intervals */
  osThreadDef(hilTelemetryTask, StartHILTelemetryTask, osPriorityLow, 0, 1024);
  hilTelemetryTaskHandle = osThreadCreate(osThread(hilTelemetryTask), NULL);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  uint32_t last_lvgl_tick = HAL_GetTick();
  uint32_t last_status_tick = HAL_GetTick();

  (void)argument;

  g_boot_stage = 0x20U;

#ifndef LVGL_MINI_TEST
  lv_init();
  g_boot_stage = 0x21U;

  if (display_drv_init() != 0) {
    Error_Handler();
  }
#endif
  g_boot_stage = 0x22U;

  if (touch_drv_init() != 0) {
    Error_Handler();
  }
  g_boot_stage = 0x23U;

  g_ui = tianji_ui_init();
  if (g_ui == NULL) {
    Error_Handler();
  }
  g_boot_stage = 0x24U;

  display_drv_set_backlight(85);
  g_boot_stage = 0x25U;

  /* Initialize HIL telemetry */
  g_boot_stage = 0x260U;
  HIL_Telemetry_Init();
  g_boot_stage = 0x261U;
  HIL_Telemetry_SetChannel(HIL_CH_BOOT_STAGE, 0x25);
  g_boot_stage = 0x262U;

  g_boot_stage = 0x26U;

  /* Infinite loop */
  for(;;)
  {
    g_boot_stage = 0x263U;
    uint32_t now = HAL_GetTick();

    g_diag_hal_tick_last = now;
    g_diag_rtos_tick_last = xTaskGetTickCount();
    g_diag_primask_last = __get_PRIMASK();
    g_diag_basepri_last = __get_BASEPRI();
    g_boot_stage = 0x264U;

    if ((now - last_lvgl_tick) >= 5U) {
      g_boot_stage = 0x265U;
      last_lvgl_tick = now;
      lv_timer_handler();
      g_boot_stage = 0x266U;
    }

    if ((g_ui != NULL) && ((now - last_status_tick) >= 200U)) {
      int16_t dial_temp;
      g_boot_stage = 0x267U;
      last_status_tick = now;

      dial_temp = tianji_ui_get_temp(g_ui);
      if (dial_temp != g_ui->current_temp) {
        g_ui->current_temp = dial_temp;
        tianji_ui_update_status(g_ui);
      }
      /* Update HIL telemetry channels */
      HIL_Telemetry_SetChannel(HIL_CH_TARGET_TEMP, (int32_t)dial_temp * 10);
      HIL_Telemetry_SetChannel(HIL_CH_CURRENT_TEMP, (int32_t)g_ui->current_temp * 10);
      HIL_Telemetry_SetChannel(HIL_CH_HEAP_FREE, (int32_t)xPortGetFreeHeapSize());
      g_boot_stage = 0x268U;
    }

    g_boot_stage = 0x27U;

    g_boot_stage = 0x271U;
    osDelay(1);
    g_boot_stage = 0x272U;
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
  * @brief  HIL Telemetry task — sends JSON frames at 100ms intervals
  * @param  argument: Not used
  */
void StartHILTelemetryTask(void const * argument)
{
  (void)argument;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(100);

  for (;;)
  {
    g_boot_stage = 0x280U;
    HIL_Telemetry_SendFrame();
    g_boot_stage = 0x281U;
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
}

/* USER CODE END Application */
