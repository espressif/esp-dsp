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

#ifndef _dsls_h_gen_H_
#define _dsls_h_gen_H_
#include "dsl_err.h"


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @function dsls_h_gen_f32
 * The Heviside function.
 * x[i]=0, if i=[0..pos)
 * x[i]=1, if i=[pos..N)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param x: input array.
 * @param len: length of the input signal
 * @param pos: heviside function position
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */

esp_err_t dsls_h_gen_f32(float *x, int len, int pos);

#ifdef __cplusplus
}
#endif

#endif // _dsls_h_gen_H_