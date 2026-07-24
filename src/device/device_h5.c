/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2026 Prusa Research
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "can.h"
#include "device.h"
#include "hal_include.h"

/*
 * Crystal-less clock tree:
 *   HSI (64 MHz) -> PLL1 /8 *60 -> VCO 480 MHz
 *     PLL1P /2 -> 240 MHz SYSCLK
 *     PLL1Q /10 -> 48 MHz FDCAN kernel clock
 *   HSI48 -> USB, trimmed against the USB SOF by the CRS
 */
void device_sysclock_config(void)
{
	/* Voltage scale 0 is required to run the core at 240 MHz. */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0);

	RCC_OscInitTypeDef RCC_OscInitStruct = {
		.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48,
		.HSIState = RCC_HSI_ON,
		.HSIDiv = RCC_HSI_DIV1,
		.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT,
		.HSI48State = RCC_HSI48_ON,
		.PLL = {
			.PLLState = RCC_PLL_ON,
			.PLLSource = RCC_PLL1_SOURCE_HSI,
			.PLLM = 8,
			.PLLN = 60,
			.PLLP = 2,
			.PLLQ = 10,
			.PLLR = 2,
			.PLLRGE = RCC_PLL1_VCIRANGE_3,
			.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE,
			.PLLFRACN = 0,
		},
	};
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/* OscConfig only enables PLL1P; FDCAN needs the PLL1Q output too. */
	__HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);

	RCC_ClkInitTypeDef RCC_ClkInitStruct = {
		.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
					 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3,
		.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
		.AHBCLKDivider = RCC_SYSCLK_DIV1,
		.APB1CLKDivider = RCC_HCLK_DIV1,
		.APB2CLKDivider = RCC_HCLK_DIV1,
		.APB3CLKDivider = RCC_HCLK_DIV1,
	};
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

	RCC_PeriphCLKInitTypeDef PeriphClkInit = {
		.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_FDCAN,
		.UsbClockSelection = RCC_USBCLKSOURCE_HSI48,
		.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL1Q,
	};
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	/* Trim HSI48 against the USB start-of-frame so USB works without a crystal. */
	__HAL_RCC_CRS_CLK_ENABLE();
	RCC_CRSInitTypeDef RCC_CRSInitStruct = {
		.Prescaler = RCC_CRS_SYNC_DIV1,
		.Source = RCC_CRS_SYNC_SOURCE_USB,
		.Polarity = RCC_CRS_SYNC_POLARITY_RISING,
		.ReloadValue = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000),
		.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT,
		.HSI48CalibrationValue = RCC_CRS_HSI48CALIBRATION_DEFAULT,
	};
	HAL_RCCEx_CRSConfig(&RCC_CRSInitStruct);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* Called by HAL_FDCAN_Init(); brings up the FDCAN clock and the PB7/PB8 pins. */
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan)
{
	(void)hfdcan;

	__HAL_RCC_FDCAN_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {
		.Pin = GPIO_PIN_7 | GPIO_PIN_8,
		.Mode = GPIO_MODE_AF_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate = GPIO_AF9_FDCAN1,
	};
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
