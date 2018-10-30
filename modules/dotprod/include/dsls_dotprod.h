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

esp_err_t dsls_dotprod_16s_ae32(int16_t* src1, int16_t* src2, int16_t* dest, int len, int8_t shift);
esp_err_t dsls_dotprod_16s_ansi(int16_t* src1, int16_t* src2, int16_t* dest, int len, int8_t shift);
esp_err_t dsls_dotprod_16sc(int16_t* src1, int16_t* src2, int16_t* dest, int len, int8_t shift);
esp_err_t dsls_dotprod_32f_ansi(float* src1, float* src2, float* dest, int len);
esp_err_t dsls_dotprode_32f_ansi(float* src1, float* src2, float* dest, int len, int step1, int step2);
esp_err_t dsls_dotprod_32f_ae32(float* src1, float* src2, float* dest, int len);
esp_err_t dsls_dotprode_32f_ae32(float* src1, float* src2, float* dest, int len, int step1, int step2);
esp_err_t dsls_dotprod_32fc(float* src1, float* src2, float* dest, int len);

//#define dsls_dotprod_16s dsls_dotprod_16s_asm

#ifdef __cplusplus
}
#endif

#endif // _DSPI_DOTPROD_H_