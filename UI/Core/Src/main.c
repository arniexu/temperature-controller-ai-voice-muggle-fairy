/**
 * @file main.c
 * @brief Tianji Thermostat - Main Entry Point for STM32F407
 * @note  Assumes STM32CubeF4 HAL framework with FreeRTOS (optional)
 *        Hardware: STM32F407VG + 240x320 TFT LCD + Touch (XPT2046)
 */

#include "main.h"
#include "cmsis_os.h"
#include "tianji_ui.h"
#include "display_drv.h"
#include "touch_drv.h"
#include "lvgl.h"

/*=================================================================*/
/* Private variables                                               */
/*=================================================================*/

static tianji_ui_t *g_ui = NULL;
static uint32_t last_lvgl_tick = 0;

/*=================================================================*/
/* LVGL Tick Provider (1ms)                                        */
/*=================================================================*/

void HAL_SYSTICK_Callback(void)
{
    lv_tick_inc(1);
}

/*=================================================================*/
/* System Clock Configuration (for STM32F407VG)                    */
/*=================================================================*/

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Enable HSE Oscillator */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;       /* HSE = 8MHz */
    RCC_OscInitStruct.PLL.PLLN = 336;     /* VCO = 336MHz */
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;  /* SYSCLK = 168MHz */
    RCC_OscInitStruct.PLL.PLLQ = 7;       /* USB = 48MHz */
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Configure bus clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                   RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;   /* HCLK = 168MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;    /* PCLK1 = 42MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;    /* PCLK2 = 84MHz */
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/*=================================================================*/
/* HAL MSP Initialization                                          */
/*=================================================================*/

void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

/*=================================================================*/
/* LVGL Periodic Handler (called from main loop or timer)          */
/*=================================================================*/

static void lvgl_handler(void)
{
    uint32_t now = HAL_GetTick();
    if (now - last_lvgl_tick >= 5) {  /* Every 5ms */
        last_lvgl_tick = now;
        lv_timer_handler();
    }
}

/*=================================================================*/
/* Periodic UI Updates                                             */
/*=================================================================*/

static void ui_update_task(void *argument)
{
    (void)argument;
    const TickType_t xFrequency = pdMS_TO_TICKS(200); /* 5Hz update */
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        if (g_ui) {
            /* Sync temp from dial to UI */
            int16_t dial_temp = tianji_dial_get_temp(g_ui->dial);
            if (dial_temp != g_ui->current_temp) {
                g_ui->current_temp = dial_temp;
                tianji_ui_update_status(g_ui);
            }
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/*=================================================================*/
/* Main Entry Point                                                */
/*=================================================================*/

int main(void)
{
    /* MCU Configuration */
    HAL_Init();
    SystemClock_Config();

    /* Initialize LVGL */
    lv_init();

    /* Initialize display driver (BSP integration) */
    if (display_drv_init() != 0) {
        Error_Handler();
    }

    /* Initialize touch driver (BSP integration) */
    if (touch_drv_init() != 0) {
        Error_Handler();
    }

    /* Calibrate touchscreen (first boot or on demand) */
    /* touch_drv_calibrate(); */

    /* Create the Tianji UI */
    g_ui = tianji_ui_init();
    if (!g_ui) {
        Error_Handler();
    }

    /* Set initial backlight */
    display_drv_set_backlight(85);

    /* Create UI update task (FreeRTOS) */
    /* If not using FreeRTOS, call updates from main loop */
    xTaskCreate(ui_update_task, "UI_Update", 256, NULL, 2, NULL);

    /* Start scheduler */
    osKernelStart();

    /* Should never reach here */
    for (;;) {
        /* Fallback: simple loop without RTOS */
        lvgl_handler();
        HAL_Delay(5);
    }
}

/*=================================================================*/
/* Error Handler                                                   */
/*=================================================================*/

void Error_Handler(void)
{
    __disable_irq();
    /* Turn on LED or enter safe mode */
    while (1) {
        HAL_Delay(500);
    }
}

/*=================================================================*/
/* Assert Failed                                                   */
/*=================================================================*/

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
    Error_Handler();
}
#endif
