/*

        FUNCTIONS TO CONFIGURATE PWM, PINS, ADC, INTERRUPTIONS

*/
#include "functions.h"

void Error_func(void);

static void Define_ADC(ADC_TypeDef *ADCx, uint32_t RESOLUTION, uint32_t CHANNEL, uint32_t SAMPLE_TIME, ADC_HandleTypeDef *ADC_CLASS, ADC_ChannelConfTypeDef *ADC_CONFIG)
{
    (*ADC_CLASS).Instance = ADCx;
		__HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_SYSCLK);

    if (ADCx == ADC1)   __HAL_RCC_ADC_CLK_ENABLE();

    if (HAL_ADC_DeInit(ADC_CLASS) != HAL_OK)																		Error_func();

    (*ADC_CLASS).Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
    (*ADC_CLASS).Init.Resolution            = RESOLUTION;
    (*ADC_CLASS).Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    (*ADC_CLASS).Init.ScanConvMode          = DISABLE;
    (*ADC_CLASS).Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    (*ADC_CLASS).Init.LowPowerAutoWait      = DISABLE;
    (*ADC_CLASS).Init.ContinuousConvMode    = DISABLE;
    (*ADC_CLASS).Init.NbrOfConversion       = 1;
    (*ADC_CLASS).Init.DiscontinuousConvMode = DISABLE;
    (*ADC_CLASS).Init.NbrOfDiscConversion   = 1;
    (*ADC_CLASS).Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    (*ADC_CLASS).Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    (*ADC_CLASS).Init.DMAContinuousRequests = DISABLE;
    (*ADC_CLASS).Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
    (*ADC_CLASS).Init.OversamplingMode      = DISABLE;

    if (HAL_ADC_Init(ADC_CLASS) != HAL_OK)														        	        Error_func();

    (*ADC_CONFIG).Channel      = CHANNEL;
    (*ADC_CONFIG).Rank         = ADC_REGULAR_RANK_1;
    (*ADC_CONFIG).SamplingTime = SAMPLE_TIME;
    (*ADC_CONFIG).SingleDiff   = ADC_SINGLE_ENDED;
    (*ADC_CONFIG).OffsetNumber = ADC_OFFSET_NONE;
    (*ADC_CONFIG).Offset       = 0;

    if (HAL_ADC_ConfigChannel(ADC_CLASS, ADC_CONFIG) != HAL_OK)							                    		Error_func();

    if (HAL_ADCEx_Calibration_Start(ADC_CLASS, ADC_SINGLE_ENDED) != HAL_OK)                                     	Error_func();

    if (HAL_ADC_Start(ADC_CLASS) != HAL_OK)					        				            					Error_func();

    if (HAL_ADC_PollForConversion(ADC_CLASS, 10) != HAL_OK)  						                    			Error_func();
}

static void PIN_DEF(uint32_t MODE, uint32_t PULL, uint16_t PIN, GPIO_TypeDef *BLOCK, uint32_t ALTER)
{
    GPIO_InitTypeDef GPIO_PINX = {0};

    if (BLOCK == GPIOB){
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}else if(BLOCK == GPIOA){
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}

    GPIO_PINX.Pin = PIN;
    GPIO_PINX.Mode = MODE;
    GPIO_PINX.Pull = PULL;

    if (MODE == GPIO_MODE_OUTPUT_PP ||
        MODE == GPIO_MODE_OUTPUT_OD ||
        MODE == GPIO_MODE_AF_PP ||
        MODE == GPIO_MODE_AF_OD)
    {
        GPIO_PINX.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    }

    if (MODE == GPIO_MODE_AF_PP ||
        MODE == GPIO_MODE_AF_OD)
    {
        GPIO_PINX.Alternate = ALTER;
    }

    HAL_GPIO_Init(BLOCK, &GPIO_PINX);
}

