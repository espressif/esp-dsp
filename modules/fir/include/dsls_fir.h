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

#ifndef _dsls_fir_H_
#define _dsls_fir_H_


#include "dsl_err.h"


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct fir_32f_s
{
	float*  coeffs;
	float*  delay;
	int     N;
	int     pos;
	int     d;
	int     d_pos;
}fir_32f_t;

/**
 * @function dsls_fir_init_32f
 * Function initialize structure for 32 bit floating point FIR filter
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param fir: pointer to fir filter structure, that must be preallocated
 * @param coeffs: array with FIR filter coefficients. Must be length N
 * @param delay: array for FIR filter delay line. Must be length N
 * @param N: FIR filter length. Length of coeffs and delay arrays.
 * 
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_fir_init_32f(fir_32f_t* fir, float* coeffs, float* delay, int N);

/**
 * @function dsls_fird_init_32f
 * Function initialize structure for 32 bit floating point FIR filter with decimation
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param fir: pointer to fir filter structure, that must be preallocated
 * @param coeffs: array with FIR filter coefficients. Must be length N
 * @param delay: array for FIR filter delay line. Must be length N
 * @param N: FIR filter length. Length of coeffs and delay arrays.
 * @param d: decimation factor.
 * @param start_pos: initial value of decimation counter. Must be [0..d)
 * 
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_fird_init_32f(fir_32f_t* fir, float* coeffs, float* delay, int N, int d, int start_pos);


/**
 * @function dsls_fir_32f_ansi
 * Function implements FIR filter 
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param fir: pointer to fir filter structure, that must be initialized before
 * @param x: input array
 * @param y: array with result of FIR filter
 * @param len: length of input and result arrays
 * 
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_fir_32f_ansi(fir_32f_t* fir, float* x, float* y, int len);
/**
 * @function dsls_fir_32f_ansi
 * Function implements FIR filter 
 * The implementation optimized for Esp32 platform
 *
 * @param fir: pointer to fir filter structure, that must be initialized before
 * @param x: input array
 * @param y: array with result of FIR filter
 * @param len: length of input and result arrays
 * 
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_fir_32f_ae32(fir_32f_t* fir, float* x, float* y, int len);

/**
 * @function dsls_fir_32f_ansi
 * Function implements FIR filter with decimation
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param fir: pointer to fir filter structure, that must be initialized before
 * @param x: input array
 * @param y: array with result of FIR filter
 * @param len: length of input and result arrays
 * 
 * @return: function returns amount of samples stored to the output array
 *          depends on the previous state value could be [0..len/decimation]
 */
int dsls_fird_32f_ansi(fir_32f_t* fir, float* x, float* y, int len);

/**
 * @function dsls_fir_32f_ansi
 * Function implements FIR filter with decimation
 * The implementation optimized for Esp32 platform
 *
 * @param fir: pointer to fir filter structure, that must be initialized before
 * @param x: input array
 * @param y: array with result of FIR filter
 * @param len: length of input and result arrays
 * 
 * @return: function returns amount of samples stored to the output array
 *          depends on the previous state value could be [0..len/decimation]
 */
int dsls_fird_32f_ae32(fir_32f_t* fir, float* x, float* y, int len);

#ifdef __cplusplus
}
#endif

#endif // _dsls_fir_H_