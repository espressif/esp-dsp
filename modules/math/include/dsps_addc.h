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

#ifndef _dsps_addc_H_
#define _dsps_addc_H_
#include "dsp_err.h"


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief   add constant
 * 
 * The function adds constant to the input array
 * x[i*step1] = y[i*step2] + C; i=[0..len)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param[in] input: input array
 * @param output: output array
 * @param len: amount of operations for arrays
 * @param C: constant value
 * @param step1: step over x array (by default should be 1)
 * @param step2: step over y array (by default should be 1)
 * 
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_addc_f32_ansi(const float *input, float *output, int len, float C, int step1, int step2);

#ifdef __cplusplus
}
#endif


#ifdef CONFIG_DSP_OPTIMIZED
#define dsps_addc_f32 dsps_addc_f32_ansi
#endif
#ifdef CONFIG_DSP_ANSI
#define dsps_addc_f32 dsps_addc_f32_ansi
#endif

#endif // _dsps_addc_H_