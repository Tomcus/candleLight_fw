/**
  ******************************************************************************
  * @file    stm32h5xx_hal_conf.h
  * @author  MCD Application Team
  * @brief   HAL configuration file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32H5xx_HAL_CONF_H
#define STM32H5xx_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the HAL driver
  */
#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_FDCAN_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_PCD_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED

/* ########################## Register Callbacks selection ############################## */
/**
  * @brief This is the list of modules where register callback can be used
  */
#define USE_HAL_FDCAN_REGISTER_CALLBACKS  0u
#define USE_HAL_PCD_REGISTER_CALLBACKS    0u
#define USE_HAL_TIM_REGISTER_CALLBACKS    0u

/* ########################## Oscillator Values adaptation ####################*/
/**
  * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
  */
#if !defined  (HSE_VALUE)
#define HSE_VALUE             8000000UL  /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT   100UL      /*!< Time out for HSE start up, in ms */
#endif /* HSE_STARTUP_TIMEOUT */

/**
  * @brief Internal Low Power oscillator (CSI) default value.
  */
#if !defined  (CSI_VALUE)
#define CSI_VALUE             4000000UL  /*!< Value of the Internal oscillator in Hz */
#endif /* CSI_VALUE */

/**
  * @brief Internal High Speed oscillator (HSI) value.
  */
#if !defined  (HSI_VALUE)
#define HSI_VALUE             64000000UL /*!< Value of the Internal oscillator in Hz */
#endif /* HSI_VALUE */

/**
  * @brief Internal High Speed oscillator (HSI48) value for USB.
  *        Trimmed at runtime against the USB SOF by the CRS.
  */
#if !defined  (HSI48_VALUE)
#define HSI48_VALUE           48000000UL /*!< Value of the Internal High Speed oscillator for USB in Hz */
#endif /* HSI48_VALUE */

/**
  * @brief Internal Low Speed oscillator (LSI) value.
  */
#if !defined  (LSI_VALUE)
#define LSI_VALUE             32000UL    /*!< LSI Typical Value in Hz */
#endif /* LSI_VALUE */

/**
  * @brief External Low Speed oscillator (LSE) value.
  */
#if !defined  (LSE_VALUE)
#define LSE_VALUE             32768UL    /*!< Value of the External oscillator in Hz */
#endif /* LSE_VALUE */

#if !defined  (LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT   5000UL     /*!< Time out for LSE start up, in ms */
#endif /* LSE_STARTUP_TIMEOUT */

#if !defined  (EXTERNAL_CLOCK_VALUE)
#define EXTERNAL_CLOCK_VALUE  12288000UL /*!< Value of the External clock source in Hz */
#endif /* EXTERNAL_CLOCK_VALUE */

/* ########################### System Configuration ######################### */
/**
  * @brief This is the HAL system configuration section
  */
#define  VDD_VALUE            3300UL /*!< Value of VDD in mv */
#define  TICK_INT_PRIORITY    ((1UL << __NVIC_PRIO_BITS) - 1UL) /*!< tick interrupt priority */
#define  USE_RTOS             0U
#define  PREFETCH_ENABLE      1U

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include modules header file
  */

#ifdef HAL_RCC_MODULE_ENABLED
#include "stm32h5xx_hal_rcc.h"
#endif /* HAL_RCC_MODULE_ENABLED */

#ifdef HAL_GPIO_MODULE_ENABLED
#include "stm32h5xx_hal_gpio.h"
#endif /* HAL_GPIO_MODULE_ENABLED */

#ifdef HAL_CORTEX_MODULE_ENABLED
#include "stm32h5xx_hal_cortex.h"
#endif /* HAL_CORTEX_MODULE_ENABLED */

#ifdef HAL_FDCAN_MODULE_ENABLED
#include "stm32h5xx_hal_fdcan.h"
#endif /* HAL_FDCAN_MODULE_ENABLED */

#ifdef HAL_FLASH_MODULE_ENABLED
#include "stm32h5xx_hal_flash.h"
#endif /* HAL_FLASH_MODULE_ENABLED */

#ifdef HAL_PCD_MODULE_ENABLED
#include "stm32h5xx_hal_pcd.h"
#endif /* HAL_PCD_MODULE_ENABLED */

#ifdef HAL_PWR_MODULE_ENABLED
#include "stm32h5xx_hal_pwr.h"
#endif /* HAL_PWR_MODULE_ENABLED */

#ifdef HAL_TIM_MODULE_ENABLED
#include "stm32h5xx_hal_tim.h"
#endif /* HAL_TIM_MODULE_ENABLED */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for functions parameters check.
  */
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
}
#endif

#endif /* STM32H5xx_HAL_CONF_H */
