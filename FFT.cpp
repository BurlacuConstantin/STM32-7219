#include "FFT.h"



FFT::FFT()
{
}

void FFT::fft(float vReal[], float vImg[], int samples)
{
	uint8_t nm1 = samples - 1;
	uint8_t nd2 = samples / 2;
	uint8_t m = log10(samples) / log10(2);
	uint8_t j = nd2;
	uint8_t i, k, l;
	uint8_t le, le2;
	float ur, ui, sr, si;
	uint8_t jm1, ip;
	float tr, ti;
	
	// Set imaginary part input signal to 0
//	for( i = 0 ; i <= nm1 ; i++)
//	{
//		vImg[i] = 0;
//	}
	
	// Bit reversal sorting
	for(i = 1 ; i <= samples - 2 ; i++)
	{
		if (i >= j) goto a;
		tr = vReal[j];
		ti = vImg[j];
		vReal[j] = vReal[i];
		vImg[j] = vImg[i];
		vReal[i] = tr;
		vImg[i] = ti;
a:
		k = nd2;
b:
		if (k > j) goto c;
		j -= k;
		k /= 2;
		goto b;
c:
		j += k;
	}

	// Loop for each FFT stage
	for(l = 1 ; l <= m ; l++)
	{
		le = pow(2, l);
		le2 = le / 2;
		ur = 1;
		ui = 0;
		// Calculate sine and cosine values
		sr = cos(PI / le2);
		si = -sin(PI / le2);
		// Loop for each sub FFT
		for(j = 1 ; j <= le2 ; j++)
		{
			jm1 = j - 1; 
			// Loop for each butterfly
			for(i = jm1 ; i <= nm1 ; i += le)
			{
				ip = i + le2;
				tr = vReal[ip]*ur - vImg[ip]*ui;
				ti = vReal[ip]*ui + vImg[ip]*ur;
				vReal[ip] = vReal[i] - tr;
				vImg[ip] = vImg[i] - ti;
				vReal[i] = vReal[i] + tr;
				vImg[i] = vImg[i] + ti;
			}
			tr = ur;
			ur = tr*sr - ui*si;
			ui = tr*si + ui*sr;
		}
	}
}

bool FFT::Fft_transformRadix2(float real[], float imag[], size_t n)
{
	// Length variables
	bool status = false;
	int levels = 0;   // Compute levels = floor(log2(n))
	for(size_t temp = n ; temp > 1U ; temp >>= 1)
		levels++;
	if ((size_t)1U << levels != n)
		return false;  // n is not a power of 2

	// Trignometric tables
	if(SIZE_MAX / sizeof(float) < n / 2)
		return false;
	size_t size = (n / 2) * sizeof(float);
	float *cos_table = (float*)malloc(size);
	float *sin_table = (float*)malloc(size);
	if (cos_table == NULL || sin_table == NULL)
		goto cleanup;
	for (size_t i = 0; i < n / 2; i++) {
		cos_table[i] = cos(2 * M_PI * i / n);
		sin_table[i] = sin(2 * M_PI * i / n);
	}

	// Bit-reversed addressing permutation
	for(size_t i = 0 ; i < n ; i++) {
		size_t j = reverse_bits(i, levels);
		if (j > i) {
			float temp = real[i];
			real[i] = real[j];
			real[j] = temp;
			temp = imag[i];
			imag[i] = imag[j];
			imag[j] = temp;
		}
	}

	// Cooley-Tukey decimation-in-time radix-2 FFT
	for(size_t size = 2 ; size <= n ; size *= 2) {
		size_t halfsize = size / 2;
		size_t tablestep = n / size;
		for (size_t i = 0; i < n; i += size) {
			for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep) {
				size_t l = j + halfsize;
				float tpre =  real[l] * cos_table[k] + imag[l] * sin_table[k];
				float tpim = -real[l] * sin_table[k] + imag[l] * cos_table[k];
				real[l] = real[j] - tpre;
				imag[l] = imag[j] - tpim;
				real[j] += tpre;
				imag[j] += tpim;
			}
		}
		if (size == n)  // Prevent overflow in 'size *= 2'
			break;
	}
	status = true;

cleanup:
	free(cos_table);
	free(sin_table);
	return status;
}

