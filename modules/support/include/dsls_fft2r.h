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

esp_err_t dsls_fft2r_32fc(float* input, float* w, int N);
esp_err_t dsls_bit_rev_32fc(float *input, int N);
esp_err_t dsls_gen_w_r2_32fc(float *w, int N);
esp_err_t dsls_cplx2reC_32fc(float *input, int N);

#ifdef __cplusplus
}
#endif

#endif // _dsls_fft2r_cf_H_