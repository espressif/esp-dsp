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

#include <string.h>
#include "unity.h"
#include "dsp_platform.h"
#include "esp_log.h"

#include "dsps_addc.h"
#include "esp_attr.h"


TEST_CASE("dsps_addc_f32_ansi functionality", "[dsps]")
{
    int n = 64;
    float x[n];
    float y[n];
    for (int i = 0 ; i < n ; i++) {
        x[i] = i;
        y[i] = i + 10;
    }
    dsps_addc_f32_ansi(x, x, n, 10, 1, 1);
    for (int i = 0 ; i < n ; i++) {
        if (x[i] != y[i]) {
            TEST_ASSERT_EQUAL(x[i], y[i]);
        }
    }
}
