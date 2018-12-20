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

#ifndef CONFIG_DSP_MAX_FFT_SIZE
#define CONFIG_DSP_MAX_FFT_SIZE 4096
#endif // CONFIG_DSP_MAX_FFT_SIZE

#ifdef __cplusplus
extern "C"
{
#endif

extern float* dsls_fft_w_table_32fc;
extern int dsls_fft_w_table_size;
extern uint8_t dsls_fft2r_initialized;

/**
 * Initialization of Complex FFT. This function initialize coefficients table.
 * The implementation use ANSI C and could be compiled and run on any platform
 * 
 * @param fft_table_buff: pointer to floating point buffer where sin/cos table will be stored
 *                          if this parameter set to NULL, and table_size value is more then 0, then 
 *                          dsls_fft2r_init_32fc will allocate buffer internally
 * @param table_size: size of the buffer in float words  
 *                      if fft_table_buff is NULL and table_size is not 0, buffer will be allocated internally.
 *                      If table_size is 0, buffer will not be allocated. 
 * 
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_DSL_PARAM_OUTOFRANGE if table_size > CONFIG_DSP_MAX_FFT_SIZE
 *      - ESP_ERR_DSL_REINITIALIZED if buffer already allocated internally by other function
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_fft2r_init_32fc(float* fft_table_buff, int table_size);

/**
 * Free resources of Complex FFT. This function delete coefficients table if it was allocated by dsls_fft2r_init_32fc.
 * The implementation use ANSI C and could be compiled and run on any platform
 * 
 * 
 * @return
 */
void dsls_fft2r_deinit();

/**
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
// This is workaround because linker generates permanent error when assembler uses 
// direct access to the table pointer
#define dsls_fft2r_32fc_ae32(x,y) dsls_fft2r_32fc_ae32_(x,y, dsls_fft_w_table_32fc)
esp_err_t dsls_fft2r_32fc_ae32_(float *input, int N, float* w);

/**
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
esp_err_t dsls_cplx2reC_32fc_ansi(float *input, int N);

#ifdef __cplusplus
}
#endif

#ifdef CONFIG_DSP_OPTIMIZED
#define dsls_fft2r_32fc dsls_fft2r_32fc_ae32
#define dsls_bit_rev_32fc dsls_bit_rev_32fc_ansi
#define dsls_cplx2reC_32fc dsls_cplx2reC_32fc_ansi
#endif
#ifdef CONFIG_DSP_ANSI
#define dsls_fft2r_32fc dsls_fft2r_32fc_ansi
#define dsls_bit_rev_32fc dsls_bit_rev_32fc_ansi
#define dsls_cplx2reC_32fc dsls_cplx2reC_32fc_ansi
#endif


#endif // _dsls_fft2r_cf_H_