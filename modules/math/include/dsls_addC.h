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

#ifndef _dsls_addC_H_
#define _dsls_addC_H_
#include "dsl_err.h"


#ifdef __cplusplus
extern "C"
{
#endif

// x - input buffer
// len - length of the buffer
// pos - position of inpuls [0..len)
// step1 - step through 1 array (by default - 1)
// step2 - step through 2 array (by default - 1)

esp_err_t dsls_addC_32f_ansi(float* x, float* y, int len, float C, int step1, int step2);

#ifdef __cplusplus
}
#endif

#endif // _dsls_addC_H_