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

#include "dsps_fft2r.h"
#include "dsp_common.h"
#include <math.h>

float* dsps_fft_w_table_fc32;
int dsps_fft_w_table_size;
uint8_t dsps_fft2r_initialized = 0;
uint8_t dsps_fft2r_mem_allocated = 0;
esp_err_t dsps_fft2r_init_fc32(float* fft_table_buff, int table_size)
{
    esp_err_t result = ESP_OK;
    if (dsps_fft2r_initialized != 0) {
        return result;
    }
    if (table_size > CONFIG_DSP_MAX_FFT_SIZE)
    {
        return ESP_ERR_DSP_PARAM_OUTOFRANGE;
    }
    if (table_size == 0){
        return result;
    }
    if (fft_table_buff != NULL)
    {
        if (dsps_fft2r_mem_allocated)
        {
            return ESP_ERR_DSP_REINITIALIZED;
        }
        dsps_fft_w_table_fc32 = fft_table_buff;
        dsps_fft_w_table_size = table_size;
    } else 
    {
        if (!dsps_fft2r_mem_allocated) 
        {
            dsps_fft_w_table_fc32 = (float*)malloc(CONFIG_DSP_MAX_FFT_SIZE*sizeof(float));
        }
        dsps_fft_w_table_size = CONFIG_DSP_MAX_FFT_SIZE;
        dsps_fft2r_mem_allocated = 1;
    }

    result = dsps_gen_w_r2_fc32(dsps_fft_w_table_fc32, table_size);
    if (result != ESP_OK) {
        return result;
    }
    result = dsps_bit_rev_fc32_ansi(dsps_fft_w_table_fc32, table_size >> 1);
    if (result != ESP_OK) {
        return result;
    }
    dsps_fft2r_initialized = 1;
    return ESP_OK;
}

void dsps_fft2r_deinit_fc32()
{
    if (dsps_fft2r_mem_allocated)
    {
        free(dsps_fft_w_table_fc32);
    }
    dsps_fft2r_mem_allocated = 0;
    dsps_fft2r_initialized = 0;
}

esp_err_t dsps_fft2r_fc32_ansi(float *data, int N)
{
    if (!dsp_is_power_of_two(N)) {
        return ESP_ERR_DSP_INVALID_LENGTH;
    }
    if (!dsps_fft2r_initialized) {
        return ESP_ERR_DSP_UNINITIALIZED;
    }

    esp_err_t result = ESP_OK;

    float *w = dsps_fft_w_table_fc32;

    int ie, ia, m;
    float re_temp, im_temp;
    float c, s;
    ie = 1;
	for (int N2 = N/2; N2 > 0; N2 >>= 1) {
        ia = 0;
        for (int j = 0; j < ie; j++) {
            c = w[2 * j];
            s = w[2 * j + 1];
            for (int i = 0; i < N2; i++) {
                m = ia + N2;
                re_temp = c * data[2 * m] + s * data[2 * m + 1];
                im_temp = c * data[2 * m + 1] - s * data[2 * m];
                data[2 * m] = data[2 * ia] - re_temp;
                data[2 * m + 1] = data[2 * ia + 1] - im_temp;
                data[2 * ia] = data[2 * ia] + re_temp;
                data[2 * ia + 1] = data[2 * ia + 1] + im_temp;
                ia++;
            }
            ia += N2;
        }
        ie <<= 1;
    }
    return result;
}


static inline unsigned short reverse(unsigned short x, unsigned short N, int order)
{
    unsigned short b = x;
    
    b = (b & 0xff00) >> 8 | (b & 0x00fF) << 8;
    b = (b & 0xf0F0) >> 4 | (b & 0x0f0F) << 4;
    b = (b & 0xCCCC) >> 2 | (b & 0x3333) << 2;
    b = (b & 0xAAAA) >> 1 | (b & 0x5555) << 1;
    return b >> (16 - order);

    // unsigned short mask = 0xffff;
    // x = (((x & 0xaaaa & mask) >> 1) | ((x & 0x5555 & mask) << 1));
    // x = (((x & 0xcccc & mask) >> 2) | ((x & 0x3333 & mask) << 2));
    // x = (((x & 0xf0f0 & mask) >> 4) | ((x & 0x0f0f & mask) << 4));
    // x = (((x & 0xff00 & mask) >> 8) | ((x & 0x00ff & mask) << 8));
    // x = x & 0xffff;
    // unsigned short result = ((x >> 16) | (x << 16));
    // result = result & (N-1);
    // return result;
}