static void Define_PWM(TIM_TypeDef *TIMX, uint32_t CHANNEL, uint32_t PRESCALER, uint32_t PERIOD, uint32_t PULSE, TIM_HandleTypeDef *TIM_PWM)
{
    (*TIM_PWM).Instance = TIMX;

    (*TIM_PWM).Init.Period = PERIOD - 1;
    (*TIM_PWM).Init.Prescaler = PRESCALER - 1;
    (*TIM_PWM).Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    (*TIM_PWM).Init.CounterMode = TIM_COUNTERMODE_UP;
    (*TIM_PWM).Init.RepetitionCounter = 0;

    if (TIMX == TIM1)      __HAL_RCC_TIM1_CLK_ENABLE();
		else if (TIMX == TIM2) __HAL_RCC_TIM2_CLK_ENABLE();
		else if (TIMX == TIM6) __HAL_RCC_TIM6_CLK_ENABLE();
		else if (TIMX == TIM7) __HAL_RCC_TIM7_CLK_ENABLE();
		else if (TIMX == TIM15)__HAL_RCC_TIM15_CLK_ENABLE();
		else if (TIMX == TIM16)__HAL_RCC_TIM16_CLK_ENABLE();

    if (HAL_TIM_PWM_Init(TIM_PWM) != HAL_OK)                                                                         Error_func();

    TIM_OC_InitTypeDef TIMConf = {0};

    TIMConf.OCMode = TIM_OCMODE_PWM1;
    TIMConf.OCPolarity = TIM_OCPOLARITY_HIGH;
    TIMConf.OCFastMode = TIM_OCFAST_DISABLE;
    TIMConf.Pulse = PULSE;

    if (HAL_TIM_PWM_ConfigChannel(TIM_PWM, &TIMConf, CHANNEL) != HAL_OK)                                             Error_func();
    if (HAL_TIM_PWM_Start(TIM_PWM, CHANNEL) != HAL_OK)                                                               Error_func();
}

void TIM_interrupt(uint32_t PERIOD_VAL,uint32_t PREESCALER, TIM_TypeDef *TIMER,IRQn_Type TIM_INTER, TIM_HandleTypeDef *TIMdef){
  (*TIMdef).Instance = TIMER;

  (*TIMdef).Init.Period            = PERIOD_VAL - 1;
  (*TIMdef).Init.Prescaler         = PREESCALER - 1;
  (*TIMdef).Init.ClockDivision     = 0;
  (*TIMdef).Init.CounterMode       = TIM_COUNTERMODE_UP;
  (*TIMdef).Init.RepetitionCounter = 0;

	if (TIMER == TIM1)      __HAL_RCC_TIM1_CLK_ENABLE();
	else if (TIMER == TIM2) __HAL_RCC_TIM2_CLK_ENABLE();
	else if (TIMER == TIM6) __HAL_RCC_TIM6_CLK_ENABLE();
	else if (TIMER == TIM7) __HAL_RCC_TIM7_CLK_ENABLE();
	else if (TIMER == TIM15)__HAL_RCC_TIM15_CLK_ENABLE();
	else if (TIMER == TIM16)__HAL_RCC_TIM16_CLK_ENABLE();
  
  HAL_NVIC_SetPriority(TIM_INTER, 3, 0);
  HAL_NVIC_EnableIRQ(TIM_INTER);

  if (HAL_TIM_Base_Init(TIMdef) != HAL_OK)                                                                           Error_func();
  if (HAL_TIM_Base_Start_IT(TIMdef) != HAL_OK)                                                                       Error_func();
}

static void IT_IRQHandler_Config(uint32_t MODE, uint32_t PULL, uint16_t PIN, GPIO_TypeDef *BLOCK, IRQn_Type INTER)
{
  GPIO_InitTypeDef   GPIO_INTER;

  if (BLOCK == GPIOB){
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}else if(BLOCK == GPIOA){
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}

  /* Configure PC.13 pin as input floating */
  GPIO_INTER.Mode = MODE;
  GPIO_INTER.Pull = PULL;
  GPIO_INTER.Pin = PIN;
  HAL_GPIO_Init(BLOCK, &GPIO_INTER);

  /* Enable and set EXTI lines 10 to 15 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(INTER, 2, 0);
  HAL_NVIC_EnableIRQ(INTER);
}

void Error_func(void)
{
  while (1)
  {
		
  }
}