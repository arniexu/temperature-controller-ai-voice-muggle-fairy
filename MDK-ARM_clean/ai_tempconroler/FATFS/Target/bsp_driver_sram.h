/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * @file    bsp_driver_sram.h (based on stm32469i_eval_sram.h)
  * @brief   This file contains the common defines and functions prototypes for
  *          the bsp_driver_sram.c driver.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_SRAM_H
#define __STM32F4_SRAM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_sram.h"

/* USER CODE BEGIN 0 */

/**
  * @brief  SRAM status structure definition
  */
#define SRAM_OK            ((uint8_t)0x00)
#define SRAM_ERROR         ((uint8_t)0x01)

#define SRAM_DEVICE_ADDR   ((uint32_t)0x64000000)
#define SRAM_DEVICE_SIZE   ((uint32_t)0x200000)  /* SRAM device size in MBytes */

#define SRAM_MEMORY_WIDTH  FMC_NORSRAM_MEM_BUS_WIDTH_16

/* Other defines, such as the ones for SRAM DMA transfer, can be added here */

/* Exported functions --------------------------------------------------------*/
uint8_t BSP_SRAM_Init(void);
uint8_t BSP_SRAM_ReadData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t BSP_SRAM_ReadData_DMA(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t BSP_SRAM_WriteData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t BSP_SRAM_WriteData_DMA(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
void BSP_SRAM_DMA_IRQHandler(void);

/* USER CODE END 0 */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_SRAM_H */
