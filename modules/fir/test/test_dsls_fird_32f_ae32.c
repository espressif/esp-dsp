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
#include "dsls_fir.h"

static const char *TAG = "dsls_fird_32f_ae32";

static float x[1024];
static float y[1024];
static float y_compare[1024];

static float coeffs[32];
static float delay[32];
static float delay_compare[32];

TEST_CASE("dsls_fird_32f_ae32 functionality", "[dsls]")
{
    // In the test we generate filter with cutt off frequency 0.1
    // and then filtering 0.1 and 0.3 frequencis.
    // Result must be better then 24 dB
    int len = sizeof(x) / sizeof(float);
    int fir_len = sizeof(coeffs) / sizeof(float);
    int decim = 4;

    fir_32f_t fir1;
    fir_32f_t fir2;
    for (int i = 0 ; i < fir_len ; i++) {
        coeffs[i] = i;
    }
    coeffs[0] = 1;

    for (int i = 0 ; i < len ; i++) {
        x[i] = i;
    }

    dsls_fird_init_32f(&fir1, coeffs, delay, fir_len, decim, 0);
    dsls_fird_init_32f(&fir2, coeffs, delay_compare, fir_len, decim, 0);
    int total1 = dsls_fird_32f_ae32(&fir1, x, y, len);
    int total2 = dsls_fird_32f_ansi(&fir2, x, y_compare, len);
    total1 += dsls_fird_32f_ae32(&fir1, x, y, len);
    total2 += dsls_fird_32f_ansi(&fir2, x, y_compare, len);
    total1 += dsls_fird_32f_ae32(&fir1, x, y, len);
    total2 += dsls_fird_32f_ansi(&fir2, x, y_compare, len);
    ESP_LOGI(TAG, "Total result = %i, expected %i from %i", total1, total2, len);
    TEST_ASSERT_EQUAL(total1, total2);
    for (int i=0 ; i< total1 ; i++)
    {
        ESP_LOGD(TAG, "data[%i] = %f\n", i, y[i]);
    }
    for (int i = 0 ; i < total1 ; i++) {
        if (y[i] != y_compare[i]) {
            TEST_ASSERT_EQUAL(y[i], y_compare[i]);
        }
    }


}


TEST_CASE("dsls_fird_32f_ae32 benchmark", "[dsls]")
{

    int len = sizeof(x) / sizeof(float);
    int fir_len = sizeof(coeffs) / sizeof(float);
    int repeat_count = 1;
    int decim = 4;

    fir_32f_t fir1;
    for (int i = 0 ; i < fir_len ; i++) {
        coeffs[i] = i;
    }

    for (int i = 0 ; i < len ; i++) {
        x[i] = 0;
    }
    x[0] = 1;

    dsls_fird_init_32f(&fir1, coeffs, delay, fir_len, decim, 0);

    unsigned int start_b = xthal_get_ccount();
    for (int i = 0 ; i < repeat_count ; i++) {
        dsls_fird_32f_ae32(&fir1, x, y, len);
    }
    unsigned int end_b = xthal_get_ccount();

    float total_b = end_b - start_b;
    float cycles = total_b / (len * repeat_count);

    ESP_LOGI(TAG, "dsls_fir_32f_ae32 - %f per sample for for %i coefficients, %f per decim tap \n", cycles, fir_len, cycles / (float)fir_len * decim);
    float min_exec = 3;
    float max_exec = 300;
    if (cycles >= max_exec) {
        TEST_ASSERT_MESSAGE (false, "Exec time takes more then expected!");
    }
    if (cycles < min_exec) {
        TEST_ASSERT_MESSAGE (false, "Exec time takes less then expected!");
    }
}
