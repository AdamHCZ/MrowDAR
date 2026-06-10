/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>
//		OLED
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "ssd1306_fonts.h"
#include "functions.h"
//					OLED Libraries
#include <stdbool.h>

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

I2C_HandleTypeDef 			hi2c1;
UART_HandleTypeDef 			huart1;
UART_HandleTypeDef			huart2;
TIM_HandleTypeDef   		TimInt2;


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t raw_data[6];
uint8_t error_state = 0;
uint8_t who = 0;
uint8_t comp_ok = 0;
int16_t x_acc;
int16_t y_acc;
int16_t z_acc;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void Error_Handler(void);

//		MPU6050
static void MPU6050_Init(void);

//		UART
void MX_USART2_MspInit(void);
static void MX_USART2_UART_Init(void);

static void MX_USART1_UART_Init(void);
void HAL_UART1_MspInit(UART_HandleTypeDef* uartHandle);

//		TIMER
void TIM2_IRQHandler(void);

//		MY_FUNCTIONS
void MPU6050_Values(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

	HAL_Init();
	SystemClock_Config();
		
	//		MPU6050
	MX_I2C_MspInit();
	MX_I2C1_Init();
	MPU6050_Init();
		
	//		UART
	MX_USART1_UART_Init();	

	MX_USART2_MspInit();
	MX_USART2_UART_Init();
		
	//		TIMER_Interrupt
	TIM_interrupt(1000,8000,TIM2,TIM2_IRQn,&TimInt2);
		
	char msg[] = "Hello from STM32 USART1\r\n";
	uint8_t rxData;

	while (1)
	{
//    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

//    /* Try to receive 1 byte */
//    if (HAL_UART_Receive(&huart1, &rxData, 1, 100) == HAL_OK)
//    {
//      HAL_UART_Transmit(&huart1, &rxData, 1, HAL_MAX_DELAY);
//    }
//    HAL_Delay(1000);
	}
}
	
void MPU6050_Values(){
	HAL_StatusTypeDef okey = HAL_I2C_IsDeviceReady(&hi2c1,MPU6050_ADDR,1,100);
	if (okey == HAL_OK) {
		comp_ok = 1;
	}else{
		comp_ok = 0;
	}
	HAL_I2C_Mem_Read(&hi2c1,MPU6050_ADDR,MPU6050_REG_ACCEL_XH,I2C_MEMADD_SIZE_8BIT,raw_data,6,100);
	x_acc = ((int16_t)raw_data[0] << 8) + raw_data[1];
	y_acc = ((int16_t)raw_data[2] << 8) + raw_data[3];
	z_acc = ((int16_t)raw_data[4] << 8) + raw_data[5];
	printf("%d,%d,%d \r\n", x_acc,y_acc,z_acc);
}

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimInt2);
	MPU6050_Values();
}

static void MPU6050_Init(void)
{
    uint8_t data;
		//			CONFIGURING SLEEPING MODE
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1,MPU6050_ADDR,MPU6050_REG_PWR_MGMT1,I2C_MEMADD_SIZE_8BIT,&data,1,100);
		//			SAMPLE RATE TO 1KHz
    data = 0x07;
    HAL_I2C_Mem_Write(&hi2c1,MPU6050_ADDR,MPU6050_REG_SMPLRT,I2C_MEMADD_SIZE_8BIT,&data,1,100);
		//			DLPF CONFIG
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1,MPU6050_ADDR,MPU6050_REG_CONFIG,I2C_MEMADD_SIZE_8BIT,&data,1,100);
		//			GYROSCOPE CONFIGURATION
    data = 0x08;
    HAL_I2C_Mem_Write(&hi2c1,MPU6050_ADDR,MPU6050_REG_GYRO_CFG,I2C_MEMADD_SIZE_8BIT,&data,1,100);
		//			ACCELEROMETER CONFIGURATION
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1,MPU6050_ADDR,MPU6050_REG_ACCEL_CFG,I2C_MEMADD_SIZE_8BIT,&data,1,100);
}

//-----------------------UART-----------------------//

static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;

  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart1) != HAL_OK) Error_Handler();
}

void HAL_UART1_MspInit(UART_HandleTypeDef* uartHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if ((*uartHandle).Instance == USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*
      USART1 GPIO Configuration
      PA9  ------> USART1_TX
      PA10 ------> USART1_RX
    */

    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void MX_USART2_MspInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    /*
      PA2 -> USART2_TX
      PA15 -> USART2_RX 		(VIRTUAL FOR USB PORT)
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;

    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}

void MX_I2C_MspInit(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct;
	/**I2C1 GPIO Configuration    
	PB6(D5)     ------> I2C1_SCL
	PB7(D4)     ------> I2C1_SDA 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void MX_I2C1_Init(void)
{
  hi2c1.Instance 								= I2C1;
  hi2c1.Init.Timing       	    = 0xE0330309;
  hi2c1.Init.OwnAddress1 				= 0;
	hi2c1.Init.OwnAddress2Masks		= I2C_OA2_NOMASK;
  hi2c1.Init.AddressingMode 		= I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode 		= I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 				= 0;
  hi2c1.Init.GeneralCallMode 		= I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode 			= I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) 		 != HAL_OK)
  {
    Error_Handler();
  }

}

void Error_Handler(void)
{
  while (1)
  {
		error_state = 1;
  }
	error_state = 0;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
