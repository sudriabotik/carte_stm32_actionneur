/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

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
#define end_switch_horizontal_Pin GPIO_PIN_1
#define end_switch_horizontal_GPIO_Port GPIOF
#define end_switch_vertical_Pin GPIO_PIN_2
#define end_switch_vertical_GPIO_Port GPIOC
#define curr_sens_M_D_Pin GPIO_PIN_3
#define curr_sens_M_D_GPIO_Port GPIOA
#define N_fault_D_Pin GPIO_PIN_4
#define N_fault_D_GPIO_Port GPIOA
#define DIR_D_Pin GPIO_PIN_5
#define DIR_D_GPIO_Port GPIOA
#define pwm_M_D_Pin GPIO_PIN_7
#define pwm_M_D_GPIO_Port GPIOA
#define pwm_M_G_Pin GPIO_PIN_5
#define pwm_M_G_GPIO_Port GPIOC
#define curr_sens_M_G_Pin GPIO_PIN_0
#define curr_sens_M_G_GPIO_Port GPIOB
#define N_fault_G_Pin GPIO_PIN_1
#define N_fault_G_GPIO_Port GPIOB
#define DIR_G_Pin GPIO_PIN_2
#define DIR_G_GPIO_Port GPIOB
#define V_Bus_Sensing_Pin GPIO_PIN_9
#define V_Bus_Sensing_GPIO_Port GPIOA
#define encoder_D_A_Pin GPIO_PIN_4
#define encoder_D_A_GPIO_Port GPIOB
#define encoder_D_B_Pin GPIO_PIN_5
#define encoder_D_B_GPIO_Port GPIOB
#define encoder_G_A_Pin GPIO_PIN_6
#define encoder_G_A_GPIO_Port GPIOB
#define encoder_G_B_Pin GPIO_PIN_7
#define encoder_G_B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
