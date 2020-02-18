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

#include "dsps_view.h"
#include "dsps_fft2r.h"
#include "dsp_tests.h"

static const char *TAG = "dsps_fft2r_ansi";

static float data[1024*2];

TEST_CASE("dsps_fft2r_fc32_ansi functionality", "[dsps]")
{
    int N = sizeof(data) / sizeof(float) / 2;
    int check_bin = 32;
    for (int i = 0 ; i < N ; i++) {
        data[i * 2 + 0] = 2 * sinf(M_PI / N * check_bin * 2 * i) / (N / 2);
        data[i * 2 + 1] = 0;
    }

    esp_err_t ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret  != ESP_OK)
    {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return;
    }

    unsigned int start_b = xthal_get_ccount();
    dsps_fft2r_fc32_ansi(data, N);
	unsigned int end_b = xthal_get_ccount();
    dsps_bit_rev_fc32_ansi(data, N);
    
    float min = 10000;
    float max = -10000;
    int max_pos = 0;
    for (int i = 0 ; i < N ; i++) {
        data[i] = 10 * log10f(data[i * 2 + 0] * data[i * 2 + 0] + data[i * 2 + 1] * data[i * 2 + 1]);
        if (data[i] < min) {
            min = data[i];
        }
        if (data[i] > max) {
            max = data[i];
            max_pos = i;
        }
        ESP_LOGD(TAG, "FFT Data[%i] =%8.4f dB", i, data[i]);
    }
    dsps_view_spectrum(data, 256, -160, 40);

    TEST_ASSERT_EQUAL( check_bin, max_pos);
    float round_pow = round(max * 10);
    TEST_ASSERT_EQUAL( 6 * 10, round_pow);
    ESP_LOGI(TAG, "Calculation error is less then 0.1 dB");
    ESP_LOGI(TAG, "cycles - %i", end_b - start_b);
    dsps_fft2r_deinit_fc32();
}

TEST_CASE("dsps_fft2r_fc32_ansi benchmark", "[dsps]")
{    
    esp_err_t ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret  != ESP_OK)
    {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return;
    }
    for (int i= 5 ; i< 10 ; i++)
    {
        int N_check = 2<<i;
        unsigned int start_b = xthal_get_ccount();
        dsps_fft2r_fc32_ansi(data, N_check);

        unsigned int end_b = xthal_get_ccount();
        float total_b = end_b - start_b;
        float cycles = total_b;
        ESP_LOGI(TAG, "Benchmark dsps_fft2r_fc32_ansi - %6i cycles for %6i points FFT.", (int)cycles, N_check);
        float min_exec = 3;
        float max_exec = 330000*3;
        TEST_ASSERT_EXEC_IN_RANGE(min_exec, max_exec, cycles);
    }
    dsps_fft2r_deinit_fc32();
}
