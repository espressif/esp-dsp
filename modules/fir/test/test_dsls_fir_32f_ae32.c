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

static const char *TAG = "dsls_fir_32f_ae32";

float x[1024];
float y[1024];
float y_compare[1024];

float coeffs[32];
float delay[32];
float delay_compare[32];

TEST_CASE("dsls_fir_32f_ae32 functionality", "[dsls]")
{
    // In the test we generate filter with cutt off frequency 0.1
    // and then filtering 0.1 and 0.3 frequencis.
    // Result must be better then 24 dB
    int len = sizeof(x)/sizeof(float);
    int fir_len = sizeof(coeffs)/sizeof(float);
    
    fir_32f_t fir1;
    fir_32f_t fir2;
    for (int i=0 ; i< fir_len ; i++)
    {
        coeffs[i] = i;
    }

    for (int i=0 ; i< len ; i++)
    {
        x[i] = 0;
    }
    x[0] = 1;

    dsls_fir_init_32f(&fir1, coeffs, delay, fir_len);
    dsls_fir_32f_ae32(&fir1, x, y, len);
    int32_t* ttt = (int32_t*)y;
    // for (int i=0 ; i< fir_len*3 ; i++)
    // {
    //     printf("fir[%i] = 0x%08x\n", i, ttt[i]);
    // }
    
    for (int i=0 ; i< fir_len ; i++)
    {
        if (y[i] != i)
        {
            TEST_ASSERT_EQUAL(y[i], i);
        }
    }
    
    // Check even length
    fir_len--;
    for (int i=0 ; i< fir_len ; i++)
    {
        coeffs[i] = i;
    }

    for (int i=0 ; i< len ; i++)
    {
        x[i] = i;
    }
    x[0] = 1;
    dsls_fir_init_32f(&fir1, coeffs, delay, fir_len);
    dsls_fir_32f_ae32(&fir1, x, y, len);
    dsls_fir_init_32f(&fir2, coeffs, delay_compare, fir_len);
    dsls_fir_32f_ansi(&fir2, x, y_compare, len);
    dsls_fir_32f_ae32(&fir1, x, y, len);
    dsls_fir_32f_ansi(&fir2, x, y_compare, len);
    dsls_fir_32f_ae32(&fir1, x, y, len);
    dsls_fir_32f_ansi(&fir2, x, y_compare, len);
    
    for (int i=0 ; i< len ; i++)
    {
        if (y[i] != y_compare[i])
        {
            TEST_ASSERT_EQUAL(y[i], y_compare[i]);
        } 
        // else
        // {
        //     printf("data[%i] = %f, %f\n", i, y[i], y_compare[i]);
        // }
    }
}

TEST_CASE("dsls_fir_32f_ae32 benchmark", "[dsls]")
{

    int len = sizeof(x)/sizeof(float);
    int fir_len = sizeof(coeffs)/sizeof(float);
    int repeat_count = 1;

    fir_32f_t fir1;
    for (int i=0 ; i< fir_len ; i++)
    {
        coeffs[i] = i;
    }

    for (int i=0 ; i< len ; i++)
    {
        x[i] = 0;
    }
    x[0] = 1;

    dsls_fir_init_32f(&fir1, coeffs, delay, fir_len);

    unsigned int start_b = xthal_get_ccount();
    for (int i=0 ; i< repeat_count ; i++)
    {
        dsls_fir_32f_ae32(&fir1, x, y, len);
    }
    unsigned int end_b = xthal_get_ccount();

    float total_b = end_b - start_b;
    float cycles = total_b/(len*repeat_count);

    ESP_LOGI(TAG, "dsls_fir_32f_ae32 - %f per sample for for %i coefficients, %f per tap \n", cycles, fir_len, cycles/(float)fir_len);

    float min_exec = 3;
    float max_exec = 800;
    if (cycles >= max_exec) { 
        TEST_ASSERT_MESSAGE (false, "Exec time takes more then expected!");
    }
    if (cycles < min_exec) { 
        TEST_ASSERT_MESSAGE (false, "Exec time takes less then expected!");
    }
}
