#pragma once
#include <stm32f1xx_hal.h>
#include <stm32_hal_legacy.h>
#include <SPI.h>

#define MAX7219_MODE_NOOP         0x00
#define MAX7219_MODE_DECODE       0x09
#define MAX7219_MODE_INTENSITY    0x0A
#define MAX7219_MODE_SCAN_LIMIT   0x0B
#define MAX7219_MODE_POWER        0x0C
#define MAX7219_MODE_TEST         0x0F
#define MAX7219_REG_DIGIT_0		  0x01
#define MAX7219_REG_DIGIT_1		  0x02
#define MAX7219_REG_DIGIT_2		  0x03
#define MAX7219_REG_DIGIT_3		  0x04
#define MAX7219_REG_DIGIT_4	      0x05
#define MAX7219_REG_DIGIT_5		  0x06
#define MAX7219_REG_DIGIT_6       0x07
#define MAX7219_REG_DIGIT_7		  0x08

class MAX7219
{
	private : uint8_t MAX_DEVICES;
	private: SPI mySPI = SPI(SPI_DATASIZE_8BIT, SPI_BAUDRATEPRESCALER_4, 2); // 9 MHZ , 9MBits / s 
	private : uint8_t status[64];
	
public:

	MAX7219(uint8_t maxDevices);
	
	void Begin(void);
	void ClearMatrix(void);
	void SetIntensityLevel(uint8_t value);
	uint8_t getMaxColumns(void);
	uint8_t getMaxDevices(void);
	uint8_t getMaxControlBytes(void);
	void SendToAll(uint8_t address, uint8_t data);
	void SendToOne(uint8_t address, uint8_t address_data, uint8_t data);
	void WriteBytes(uint8_t address, uint8_t data);
	void WriteByte(uint8_t data);
	void BitWrite(uint8_t value, uint8_t num_bit, uint8_t val_bit);
	void SetLed(uint8_t address, uint8_t row, uint8_t column, bool state);
	
	void setRow(uint8_t address, uint8_t row, uint8_t value);
	~MAX7219();
};

