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


#ifndef _dsls_biquad_H_
#define _dsls_biquad_H_

#include "dsl_err.h"


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * IIR filter 2nd order direct form II (bi quad)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param x: input array
 * @param y: output array
 * @param len: length of input and output vectors
 * @param coef: array of coefficients. b0,b1,b2,a1,a2
 *              expected that a0 = 1. b0..b2 - numerator, a0..a2 - denominator
 * @param w: delay line w0,w1. Length of 2.
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_32f_ansi(float *x, float *y, int len, float *coef, float *w);

/**
 * IIR filter 2nd order direct form II (bi quad)
 * The implementation is optimized for ESP32 chip.
 *
 * @param x: input array
 * @param y: output array
 * @param len: length of input and output vectors
 * @param coef: array of coefficients. b0,b1,b2,a1,a2
 *              expected that a0 = 1. b0..b2 - numerator, a0..a2 - denominator
 * @param w: delay line w0,w1. Length of 2.
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_32f_ae32(float *x, float *y, int len, float *coef, float *w);

#ifdef __cplusplus
}
#endif


#ifdef CONFIG_DSP_OPTIMIZED
#define dsls_biquad_32f dsls_biquad_32f_ae32
#endif
#ifdef CONFIG_DSP_ANSI
#define dsls_biquad_32f dsls_biquad_32f_ansi
#endif

#endif // _dsls_biquad_H_