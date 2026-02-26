/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "adc.h"
#include "fdcan.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
# include "stdio.h"
# include <inttypes.h>
//# include "movement_statemachine.h"
# include "robot_data.h"
# include "motors.h"
# include "motor_asserv.h"
# include "pid_config.h"
# include "recorder.h"
# include "ax_controller.h"
# include "mv_statemachine.h"
# include "mv_statemachine_states.h"
# include "elevator_states.h"
# include "sequencer.h"
# include "robot_sequences.h"

#include "CO_app_STM32.h"
#include "OD.h"
#include "can_debug.h"
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

/* USER CODE BEGIN PV */
GPIO_PinState pin_a;
GPIO_PinState pin_b;
int trig_count = 0;
bool toggle = false;

// Variables pour la réception CAN bas niveau
FDCAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
# define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, 0xFFFF);
	return ch;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_FDCAN1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_UART4_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_TIM6_Init();
  MX_I2C3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); // right
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL); // left

  HAL_TIM_PWM_Init(&htim1); // init the pwm for the motors
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_4);

  ////////////
  // CONFIG CAN //
  //////////
  CANopenNodeSTM32 canopenNodeSTM32;
  canopenNodeSTM32.CANHandle = &hfdcan1;
  canopenNodeSTM32.HWInitFunction = MX_FDCAN1_Init;
  canopenNodeSTM32.timerHandle = &htim6;
  canopenNodeSTM32.desiredNodeID = 2;
  canopenNodeSTM32.baudrate = 500; //  ce parametre ne ser à rien, c'est comme le tuto. 
  canopen_app_init(&canopenNodeSTM32);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);


  robot_data_init();

  MV_STATEMACHINE = MSM_init();
  elevator_V_statemachine = MSM_init();
  elevator_H_statemachine = MSM_init();

  extern struct Sequencer main_sequencer;
  main_sequencer = sequencer_init();
  sequencer_set_idle(&main_sequencer, &elevator_V_statemachine, &ELV_STATE_HOLD_V, genenv_elv_hold_v());
  sequencer_set_idle(&main_sequencer, &elevator_H_statemachine, &ELV_STATE_HOLD_H, genenv_elv_hold_h());

  seq_build_homing_all(&main_sequencer, 10.0f, -10.0f);
  sequencer_start(&main_sequencer);

  HAL_Delay(500);

  // Enable CAN debug logging
  printf("=== CAN Debug Initialized ===\n");
  can_debug_enable_rx_logging(true);  // Set to false to disable automatic logging
  can_debug_print_canopen_detailed(&canopenNodeSTM32);

  HAL_TIM_Base_Start_IT(&htim2);

  static int action_id = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    canopen_app_process();
    HAL_GPIO_WritePin(led_can_1_GPIO_Port, led_can_1_Pin,!canopenNodeSTM32.outStatusLEDGreen);
    HAL_GPIO_WritePin(led_can_2_GPIO_Port, led_can_2_Pin,!canopenNodeSTM32.outStatusLEDRed);

    if (action_id != OD_RAM.x2000_ACTION_ID)
    {
      printf("ActionID %d \r\n",OD_RAM.x2000_ACTION_ID);
    }
    can_debug_monitor_rpdo_changes();

    /* 
    printf("RPDO1: ACTION_ID=%u p1=%d p2=%d p3=%d\n\r", 
       OD_RAM.x2000_ACTION_ID, 
       OD_RAM.x2001_param_1,
       OD_RAM.x2002_param_2,
       OD_RAM.x2003_param_3);
    */
    //printf("ActionID %d \r\n",OD_RAM.x2000_ACTION_ID);
    /* 
    // Test bas niveau : vérifier si une trame CAN est reçue
    if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) > 0)
    {
      // Lire le message
      if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
      {
        // Afficher la trame reçue
        printf("[HAL_CAN] ID=0x%03lX DLC=%lu DATA=[ ", RxHeader.Identifier,
               (RxHeader.DataLength >> 16));
        for (uint8_t i = 0; i < 8; i++) {
          printf("%02X ", RxData[i]);
        }
        printf("]\n");
      }
    }
    */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    //Encoder16Update(&encoder_R);

    /*
    printf("R pos ");
    printf("%"PRIi32"\n", encoder_R.total_count);
    printf("delta %"PRIi32"\n", encoder_R.total_count_delta);
    printf("pwm ccr %"PRIu32"\n", *motor_R.pwm_ccr);
    printf("R rollovers %"PRIi32"\n", encoder_R.rollover_count);
    printf("R situation %"PRIu32"\n", encoder_R.situation);
    printf("R encoder register %"PRIu32"\n", TIM3->CNT);

    printf("L pos ");
    printf("%"PRIi32"\n", encoder_L.total_count);
    printf("delta %"PRIi32"\n", encoder_L.total_count_delta);
    printf("pwm ccr %"PRIu32"\n", *motor_L.pwm_ccr);
    printf("L rollovers %"PRIi32"\n", encoder_L.rollover_count);
    printf("L situation %"PRIu32"\n", encoder_L.situation);
    printf("L encoder register %"PRIu32"\n", TIM4->CNT);
    */

    /*
    if (trig_count == 25)
    {
      printf("Encoder R : \n");
      Encoder16PrintStatus(encoder_R);
      printf("Encoder L : \n");
      Encoder16PrintStatus(encoder_L);
      trig_count = 0;
    }
    */

    //printf("a\n");
    //ax_write_position(2, 20);

    /*
    if (recorder_is_track_full(0))
    {
      recorder_print_recorded_data(0);
      HAL_Delay(20000);
    }
    */

    //MSM_update(100);
    //HAL_UART_Transmit(&huart4, "m\n", 2, 1000);
    //movement_statemachine_update();
    //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    //Encoder16Update(&encoder_R);
    //motor_drive_pid(1000, 0.0f, motor_R, encoder_R, pid_motor_R, &pid_motor_R_runtime);
    //motor_drive(motor_R, -20);
    //MSM_update(10);

    /*
    if (MSM_busy() == 0)
    {
      //if (toggle) MSM_begin_recalibration(1, 15000, 20, POSITIVE_X);
      if (toggle) MSM_begin_translation(500, 0.02, 0.00005);
      else MSM_begin_translation(-500, 0.02, 0.00005);
      toggle = !toggle;
    }
      */
    //printf("R encoder register %"PRIu32"\n\r", TIM3->CNT);
    trig_count ++;
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
