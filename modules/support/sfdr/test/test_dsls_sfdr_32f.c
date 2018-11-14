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
#include "test_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portable.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_clk.h"
#include "esp_log.h"
#include "soc/cpu.h"

#include "dsls_view.h"
#include "dsls_sfdr.h"


static const char *TAG = "dsls_sfdr_32f";

static float data[1024];

TEST_CASE("dsls_sfdr_32f functionality", "[dsls]")
{
    int N = sizeof(data)/sizeof(float)/2;
    int check_bin = 32;
    float sfdr_exp = 4;
    for (int i=0 ; i< N ; i++)
    {
        data[i] = 4*sinf(M_PI/N*check_bin*i)/(N/2);
        data[i] += sinf(M_PI/N*check_bin*i*2)/(N/2);
    }
    float sfdr = dsls_sfdr_32f(data, N, 1);
    TEST_ASSERT_EQUAL( (int)20*log10(sfdr_exp), (int)sfdr);
	ESP_LOGI(TAG, "dsls_sfdr_32f = %f dB", sfdr);
}
