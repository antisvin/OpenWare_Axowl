/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED2_Pin GPIO_PIN_8
#define LED2_GPIO_Port GPIOB
#define S1_Pin GPIO_PIN_5
#define S1_GPIO_Port GPIOB
#define S1_EXTI_IRQn EXTI9_5_IRQn
#define USB_POWERSW_Pin GPIO_PIN_7
#define USB_POWERSW_GPIO_Port GPIOD
#define LED3_Pin GPIO_PIN_9
#define LED3_GPIO_Port GPIOB
#define USBD_DP_Pin GPIO_PIN_12
#define USBD_DP_GPIO_Port GPIOA
#define USBD_DM_Pin GPIO_PIN_11
#define USBD_DM_GPIO_Port GPIOA
#define S2_Pin GPIO_PIN_10
#define S2_GPIO_Port GPIOA
#define S2_EXTI_IRQn EXTI15_10_IRQn
#define LED_RED_Pin GPIO_PIN_6
#define LED_RED_GPIO_Port GPIOC
#define LED_GREEN_Pin GPIO_PIN_6
#define LED_GREEN_GPIO_Port GPIOG
#define SWJOY_Pin GPIO_PIN_0
#define SWJOY_GPIO_Port GPIOC
#define SWJOY_EXTI_IRQn EXTI0_IRQn
#define SW1_Pin GPIO_PIN_1
#define SW1_GPIO_Port GPIOC
#define SW1_EXTI_IRQn EXTI1_IRQn
#define SW2_Pin GPIO_PIN_2
#define SW2_GPIO_Port GPIOC
#define SW2_EXTI_IRQn EXTI2_IRQn
#define SW3_Pin GPIO_PIN_3
#define SW3_GPIO_Port GPIOC
#define SW3_EXTI_IRQn EXTI3_IRQn
#define SD_DETECT_Pin GPIO_PIN_13
#define SD_DETECT_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_0
#define LED4_GPIO_Port GPIOB
#define USBH_DM_Pin GPIO_PIN_14
#define USBH_DM_GPIO_Port GPIOB
#define USBH_DP_Pin GPIO_PIN_15
#define USBH_DP_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
