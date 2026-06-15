/**
  ******************************************************************************
  * @file           : usart.h
  * @brief          : UART1 header for HIL Telemetry
  ******************************************************************************
  */

#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/* Exported handle — shared with hil_telemetry task */
extern UART_HandleTypeDef huart1;

/* Function prototypes */
void MX_USART1_UART_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H */