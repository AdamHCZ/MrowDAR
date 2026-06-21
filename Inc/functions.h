/**
  ******************************************************************************
  * @file    Templates/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_32.h"

/* Exported types ------------------------------------------------------------*/

typedef struct
{
    float x;             // Estimated value
    float P;             // Estimation error
    float Q;             // Process noise
    float R;             // Measurement noise
    unsigned char init;  // Initialization flag
} KalmanFilter_t;

void Kalman_Init(KalmanFilter_t *kf, float Q, float R, float initial_P);
float Kalman_Update(KalmanFilter_t *kf, float measurement);

/* Exported constants --------------------------------------------------------*/

//		MATH
#define PI 3.14159265f
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

//---------------------OLED---------------------------//
//void MX_I2C_MspInit(void);
//void MX_I2C1_Init(void);
//---------------------OLED---------------------------//

//-----------------INTERRUPTIONS----------------------//
void IT_IRQHandler_Config(uint32_t, uint32_t, uint16_t, GPIO_TypeDef *, IRQn_Type);	                                       	//MODE - PULL - PIN - BLOCK - INTERRUPT
void TIM_interrupt(uint32_t,uint32_t,TIM_TypeDef *,IRQn_Type, TIM_HandleTypeDef *);                                         //PERIOD - PREESCALER - TIMER - INTERRUPT - DEFINE_TIMER
//-----------------INTERRUPTIONS----------------------//

//------------------DEFINE_PINS-----------------------//
void PIN_DEF(uint32_t, uint32_t, uint16_t, GPIO_TypeDef *, uint32_t);					        	                              			//MODE - PULL - PIN - BLOCK - ALTER (0 as default)
//------------------DEFINE_PINS-----------------------//

//-------------------DEFINE_PWM-----------------------//
void Define_PWM(TIM_TypeDef *, uint32_t , uint32_t , uint32_t , uint32_t , TIM_HandleTypeDef *);				                		//TIMER - CHANNEL - PREESCALER - PERIOD - PULSE
//-------------------DEFINE_PWM-----------------------//

//-------------------DEFINE_ADC-----------------------//
void Define_ADC(ADC_TypeDef *, uint32_t, uint32_t, uint32_t, ADC_HandleTypeDef *, ADC_ChannelConfTypeDef *);								//ADCX - RESOLUTION - CHANNEL - SAMPLETIME
//-------------------DEFINE_ADC-----------------------//

#endif /* __FUNCTIONS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
