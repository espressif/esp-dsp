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
#include "freertos/FreeRTOS.h"
#include "freertos/portable.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_clk.h"
#include "soc/cpu.h"

#include "esp_dsp.h"

static const int length = 1024;
static float data[1024];

// This test check if the window is symmetric
TEST_CASE("dsps_wind_hann_f32: test Hann window for symmetry", "[dsps]")
{
    dsps_wind_hann_f32(data, length);
    float hann_diff = 0;
    for (int i=0 ; i< length/2 ; i++)
    {
        hann_diff += fabs(data[i] - data[length - 1 -i]);
    }
	
	if (hann_diff > 8) TEST_ASSERT_EQUAL(0, hann_diff);
}
