#include "SPI.h"



SPI::SPI(uint8_t DataSize, uint8_t BaudratePrescaler, uint8_t SpeedFreq)
{

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	/*Configure GPIO pin : PA4 */
	GPIO_InitStruct.Pin = SLAVE_SELECT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	
	switch (SpeedFreq)
	{
		case 0:
		{
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 10MHZ
			break;
		}
		case 1:
		{
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM; // 25MHZ
			break;
		}
		case 2:
		{
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 50Mhz
			break;
		}
		
	}

	HAL_GPIO_Init(SLAVE_SELECT_PORT, &GPIO_InitStruct);
	
		/* MOSI & SCLK for SPI1*/
	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;		// 5 clck	7 data
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  // AF_PP
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	
	switch (SpeedFreq)
	{
	case 0:
		{
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  // 10MHZ
			break;
		}
	case 1:
		{
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;  // 25MHZ
			break;
		}
	case 2:
		{
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 50Mhz
			break;
		}
		
	}
	
	HAL_GPIO_Init(GPIOA, & GPIO_InitStruct);
	
	__HAL_AFIO_REMAP_SPI1_DISABLE();
	
	__SPI1_CLK_ENABLE();
	
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_1LINE; // one line default
	hspi1.Init.DataSize = DataSize; // 8 bit default
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = BaudratePrescaler;   // 72MHZ / 64 = 1,125 MHZ for SPI communication - 562.5 Kbits/s
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_HandlerSPI();
	}
	
	__SPI_CS_DISABLE__();
}



void SPI::SendData(volatile uint8_t address, volatile uint8_t data)
{
	__SPI_CS_ENABLE__();
	HAL_SPI_Transmit(&hspi1, (uint8_t *) &address, sizeof(address), 100);
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
	HAL_SPI_Transmit(&hspi1, (uint8_t *) &data, sizeof(data), 100);
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) ;
	__SPI_CS_DISABLE__();
}

void SPI::SendData2(volatile uint8_t data)
{
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&data, sizeof(data), 100);
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
}


void SPI::WriteOnPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t value)
{
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	
	GPIOx->ODR = value;
}

/*
void SPI::shiftOut(uint8_t bitOrder, uint8_t val)
{
	uint8_t i;
	
	for (i = 0; i < 8; i++) 
	{
		if (bitOrder == LSBFIRST)
		{
			WriteOnPin(GPIOA, GPIO_PIN_7, !! (val & (1 << i)));
		}
		else
		{
			WriteOnPin(GPIOA, GPIO_PIN_7, !! (val & (1 << (7 - i))));
		}
		
		__SPI_CS_ENABLE__();  
		__SPI_CS_DISABLE__();   
	}
}
*/
	

void SPI::Error_HandlerSPI(void)
{
	
}

SPI::~SPI()
{
}


