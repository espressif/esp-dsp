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

#ifndef _dsls_tone_gen_H_
#define _dsls_tone_gen_H_
#include "dsl_err.h"


#ifdef __cplusplus
extern "C"
{
#endif

// x - input buffer
// len - length of the buffer
// A - amplitude
// f - frequency -1...1 
// ph - phase in degree
//
// x[i] = A*sin(2*PI*i + ph/180*PI)
esp_err_t dsls_tone_gen_f32(float* x, int len, float A, float f, float ph);

#ifdef __cplusplus
}
#endif

#endif // _dsls_tone_gen_H_