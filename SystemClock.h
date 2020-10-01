#pragma once
#include <stm32f1xx_hal.h>
#include <stm32_hal_legacy.h>
	
//#define SYS_CLCK_72MHZ
//#define SYS_CLCK_40MHZ
//#define SYS_CLCK_8MHZ

class SystemClock
{
	
	public:
	SystemClock();
	void SetupClk(void);
	void Error_Handler_SystemClck(void);
	~SystemClock();
};

