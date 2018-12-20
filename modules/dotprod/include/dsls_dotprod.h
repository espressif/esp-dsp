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

#ifndef _DSPI_DOTPROD_H_
#define _DSPI_DOTPROD_H_

#include "esp_log.h"
#include "dsl_err.h"

#ifdef __cplusplus
extern "C"
{
#endif
// These functions calculates dotproduct of two vectors.

/**
 * Dot product calculation for two signed 16 bit arrays: *dest += (src1[i] * src2[i]) >> (15-shift); i= [0..N)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param src1  source array 1
 * @param src2  source array 2
 * @param dest  destination pointer
 * @param len   length of input arrays
 * @param shift shift of the result.
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_dotprod_16s_ansi(int16_t *src1, int16_t *src2, int16_t *dest, int len, int8_t shift);

/**
 * Dot product calculation for two signed 16 bit arrays: *dest += (src1[i] * src2[i]) >> (15-shift); i= [0..N)
 * The implementation is optimized for ESP32 chip.
 *
 * @note this function will work correct when length is > 4. If application requires less then 5 please use generic a function dsls_dotprod_16s_ansi.
 *
 * @param src1  source array 1
 * @param src2  source array 2
 * @param dest  destination pointer
 * @param len   length of input arrays
 * @param shift shift of the result.
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_dotprod_16s_ae32(int16_t *src1, int16_t *src2, int16_t *dest, int len, int8_t shift);

/**
 * Dot product calculation for two floating point arrays: *dest += (src1[i] * src2[i]); i= [0..N)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param src1  source array 1
 * @param src2  source array 2
 * @param dest  destination pointer
 * @param len   length of input arrays
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_dotprod_32f_ansi(float *src1, float *src2, float *dest, int len);

/**
 * Dot product calculation for two floating point arrays: *dest += (src1[i*step1] * src2[i*step2]); i= [0..N)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param src1  source array 1
 * @param src2  source array 2
 * @param dest  destination pointer
 * @param len   length of input arrays
 * @param step1 step over elements in first array
 * @param step2 step over elements in second array
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_dotprode_32f_ansi(float *src1, float *src2, float *dest, int len, int step1, int step2);

/**
 * Dot product calculation for two floating point arrays: *dest += (src1[i] * src2[i]); i= [0..N)
 * The implementation is optimized for ESP32 chip.
 *
 * @param src1  source array 1
 * @param src2  source array 2
 * @param dest  destination pointer
 * @param len   length of input arrays
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_dotprod_32f_ae32(float *src1, float *src2, float *dest, int len);

/**
 * Dot product calculation for two floating point arrays: *dest += (src1[i*step1] * src2[i*step2]); i= [0..N)
 * The implementation is optimized for ESP32 chip.
 *
 * @param src1  source array 1
 * @param src2  source array 2
 * @param dest  destination pointer
 * @param len   length of input arrays
 * @param step1 step over elements in first array
 * @param step2 step over elements in second array
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_dotprode_32f_ae32(float *src1, float *src2, float *dest, int len, int step1, int step2);

#ifdef __cplusplus
}
#endif

#ifdef CONFIG_DSP_OPTIMIZED
#define dsls_dotprod_16s dsls_dotprod_16s_ae32
#define dsls_dotprod_32f dsls_dotprod_32f_ae32
#define dsls_dotprode_32f dsls_dotprode_32f_ae32
#endif
#ifdef CONFIG_DSP_ANSI
#define dsls_dotprod_16s dsls_dotprod_16s_ansi
#define dsls_dotprod_32f dsls_dotprod_32f_ansi
#define dsls_dotprode_32f dsls_dotprode_32f_ansi
#endif



#endif // _DSPI_DOTPROD_H_