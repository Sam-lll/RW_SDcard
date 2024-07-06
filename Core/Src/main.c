/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdafx.h"
#include "usr_uart.h"
#include "usr_pwm.h"
#include "disk_opt.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
FATFS fs;
FIL fil;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;

extern RTC_HandleTypeDef hrtc;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart1;
extern uint8_t* strCurfile;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern uint8_t RxBuffer[RxBuffer_Size];
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
extern void SystemClock_Config(void);
extern void MX_GPIO_Init(void);
extern void MX_DMA_Init(void);
extern void MX_SPI1_Init(SPI_HandleTypeDef* hspi);
extern void MX_I2C1_Init(I2C_HandleTypeDef* hi2c);
extern void MX_RTC_Init(RTC_HandleTypeDef* hrtc);
extern void MX_USART1_UART_Init(UART_HandleTypeDef* huart);
extern void MX_TIM2_Init(TIM_HandleTypeDef* htim);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  SysTick_Config(72);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init(&hspi1);
  MX_FATFS_Init();
  MX_I2C1_Init(&hi2c1);
  MX_RTC_Init(&hrtc);
  MX_USART1_UART_Init(&huart1);
  MX_TIM2_Init(&htim2);
  /* USER CODE BEGIN 2 */
  
  HAL_Delay(500);
  // Start_Timer(&htim2);
  // USART_Init();
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuffer, RxBuffer_Size);
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
  
  ZssLog((unsigned char*)"test");
  char* strVersion = "V_1.0.8";
  SetExtFlag(0);
  SetlogFlag(0);
  SetUnlockFlag(0);
  SetUnlockFlagStep2(0);
  pwm_msc_init(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t sysClockFreq = HAL_RCC_GetSysClockFreq();
  USART_SendFormatString("sysClockFreq: %d\r\n", sysClockFreq);
  zssLogINFO("<%s  %d> %s(Ver: %s)", __FILE__, __LINE__, __FUNCTION__, strVersion);
  USART_SendFormatString("RW_SDCard %s start...\r\n", strVersion);
      
  USART_SendFormatString("Please enter command:\r\n");
  USART_SendFormatString("\
                          extract(list log file),\r\n \
                          xxx.log(show the log file in screen),\r\n \
                          delete(delete the log file),\r\n \
                          start(start the motor)\r\n");
  while (1)
  {

    if (GetExtFlag())		//if receive extract command
		{
      USART_SendFormatString("GetExtFlag: %d\r\n", GetExtFlag());
      ListFilesAndDirectories("");
      SetExtFlag(0);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
    if (GetlogFlag())
    {
      USART_SendFormatString("GetlogFlag: %d\r\n", GetlogFlag());
      SendLogContent(strCurfile);
      free(strCurfile);
      SetlogFlag(0);
    }
    if (GetStartFlag())
    {
      USART_SendFormatString("GetStartFlag: %d\r\n", GetStartFlag());
      pwm_msc_start(&htim2);
      SetStartFlag(0);
    }
  }
  
  return 1;
  /* USER CODE END 3 */
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

#ifdef  USE_FULL_ASSERT
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
