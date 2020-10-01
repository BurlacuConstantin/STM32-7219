#pragma once
#include <stm32f1xx_hal.h>
#include <stm32_hal_legacy.h>


/* This SPI1 class is only in MOSI mode, only transmitting */
#define LSBFIRST				0
#define MSBFIRST				1

#define SLAVE_SELECT_PORT		GPIOA
#define SLAVE_SELECT_PIN		GPIO_PIN_4

#define __SPI_CS_ENABLE__()  HAL_GPIO_WritePin(SLAVE_SELECT_PORT, SLAVE_SELECT_PIN, GPIO_PIN_RESET)	// SLAVE SELECT - PA4
#define __SPI_CS_DISABLE__() HAL_GPIO_WritePin(SLAVE_SELECT_PORT, SLAVE_SELECT_PIN, GPIO_PIN_SET)	// SOFTWARE SIGNAL

class SPI
{
	private:volatile uint8_t spiBuff[2] = { 0, 0 }; // 2 bytes buffer for SPI SendData function
	private:SPI_HandleTypeDef hspi1;
	
	
public:
	SPI(uint8_t DataSize, uint8_t BaudratePrescaler, uint8_t SpeedFreq = 1);
	
	void SendData(volatile uint8_t address, volatile uint8_t data);
	void WriteOnPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t value);
	//void shiftOut(uint8_t bitOrder, uint8_t val);
	void SendData2(volatile uint8_t data);
	void Error_HandlerSPI(void);
	~SPI();
};

