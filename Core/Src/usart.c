/**
  ******************************************************************************
  * @file           : usart.c
  * @brief          : UART1 Initialization for HIL Telemetry
  * @board          : Qiming Xingxin STM32F407 V6.1
  * @pins           : PA9 (TX), PA10 (RX) — connected to MAX232 RS232
  * @baud           : 115200-8N1, no flow control
  ******************************************************************************
  */

#include "usart.h"
#include "main.h"

/* UART1 handle — shared with hil_telemetry task */
UART_HandleTypeDef huart1;

/**
  * @brief  Initialize UART1 for HIL telemetry output
  * @note   PA9=TX, PA10=RX (AF7). 115200-8N1. No interrupts, polling TX.
  *         Uses MAX232 level shifter for PC RS232 connection.
  */
void MX_USART1_UART_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 1. Enable clocks */
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* 2. Configure PA9 (TX) and PA10 (RX) as AF7 (USART1) */
    /* PA9 — USART1_TX */
    GPIO_InitStruct.Pin       = GPIO_PIN_9;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA10 — USART1_RX */
    GPIO_InitStruct.Pin       = GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* 3. Configure UART1 peripheral */
    huart1.Instance          = USART1;
    huart1.Init.BaudRate     = 115200;
    huart1.Init.WordLength   = UART_WORDLENGTH_8B;
    huart1.Init.StopBits     = UART_STOPBITS_1;
    huart1.Init.Parity       = UART_PARITY_NONE;
    huart1.Init.Mode         = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief  Transmit a single byte over UART1 (blocking, for debug printf)
  * @param  ch: character to send
  * @note   Used by retarget printf (__io_putchar) for debug console.
  */
int __io_putchar(int ch)
{
    if (ch == '\n')
    {
        /* Send \r before \n for proper terminal line ending */
        uint8_t cr = '\r';
        HAL_UART_Transmit(&huart1, &cr, 1, HAL_MAX_DELAY);
    }
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}