esp_err_t dsps_bit_rev_fc32_ansi(float *data, int N)
{
    if (!dsp_is_power_of_two(N)) {
        return ESP_ERR_DSP_INVALID_LENGTH;
    }
    esp_err_t result = ESP_OK;
    
    //float* temp = (float*)malloc(N*2*sizeof(float));
    // for (int i=0 ; i< N ; i++)
    // {
    //     int dest = i;
    //     data[i*2 + 0] = i;
    //     data[i*2 + 1] = i;
    //     temp[dest*2 + 0] = data[i*2 + 0];
    //     temp[dest*2 + 1] = data[i*2 + 1];
    // }

    // int order = log2f(N);
    // for (int i=0 ; i< N ; i++)
    // {
    //     int dest = reverse(i, N, order);
    //     if (i < dest)
    //     {        
    //         float re = data[dest*2 + 0]; 
    //         float im = data[dest*2 + 1]; 
    //         data[dest*2 + 0] = data[i*2 + 0];
    //         data[dest*2 + 1] = data[i*2 + 1];
    //         data[i*2 + 0] = re;
    //         data[i*2 + 1] = im;
    //     }
    // }
    // return result;
    
    int j, k;
    float r_temp, i_temp;
    j = 0;
    for (int i = 1; i < (N - 1); i++) {
        k = N >> 1;
        while (k <= j) {
            j -= k;
            k >>= 1;
        }
        j += k;
        if (i < j) 
        {
            r_temp = data[j * 2];
            data[j * 2] = data[i * 2];
            data[i * 2] = r_temp;
            i_temp = data[j * 2 + 1];
            data[j * 2 + 1] = data[i * 2 + 1];
            data[i * 2 + 1] = i_temp;
        }
    }
    return result;
}

esp_err_t dsps_gen_w_r2_fc32(float *w, int N)
{
    if (!dsp_is_power_of_two(N)) {
        return ESP_ERR_DSP_INVALID_LENGTH;
    }

    esp_err_t result = ESP_OK;

    int i;
    float e = M_PI * 2.0 / N;

    for (i = 0; i < (N >> 1); i++) {
        w[2 * i] = cosf(i * e);
        w[2 * i + 1] = sinf(i * e);
    }

    return result;
}

esp_err_t dsps_cplx2reC_fc32(float *data, int N)
{
    if (!dsp_is_power_of_two(N)) {
        return ESP_ERR_DSP_INVALID_LENGTH;
    }
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

    for (i = 0; i < (N / 4); i++) {
        rkl = data[i * 2 + 0 + 2];
        ikl = data[i * 2 + 1 + 2];
        rnl = data[n2 - i * 2 - 2];
        inl = data[n2 - i * 2 - 1];

        rkh = data[i * 2 + 0 + 2 + N];
        ikh = data[i * 2 + 1 + 2 + N];
        rnh = data[n2 - i * 2 - 2 - N];
        inh = data[n2 - i * 2 - 1 - N];

        data[i * 2 + 0 + 2] = rkl + rnl;
        data[i * 2 + 1 + 2] = ikl - inl;

        data[n2 - i * 2 - 1 - N] = rkh + rnh;
        data[n2 - i * 2 - 2 - N] = ikh - inh;

        data[i * 2 + 0 + 2 + N] = ikl + inl;
        data[i * 2 + 1 + 2 + N] = rkl - rnl;

        data[n2 - i * 2 - 1] = ikh + inh;
        data[n2 - i * 2 - 2] = rkh - rnh;
    }
    data[N] = data[1];
    data[1] = 0;
    data[N + 1] = 0;

    return result;
}