void FFT::Fft_transformRadix4(double real[], double imag[], int samples, int exponent)
{
	// N = 4 ^ M
    int N1, N2;
	int I1, I2, I3;
	double CO1, CO2, CO3, SI1, SI2, SI3;
	double A, B, C, E;
	double R1, R2, R3, R4;
	double S1, S2, S3, S4;
	// N = 1 << (M+M);
	N2 = samples;
	I2 = 0; I3 = 0;
	for (int K = 0; K < exponent; ++K) {
		N1 = N2;
		N2 = N2 / 4;
		E = (2 * PI) / (double)N1;
		A = 0.0;
		for (int J = 0; J < N2; ++J) {
			A = J*E;
			B = A + A;
			C = A + B;
			//Should be pre-calculated for optimization
			CO1 = cos(A);
			CO2 = cos(B);
			CO3 = cos(C);
			SI1 = sin(A);
			SI2 = sin(B);
			SI3 = sin(C);
			for (int I = J; I < samples; I += N1) {
				I1 = I + N2;
				I2 = I1 + N2;
				I3 = I2 + N2;
				R1 = real[I] + real[I2];
				R3 = real[I] - real[I2];
				S1 = imag[I] + imag[I2];
				S3 = imag[I] - imag[I2];
				R2 = real[I1] + real[I3];
				R4 = real[I1] - real[I3];
				S2 = imag[I1] + imag[I3];
				S4 = imag[I1] - imag[I3];
				real[I] = R1 + R2;
				R2 = R1 - R2;
				R1 = R3 - S4;
				R3 = R3 + S4;
				imag[I] = S1 + S2;
				S2 = S1 - S2;
				S1 = S3 + R4;
				S3 = S3 - R4;
				real[I1] = CO1*R3 + SI1*S3;
				imag[I1] = CO1*S3 - SI1*R3;
				real[I2] = CO2*R2 + SI2*S2;
				imag[I2] = CO2*S2 - SI2*R2;
				real[I3] = CO3*R1 + SI3*S1;
				imag[I3] = CO3*S1 - SI3*R1;
			}
		}
	}

	// Radix-4 bit-reverse
	double T;
	int J = 0;
	N2 = samples >> 2;
	for (int I = 0; I < samples - 1; I++) {
		if (I < J) {
			T = real[I];
			real[I] = real[J];
			real[J] = T;
			T = imag[I];
			imag[I] = imag[J];
			imag[J] = T;
		}
		N1 = N2;
		while (J >= 3*N1) {
			J -= 3*N1;
			N1 >>= 2;
		}
		J += N1;
	}
}

void FFT::mag_to_buf(float MAG[], float REX[], float IMX[], int samples, float min, float max)
{
	//uint8_t i, j;
	
	for(int i = 0 ; i <= samples / 2 ; i++)
	{
		MAG[i] = sqrt(REX[i] * REX[i] + IMX[i] * IMX[i]);
		
		//MAG[i] /= 64;
		
		//if (MAG[i] > 8)
			//MAG[i] = 8;
		MAG[i] = mapRangeF(min, max, 1, 8, MAG[i]);
	}
	/*
	// Loop for each magnitude
	for(i = 0 ; i <= samples / 2 ; i++)
	{
		// Calculate magnitude
		MAG[i] = sqrt(REX[i]*REX[i] + IMX[i]*IMX[i]);
		
		// Scaling magnitude
		MAG[i] /= 64;
		if (MAG[i] > 8)
		{
			MAG[i] = 8;
		}
	}
	*/
}

void FFT::mag_to_bufD(double MAG[], double REX[], double IMX[], int samples, double min, double max)
{
	for (int i = 0; i <= samples / 4; i++)
	{
		MAG[i] = sqrt(REX[i] * REX[i] + IMX[i] * IMX[i]);
		
		//MAG[i] /= 64;
		
		//if (MAG[i] > 8)
			//MAG[i] = 8;
		MAG[i] = mapRangeD(min, max, 1, 8, MAG[i]);
	}
}

size_t FFT::reverse_bits(size_t x, int n)
{
	size_t result = 0;
	for (int i = 0; i < n; i++, x >>= 1)
		result = (result << 1) | (x & 1U);
	return result;
}

inline float FFT::mapRangeF(float inMin, float inMax, float outMin, float OutMax, float source)
{
	return outMin + (source - inMin) * (OutMax - outMin) / (inMax - inMin);
}

inline int FFT::mapRangeI(int inMin, int inMax, int outMin, int outMax, int source)
{
	return outMin + (source - inMin) * (outMax - outMin) / (inMax - inMin);
}

inline double FFT::mapRangeD(double inMin, double inMax, double outMin, double outMax, double source)
{
	return outMin + (source - inMin) * (outMax - outMin) / (inMax - inMin);
}

FFT::~FFT()
{
}
