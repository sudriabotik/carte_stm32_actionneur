/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "mv_statemachine.h"
#include "mv_statemachine_states.h"
#include "coordonne_absolue.h"
#include "elevator_states.h"
#include "sequencer.h"
#include "robot_sequences.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint32_t counter_test = 0;
uint32_t counter_test_2 = 0;
int toggle_test = 0;
struct Sequencer main_sequencer;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* 
  //printf("irq_tim2_interrupt %lu\n", counter_test);
  MSM_update(&MV_STATEMACHINE, 10);

  if (MSM_is_busy(&MV_STATEMACHINE) == 0)
  {
      printf("not busy\n");
      //if (toggle) MSM_begin_recalibration(1, 15000, 20, POSITIVE_X);
      
      //MSM_reset_construction(&MV_STATEMACHINE);
      //MSM_enqueue_state(&MV_STATEMACHINE, &MV_STATE_TRANSLATION, genenv_mv_state_translation(0.001, 0.8, -500));
      //MSM_enqueue_state(&MV_STATEMACHINE, &MV_STATE_ROTATION, genenv_mv_state_rotation(0.001, 0.2, 180));
      //MSM_enqueue_state(&MV_STATEMACHINE, &MV_STATE_TRANSLATION, genenv_mv_state_translation(0.001, 0.8, -500));
      //MSM_enqueue_state(&MV_STATEMACHINE, &MV_STATE_HOLD, genenv_mv_state_hold());
      //MSM_ready_construction(&MV_STATEMACHINE);
      
     if (counter_test % 2 == 0)
     {
      Point2D target = {.x = 200, .y = 500};
      goto_xy(target, 0.8, 0.001, 0.1, 0.001, FACE_ARRIERE);
     }
     else
     {
      Point2D target = {.x = 0, .y = 0};
      goto_xy(target, 0.8, 0.001, 0.1, 0.001, FACE_AVANT);
     }
     counter_test ++;
  }
  */
  MSM_update(&elevator_V_statemachine, 0.020f);
  MSM_update(&elevator_H_statemachine, 0.020f);
  sequencer_update(&main_sequencer);
  
  /* 
  if (MSM_is_busy(&elevator_V_statemachine) == 0)
  {
    if (counter_test == 0)
    {
      counter_test ++;
      printf("counter_test == 1");
      MSM_reset_construction(&elevator_V_statemachine);
      MSM_enqueue_state(&elevator_V_statemachine, &ELV_STATE_HOME_V, genenv_elv_home_v(10.0f));
      //MSM_enqueue_state(&elevator_V_statemachine, &ELV_STATE_MOVE_V, genenv_elv_move_v(30.0f, 15.0f, -40.0f));
      MSM_enqueue_state(&elevator_V_statemachine, &ELV_STATE_HOLD_V, genenv_elv_hold_v());
      MSM_ready_construction(&elevator_V_statemachine);
    }
  }
    */ 
  
  /*
    if (MSM_is_busy(&elevator_H_statemachine) == 0)
  {
    if (counter_test_2 == 0)
    {
      counter_test_2 ++;
      printf("counter_test == 1");
      MSM_reset_construction(&elevator_H_statemachine);

      MSM_enqueue_state(&elevator_H_statemachine, &ELV_STATE_HOME_H, genenv_elv_home_h(-10.0f));
      MSM_enqueue_state(&elevator_H_statemachine, &ELV_STATE_MOVE_H, genenv_elv_move_h(20.0f, 10.0f, 20.0f));
      MSM_enqueue_state(&elevator_H_statemachine, &ELV_STATE_HOLD_H, genenv_elv_hold_h());
      
      MSM_ready_construction(&elevator_H_statemachine);
    }
  }
	*/
  

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt / USART3 wake-up interrupt through EXTI line 28.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC3 channel underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
