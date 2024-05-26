#include "main.h"

extern "C"
{
   extern ADC_HandleTypeDef hadc1;
   extern TIM_HandleTypeDef htim1;
   extern TIM_HandleTypeDef htim2;
   extern TIM_HandleTypeDef htim3;
   extern TIM_HandleTypeDef htim4;
}

uint8_t newDmaSignal = 0;
uint32_t dmaSignal = 0;
uint16_t motorspeed = 0;
uint16_t motorspeedCurrent = 0;

/////////////////////////////////////////
const uint16_t dmaPulse = 1;
const uint16_t dmaPulseReload = 4034;
volatile uint16_t dmaBuffer[dmaPulse] = {0};
volatile uint16_t resolution = dmaPulse * dmaPulseReload;
void setDutyCycle(uint16_t dc)
{
	if (dc > resolution) dc = resolution;
	dmaBuffer[0] = dc;
}
const uint32_t zcOff[6]={//
		ZC_B_Pin, // B
		ZC_A_Pin, // A
		ZC_C_Pin, // C
		ZC_B_Pin, // B
		ZC_A_Pin, // A
		ZC_C_Pin, // C
};

const uint32_t odLow[6]={
		OD_C_Pin, //C
		OD_C_Pin, //C
		OD_A_Pin, //A
		OD_A_Pin, //A
		OD_B_Pin, //B
		OD_B_Pin, //B
};

const uint32_t odHigh[6]={
		OD_A_Pin, //A
		OD_B_Pin, //B
		OD_B_Pin, //B
		OD_C_Pin, //C
		OD_C_Pin, //C
		OD_A_Pin, //A
};

const uint32_t odOff[6]={
		OD_B_Pin, //B
		OD_A_Pin, //A
		OD_C_Pin, //C
		OD_B_Pin, //B
		OD_A_Pin, //A
		OD_C_Pin, //C
};

const uint32_t ccOff[6]={
		1<<8, // B
		1<<0, // A
		1<<12, //C
		1<<8, // B
		1<<0, // A
		1<<12, //C

};

const uint32_t diOff[6]={
		1<<11, // B
		1<<9, // A
		1<<12, //C
		1<<11, // B
		1<<9, // A
		1<<12, //C

};

const GPIO_TypeDef* zcPortOff[6] ={//
		(GPIO_TypeDef*)ZC_B_GPIO_Port, //B
		(GPIO_TypeDef*)ZC_A_GPIO_Port, //A
		(GPIO_TypeDef*)ZC_C_GPIO_Port, //C
		(GPIO_TypeDef*)ZC_B_GPIO_Port, //B
		(GPIO_TypeDef*)ZC_A_GPIO_Port, //A
		(GPIO_TypeDef*)ZC_C_GPIO_Port, //C
};

volatile uint32_t resetImrFlags;
volatile uint8_t powerStep = 0;
const uint8_t rising[6] = {1,0,1,0,1,0};

void commucate()
{
	 // clear wakeup with interrupt reg.
	EXTI -> IMR = resetImrFlags;
	// disable zc interrupts .
	EXTI -> FTSR &= ~(ZC_A_Pin | ZC_B_Pin | ZC_C_Pin);
	EXTI -> RTSR &= ~(ZC_A_Pin | ZC_B_Pin | ZC_C_Pin);
	// clear any zc pending interrupts.
	EXTI -> PR &= ~(ZC_A_Pin | ZC_B_Pin | ZC_C_Pin);
	// go to next power step.
	powerStep++;
	powerStep %= 6;

	if(rising[powerStep])
	{
	//if zc rising
	// enable zc rising interrupt
		EXTI -> RTSR |= zcOff[powerStep];
	// enable odLow.
		GPIOA->BSRR = odLow[powerStep];

	}else
	{
	//else
	// enable zc falling int.
		EXTI -> FTSR |= zcOff[powerStep];
	// enable odHigh.
		GPIOA->BSRR = odHigh[powerStep];
	}
	// disable odOff
	GPIOA -> BRR = odOff[powerStep];

	if(!rising[powerStep])
	{
	// if falling
	//  disable cc on timer 3 and timer 2
		TIM3 -> CCER &= ~(ccOff[powerStep]);
		TIM2 -> CCER &= ~(ccOff[powerStep]);
	//  disable dma on timer 3 and timer 2
		TIM3 -> DIER &= ~(diOff[powerStep]);
		TIM2 -> DIER &= ~(diOff[powerStep]);
	}else
	{
	//else
	// enable cc on timer 3 and timer 2
		TIM3 -> CCER |= (ccOff[powerStep]);
		TIM2 -> CCER |= (ccOff[powerStep]);
	// enable dma on timer 3 and timer 2
		TIM3 -> DIER |= (diOff[powerStep]);
		TIM2 -> DIER |= (diOff[powerStep]);
	}

	//add delay of 2 us.
	TIM1 -> CNT = 0;
	while (TIM1->CNT < 128); //2us
	//enable wakeup with int
	EXTI -> IMR = resetImrFlags | zcOff[powerStep];
	//reset timer 4 for bump motor
	TIM4 -> CNT = 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	TIM1->CNT = 0;
	while(TIM1 -> CNT < (4088 - motorspeedCurrent))
		if((zcPortOff[powerStep]->IDR & zcOff[powerStep]) == 0 || (zcPortOff[powerStep]->IDR & zcOff[powerStep]) == 1 )
			return;
	commucate();
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim4 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		if(motorspeedCurrent > 0)
			commucate();
}
////////////////////////////////////////


void processDmaSignal()
{
	newDmaSignal = 0;
	motorspeed = dmaSignal;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    	newDmaSignal = 1;
}

void setup()
{
	EXTI -> FTSR = 0;
	EXTI -> RTSR = 0;
	HAL_Delay(1000);
	resetImrFlags = EXTI -> IMR;
	resetImrFlags &= ~(ZC_A_Pin | ZC_B_Pin | ZC_C_Pin);
	EXTI->IMR = resetImrFlags;
    HAL_ADC_Start_DMA(&hadc1, &dmaSignal, 1);

    if(HAL_TIM_Base_Start(&htim1) != HAL_OK)
    	Error_Handler();
    if(HAL_TIM_Base_Start(&htim3) != HAL_OK)
    	Error_Handler();
    if(HAL_TIM_Base_Start(&htim2) != HAL_OK)
    	Error_Handler();

    TIM2 -> CCR1 = 0;
    TIM3 -> CCR3 = 0;
    TIM3 -> CCR4 = 0;
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);


    HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_4);

    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)dmaBuffer, dmaPulse);
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3, (uint32_t*)dmaBuffer, dmaPulse);
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4, (uint32_t*)dmaBuffer, dmaPulse);

    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 150);
    HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1);
}

void maincpp()
{
	setup();
    while(1){
    	if (newDmaSignal)
    	{
    		processDmaSignal();
    		if (motorspeed != motorspeedCurrent)
    		{
    			if((motorspeed - motorspeedCurrent) > 10)
    			{
    				motorspeedCurrent += 10;
    			}
    			else
    			{
    				motorspeedCurrent = motorspeed;
    			}
    			setDutyCycle(motorspeedCurrent);
    		}
    		HAL_ADC_Start_DMA(&hadc1, &dmaSignal, 1);
    	}
    	HAL_Delay(1);
    }
}
