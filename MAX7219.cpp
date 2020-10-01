#include "MAX7219.h"


MAX7219::MAX7219(uint8_t maxDevices)
{
	if (!maxDevices || maxDevices < 0)
		return;
	
	this->MAX_DEVICES = maxDevices;
}

uint8_t MAX7219::getMaxDevices(void)
{
	return this->MAX_DEVICES;
}

void MAX7219::Begin(void)
{	
	SendToAll(0x00, 0x00); // empty the sift registers
	SendToAll(MAX7219_MODE_DECODE, 0x00);
	SendToAll(MAX7219_MODE_TEST, 0x00);
	SendToAll(MAX7219_MODE_SCAN_LIMIT, 0x07); // 0X07
	SendToAll(MAX7219_MODE_INTENSITY, 0x01);
	SendToAll(MAX7219_MODE_POWER, 0x01);
	
	ClearMatrix();
}

void MAX7219::ClearMatrix(void)
{
	SendToAll(MAX7219_REG_DIGIT_0, 0x00);
	SendToAll(MAX7219_REG_DIGIT_1, 0x00);
	SendToAll(MAX7219_REG_DIGIT_2, 0x00);
	SendToAll(MAX7219_REG_DIGIT_3, 0x00);
	SendToAll(MAX7219_REG_DIGIT_4, 0x00);
	SendToAll(MAX7219_REG_DIGIT_5, 0x00);
	SendToAll(MAX7219_REG_DIGIT_6, 0x00);
	SendToAll(MAX7219_REG_DIGIT_7, 0x00);
}

void MAX7219::SetIntensityLevel(uint8_t value)
{
	if (!value || value > 15 || value < 1) 
		return;
	__SPI_CS_ENABLE__();
	for (uint8_t i = 0; i < this->MAX_DEVICES; i++)
	{
		mySPI.SendData2(MAX7219_MODE_INTENSITY);
		mySPI.SendData2(value);
	}
	__SPI_CS_DISABLE__();
}

uint8_t MAX7219::getMaxColumns(void)
{
	return this->MAX_DEVICES * 8;
}

uint8_t MAX7219::getMaxControlBytes(void)
{
	return this->MAX_DEVICES * 2;
}

void MAX7219::BitWrite(uint8_t value, uint8_t num_bit, uint8_t val_bit)
{
	if (!value || num_bit > 7 || val_bit > 1)
		return;
	
	value = value | val_bit << num_bit;
}

void MAX7219::WriteBytes(uint8_t address, uint8_t data)
{
	if (!address || !data)
		return;
	
	mySPI.SendData(address, data);
}

void MAX7219::WriteByte(uint8_t data)
{
	if(!data)
		return;
	
	mySPI.SendData2(data);
}

void MAX7219::SendToAll(uint8_t address, uint8_t data)
{
	__SPI_CS_ENABLE__();
	
	for (uint8_t i = 0; i < this->MAX_DEVICES; i++)
	{
		mySPI.SendData2(address);
		mySPI.SendData2(data);

	}
	__SPI_CS_DISABLE__();
}

void MAX7219::SendToOne(uint8_t address, uint8_t address_data, uint8_t data)
{
	if (address < 0 || address_data < 0 || data < 0)
		return;
	//if (!address || !address_data || address_data < 0 || data < 0 || !data)
		//return;
	
	uint8_t array_to_send[getMaxControlBytes()];
	
	for (uint8_t i = 0; i < getMaxControlBytes(); i++)
	{
		if (i % 2 == 0)
		{
			array_to_send[i] = address_data;
		}
		else
		{
			array_to_send[i] = 0x00;
		}
	}
	
	for (uint8_t i = 0; i < getMaxControlBytes(); i++)
	{
		if (i == (address * 2)) // multiplexing like getmaxcontrolbyte 4 devices * 2 bytes of control
			{
				array_to_send[i] = address_data;
				array_to_send[i + 1] = data;
			}
	}
	
	__SPI_CS_ENABLE__();
	for (uint8_t i = 0; i < getMaxControlBytes(); i++)
	{
		mySPI.SendData2(array_to_send[i]);
	}
	__SPI_CS_DISABLE__();
}



void MAX7219::SetLed(uint8_t address, uint8_t row, uint8_t column, bool state)
{
	if (address < 0 || address > getMaxDevices())
		return;
	
	if (row < 1 || row > 8 || column < 0 || column > 7)
		return;
	
	uint8_t dataStore = 0;
	uint8_t array_to_send[getMaxControlBytes()];
	

	
	for (uint8_t i = 0; i < getMaxControlBytes(); i++)
	{
		if (i % 2 == 0)
		{
			array_to_send[i] = row;
		}
		else
		{
			array_to_send[i] = 0x00;
		}
	}
	
	uint8_t val = 0B10000000 >> column;
	
	if (state == true)
	{
		dataStore |= val;
	}
	else
	{
		val = ~val;
		dataStore &= val;
	}
	
	for (uint8_t i = 0; i < getMaxControlBytes(); i++)
	{
		if (i == (address * 2)) // multiplexing like getmaxcontrolbyte 4 devices * 2 bytes of control
			{
				array_to_send[i] = row;
				array_to_send[i + 1] = dataStore;
			}
	}
	
	__SPI_CS_ENABLE__();
	for (uint8_t i = 0; i < getMaxControlBytes(); i++)
	{
		mySPI.SendData2(array_to_send[i]);
	}
	__SPI_CS_DISABLE__();
}

void MAX7219::setRow(uint8_t address, uint8_t row, uint8_t value)
{
	if (address < 0 || address >= getMaxDevices())
	{
		
	}
}

MAX7219::~MAX7219()
{
}