#include "ADC.h"



ADC::ADC()
{
	
}

void ADC::ADC_Begin(uint16_t Gpio_Pin, uint16_t Gpio_Channel, uint16_t Sampling_Time)
{
	ADC_ChannelConfTypeDef sConfig = { 0 };
	GPIO_InitTypeDef gpio = { 0 };
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	gpio.Pin = Gpio_Pin;
	gpio.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &gpio);
	
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	
	HAL_ADC_Init(&this->hadc1);
	
	sConfig.Channel = Gpio_Channel;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = Sampling_Time;
	
	HAL_ADC_ConfigChannel(&this->hadc1, &sConfig); 
}

/* This function return unsigned 16 bit ADC conversion value of 12 bits
 * There is no need to reset the flag becouse it is done by it self in hardware
 * */
uint16_t ADC::ADC_ReadConversion(void)
{
		HAL_ADC_Start(&this->hadc1);
		while (__HAL_ADC_GET_FLAG(&this->hadc1, ADC_FLAG_EOC) == RESET) ;
		return HAL_ADC_GetValue(&this->hadc1);
		//__HAL_ADC_CLEAR_FLAG(&this->hadc1, ADC_FLAG_EOC);
		//HAL_ADC_Stop(&this->hadc1);
}

ADC::~ADC()
{
}
