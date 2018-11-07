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
#include "soc/cpu.h"
#include "esp_log.h"

#include "dsls_tone_gen.h"
#include "dsls_d_gen.h"
#include "dsls_biquad_gen.h"
#include "dsls_biquad.h"

static const char *TAG = "dsls_biquad_32f_ae32";

float x[1024];
float y[1024];
float z[1024];

TEST_CASE("dsls_biquad_32f_ae32 functionality", "[dsls]")
{
    // In the test we generate filter with cutt off frequency 0.1
    // and then filtering 0.1 and 0.3 frequencis.
    // Result must be better then 24 dB
    int len = sizeof(x)/sizeof(float);

//    dsls_tone_gen_f32(x, len, 1, 0.1, 0);
    dsls_d_gen_f32(x, len, 0);
    float coeffs[5];
    float w1[2] = {0};
    float w2[2] = {0};
    dsls_biquad_gen_lpf_32f(coeffs, 0.1, 1, 1);
    dsls_biquad_32f_ae32(x, y, len, coeffs, w1);
    dsls_biquad_32f_ansi(x, z, len, coeffs, w2);

    for (int i=0 ; i< 32 ; i++)
    {
        printf("[%i]calc = %f, expected=%f\n", i, y[i], z[i]);
        // if (y[i] != z[i])
        // {
        //     TEST_ASSERT_EQUAL( y[i], z[i]);
        // }
    }
}

TEST_CASE("dsls_biquad_32f_ae32 benchmark", "[dsls]")
{
    float w1[2] = {0};
    float w2[2] = {0};
    int len = sizeof(x)/sizeof(float);
    int repeat_count = 1024;
    dsls_d_gen_f32(x, len, 0);
    float coeffs[5];
    dsls_biquad_gen_lpf_32f(coeffs, 0.1, 1, 1);


    unsigned int start_b = xthal_get_ccount();
    for (int i=0 ; i< repeat_count ; i++)
    {
        dsls_biquad_32f_ae32(x, y, len, coeffs, w1);
    }
    unsigned int end_b = xthal_get_ccount();

    float total_b = end_b - start_b;
    float cycles = total_b/(len*repeat_count);

    start_b = xthal_get_ccount();
    for (int i=0 ; i< repeat_count ; i++)
    {
        dsls_biquad_32f_ansi(x, y, len, coeffs, w1);
    }
    end_b = xthal_get_ccount();
    float total_b_ansi = end_b - start_b;
    float cycles_ansi = total_b_ansi/(len*repeat_count);

    ESP_LOGI(TAG, "dsls_biquad_32f_ae32 - %f per sample\n", cycles);
    ESP_LOGI(TAG, "dsls_biquad_32f_ansi - %f per sample\n", cycles_ansi);
    // float min_exec = 10;
    // float max_exec = 20;
    // if (cycles >= max_exec) { 
    //     TEST_ASSERT_MESSAGE (false, "Exec time takes more then expected!");
    // }
    // if (cycles < min_exec) { 
    //     TEST_ASSERT_MESSAGE (false, "Exec time takes less then expected!");
    // }

}