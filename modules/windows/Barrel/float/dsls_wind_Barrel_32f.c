#include "dsls_wind_Barrel.h"
#include <math.h>

void dsls_wind_Barrel_32f(float* window, int len)
{
	float pi = 4.0 * atan(1.0);
	for (int i = 0; i < len; i++)
	{
		window[i] = 0.5 * (1 - cosf(i * 2 * pi / (float)len));
	}
}
