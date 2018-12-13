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
uint8_t dsls_fft2r_initialized = 0;

esp_err_t dsls_fft2r_init_32fc()
{
    esp_err_t result = ESP_OK;
    if (dsls_fft2r_initialized != 0) {
        return result;
    }
    result = dsls_gen_w_r2_32fc(dsls_fft_w_table_32fc, CONFIG_DSL_MAX_FFT_SIZE);
    if (result != ESP_OK) {
        return result;
    }
    result = dsls_bit_rev_32fc_ansi(dsls_fft_w_table_32fc, CONFIG_DSL_MAX_FFT_SIZE >> 1);
    if (result != ESP_OK) {
        return result;
    }
    dsls_fft2r_initialized = 1;
    return ESP_OK;
}

esp_err_t dsls_fft2r_32fc_ansi(float *input, int N)
{
    if (!is_power_of_two(N)) {
        return ESP_ERR_DSL_INVALID_LENGTH;
    }
    if (!dsls_fft2r_initialized) {
        return ESP_ERR_DSL_UNINITIALIZED;
    }

    esp_err_t result = ESP_OK;

    float *w = dsls_fft_w_table_32fc;

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

esp_err_t dsls_bit_rev_32fc_ansi(float *input, int N)
{
    if (!is_power_of_two(N)) {
        return ESP_ERR_DSL_INVALID_LENGTH;
    }
    esp_err_t result = ESP_OK;
    
    //float* temp = (float*)malloc(N*2*sizeof(float));
    // for (int i=0 ; i< N ; i++)
    // {
    //     int dest = i;
    //     input[i*2 + 0] = i;
    //     input[i*2 + 1] = i;
    //     temp[dest*2 + 0] = input[i*2 + 0];
    //     temp[dest*2 + 1] = input[i*2 + 1];
    // }

    // int order = log2f(N);
    // for (int i=0 ; i< N ; i++)
    // {
    //     int dest = reverse(i, N, order);
    //     if (i < dest)
    //     {        
    //         float re = input[dest*2 + 0]; 
    //         float im = input[dest*2 + 1]; 
    //         input[dest*2 + 0] = input[i*2 + 0];
    //         input[dest*2 + 1] = input[i*2 + 1];
    //         input[i*2 + 0] = re;
    //         input[i*2 + 1] = im;
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
    if (!is_power_of_two(N)) {
        return ESP_ERR_DSL_INVALID_LENGTH;
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

esp_err_t dsls_cplx2reC_32fc(float *input, int N)
{
    if (!is_power_of_two(N)) {
        return ESP_ERR_DSL_INVALID_LENGTH;
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
