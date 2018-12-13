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

#ifndef _dsls_fft2r_H_
#define _dsls_fft2r_H_
#include "dsl_err.h"
#include "sdkconfig.h"

#ifndef CONFIG_DSL_MAX_FFT_SIZE
#define CONFIG_DSL_MAX_FFT_SIZE 4096
#endif // CONFIG_DSL_MAX_FFT_SIZE

#ifdef __cplusplus
extern "C"
{
#endif

extern float dsls_fft_w_table_32fc[CONFIG_DSL_MAX_FFT_SIZE];

/**
 * @function dsls_fft2r_init_32fc
 * Initialization of Complex FFT. This function initialize coefficients table.
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_fft2r_init_32fc();

/**
 * @function dsls_fft2r_32fc_ansi
 * Complex FFT of radix 2
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param input: input complex array. An elements located: Re[0], Im[0], ... Re[N-1], Im[N-1]
 *               result of FFT will be stored to this array.
 * @param N: size of the complex array
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_fft2r_32fc_ansi(float *input, int N);
/**
 * @function dsls_fft2r_32fc_ae32
 * Complex FFT of radix 2
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param input: input complex array. An elements located: Re[0], Im[0], ... Re[N-1], Im[N-1]
 *               result of FFT will be stored to this array.
 * @param N: size of the complex array
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_fft2r_32fc_ae32(float *input, int N, float* w);

/**
 * @function dsls_bit_rev_32fc_ansi
 * Bit reverse operation for the complex input array
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param input: input complex array. An elements located: Re[0], Im[0], ... Re[N-1], Im[N-1]
 *               result of FFT will be stored to this array.
 * @param N: size of the complex array
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_bit_rev_32fc_ansi(float *input, int N);

/**
 * @function dsls_gen_w_r2_32fc
 * Generate coefficients table for the FFT radix 2. This function called inside init.
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param w: memory location to store coefficients.
 *           By default coefficients will be stored to the dsls_fft_w_table_32fc.
 *           Maximum size of the FFT must be setup in menuconfig
 * @param N: maximum size of the FFT that will be used
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_gen_w_r2_32fc(float *w, int N);

/**
 * @function dsls_cplx2reC_32fc
 * Convert complex array to two real arrays in case if input was two real arrays.
 * This function have to be used if FFT used to process real data.
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param input: Input complex array as result of FFT2R.
 *               input has size of 2*N, because contains real and imaginary part.
 *               result will be stored to the same array.
 *               Input1: input[0..N-1], Input2: input[N..2*N-1]
 * @param N: input complex array size
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_cplx2reC_32fc(float *input, int N);

#ifdef __cplusplus
}
#endif

#endif // _dsls_fft2r_cf_H_