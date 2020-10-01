#pragma once
#include <stm32f1xx_hal.h>
#include <stm32_hal_legacy.h>

class ADC
{
		private:ADC_HandleTypeDef hadc1;
	
	
public:
	ADC();
	void ADC_Begin(uint16_t Gpio_Pin, uint16_t Gpio_Channel, uint16_t Sampling_Time);
	uint16_t ADC_ReadConversion(void);
	~ADC();
};

