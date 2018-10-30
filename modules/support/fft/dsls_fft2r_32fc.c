// Copyright 2018-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License. 

#include "dsls_fft2r.h"
#include "dsl_common.h"
#include <math.h>

float dsls_fft_w_table_32fc[CONFIG_DSL_MAX_FFT_SIZE];

esp_err_t dsls_fft2r_32fc(float* input, float* w, int N)
{
    if (!is_power_of_two(N)) return ESP_ERR_DSL_INVALID_LENGTH;
   
    esp_err_t result = ESP_OK;
	
    int ie, ia, m;
	float re_temp, im_temp;
    float c, s;
	int N2 = N;
	ie = 1;
	for (int k = N; k > 1; k >>= 1)
	{
		N2 >>= 1;
		ia = 0;
		for (int j = 0; j < ie; j++)
		{
			c = w[2 * j];
			s = w[2 * j + 1];
			for (int i = 0; i < N2; i++)
			{
				m = ia + N2;
				re_temp = c * input[2 * m] + s * input[2 * m + 1];
				im_temp = c * input[2 * m + 1] - s * input[2 * m];
				input[2 * m] = input[2 * ia] - re_temp;
				input[2 * m + 1] = input[2 * ia + 1] - im_temp;
				input[2 * ia] = input[2 * ia] + re_temp;
				input[2 * ia + 1] = input[2 * ia + 1] + im_temp;
				ia++;
			}
			ia += N2;
		}
		ie <<= 1;
	}    
    return result;
}

esp_err_t dsls_bit_rev_32fc(float *input, int N)
{
    if (!is_power_of_two(N)) return ESP_ERR_DSL_INVALID_LENGTH;
    esp_err_t result = ESP_OK;

	int i, j, k;
	float r_temp, i_temp;

	j = 0;
	for (i = 1; i < (N - 1); i++)
	{
		k = N >> 1;
		while (k <= j)
		{
			j -= k;
			k >>= 1;
		}
		j += k;
		if (i < j)
		{
			r_temp = input[j * 2];
			input[j * 2] = input[i * 2];
			input[i * 2] = r_temp;
			i_temp = input[j * 2 + 1];
			input[j * 2 + 1] = input[i * 2 + 1];
			input[i * 2 + 1] = i_temp;
		}
	}
    return result;
}

esp_err_t dsls_gen_w_r2_32fc(float *w, int N)
{
    if (!is_power_of_two(N)) return ESP_ERR_DSL_INVALID_LENGTH;

    esp_err_t result = ESP_OK;

	int i;
	float e = M_PI * 2.0 / N;

	for (i = 0; i < (N >> 1); i++)
	{
		w[2 * i] = cosf(i * e);
		w[2 * i + 1] = sinf(i * e);
	}

    return result;
}

esp_err_t dsls_cplx2reC_32fc(float *input, int N)
{
    if (!is_power_of_two(N)) return ESP_ERR_DSL_INVALID_LENGTH;
    esp_err_t result = ESP_OK;

	int i;
	int n2 = N << 1;

	float rkl = 0;
	float rkh = 0;
	float rnl = 0;
	float rnh = 0;
	float ikl = 0;
	float ikh = 0;
	float inl = 0;
	float inh = 0;

	for (i = 0; i < (N / 4); i++)
	{
		rkl = input[i * 2 + 0 + 2];
		ikl = input[i * 2 + 1 + 2];
		rnl = input[n2 - i * 2 - 2];
		inl = input[n2 - i * 2 - 1];

		rkh = input[i * 2 + 0 + 2 + N];
		ikh = input[i * 2 + 1 + 2 + N];
		rnh = input[n2 - i * 2 - 2 - N];
		inh = input[n2 - i * 2 - 1 - N];

		input[i * 2 + 0 + 2] = rkl + rnl;
		input[i * 2 + 1 + 2] = ikl - inl;

		input[n2 - i * 2 - 1 - N] = rkh + rnh;
		input[n2 - i * 2 - 2 - N] = ikh - inh;

		input[i * 2 + 0 + 2 + N] = ikl + inl;
		input[i * 2 + 1 + 2 + N] = rkl - rnl;

		input[n2 - i * 2 - 1] = ikh + inh;
		input[n2 - i * 2 - 2] = rkh - rnh;
	}
	input[N] = input[1];
	input[1] = 0;
	input[N + 1] = 0;

    return result;
}

// =============== How to use: ==========================================
// float FFT::CFFT_Coeffs_Table[1024];
// bool FFT::initialized = false;

// int FFT::InitFFT()
// {
// 	if (false == this->initialized) // The buffers will be initialized only once
// 	{
// 		gen_w_r2(FFT::CFFT_Coeffs_Table, 512);	 //Generate coefficient table
// 		bit_rev(FFT::CFFT_Coeffs_Table, 512 >> 1); //Bitreverse coefficient table
// 		this->initialized = true;
// 	}
// 	return 0;
// }

// int FFT::fft(float *in_buff, int size, float *out_buff)
// {
// 	DSPF_sp_cfftr2_ditSLOW(in_buff, FFT::CFFT_Coeffs_Table, size);
// 	bit_rev(in_buff, size);
// 	return 0;
// }
