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

#ifndef _dslm_mult_H_
#define _dslm_mult_H_

#include "dsl_err.h"

esp_err_t dslm_mult_32f_ansi(float* A, float* B, float* C, int m, int n, int k);
esp_err_t dslm_mult_32f_ae32(float* A, float* B, float* C, int m, int n, int k);
esp_err_t dslm_mult_3x3x1_32f_ae32(float* A, float* B, float* C);
esp_err_t dslm_mult_3x3x3_32f_ae32(float* A, float* B, float* C);
esp_err_t dslm_mult_4x4x1_32f_ae32(float* A, float* B, float* C);
esp_err_t dslm_mult_4x4x4_32f_ae32(float* A, float* B, float* C);

esp_err_t dslm_mult_16s_ansi(int16_t* A, int16_t* B, int16_t* C, int m, int n, int k, int shift);
esp_err_t dslm_mult_16s_ae32(int16_t* A, int16_t* B, int16_t* C, int m, int n, int k, int shift);

#endif // _dslm_mult_H_