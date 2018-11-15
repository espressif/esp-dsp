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

#include "dsls_wind_Barrel.h"
#include <math.h>

void dsls_wind_Barrel_32f(float *window, int len)
{
    float pi = 4.0 * atan(1.0);
    for (int i = 0; i < len; i++) {
        window[i] = 0.5 * (1 - cosf(i * 2 * pi / (float)len));
    }
}
