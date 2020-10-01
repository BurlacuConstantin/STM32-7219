#pragma once
#include <stm32f1xx_hal.h>
#include <stm32_hal_legacy.h>
#include <arm_math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
class FFT
{
	
public:
	FFT();
	void fft(float vReal[], float vImg[], int samples);
	void mag_to_buf(float MAG[], float REX[], float IMX[], int samples, float min, float max);
	void mag_to_bufD(double MAG[], double REX[], double IMX[], int samples, double min, double max);
	bool Fft_transformRadix2(float real[], float imag[], size_t n);
	void Fft_transformRadix4(double real[], double imag[], int samples, int exponent);
	size_t reverse_bits(size_t x, int n);
	//static void *memdup(const void *src, size_t n);
	inline float mapRangeF(float inMin, float inMax, float outMin, float OutMax, float source);
	inline int mapRangeI(int inMin, int inMax, int outMin, int outMax, int source);
	inline double mapRangeD(double inMin, double inMax, double outMin, double outMax, double source);
	~FFT();
};

