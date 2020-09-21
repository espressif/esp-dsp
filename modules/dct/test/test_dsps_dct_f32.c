// Copyright 2018-2020 Espressif Systems (Shanghai) PTE LTD
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
#include "dsps_dct.h"
#include "dsps_fft2r.h"
#include "dsp_tests.h"

static const char *TAG = "dsps_dct";

TEST_CASE("dsps_dct_f32 functionality", "[dsps]")
{
    float* data = calloc(1024*2, sizeof(float));
    float* data_ref = calloc(1024*2, sizeof(float));
    float* data_fft = calloc(1024*2, sizeof(float));

    int N = 64;
    int check_bin = 4;
    for (int i = 0 ; i < N ; i++) {
        data[i] = 2 * sin(M_PI / N * check_bin * 2 * i);
        data_ref[i] = data[i];
        data_fft[i] = data[i];
        data[i + N] = 0;
        data_ref[i + N] = 0;
        data_fft[i + N] = 0;
    }

    dsps_dct_f32_ref(data, N, &data[N]);
    dsps_view(&data[N], 32, 32, 10, -2, 2, '.');

    dsps_dct_inverce_f32_ref(&data[N], N, data);
    dsps_view(&data[0], 32, 32, 10, -2, 2, '.');

    for (size_t i = 0; i < N; i++) {
        ESP_LOGD(TAG, "DCT data[%i] = %2.3f\n", i, data[N + i]);
    }
    float abs_tol = 1e-5;
    for (size_t i = 0; i < N; i++) {
        ESP_LOGD(TAG, "data[%i] = %f, ref_data = %f\n", i, data[i], data_ref[i]*N / 2);
        float error = abs(data[i] - data_ref[i] * N / 2);
        if (error > abs_tol) {
            ESP_LOGE(TAG, "data[%i] = %f, ref_data = %f, error= %f\n", i, data[i], data_ref[i]*N / 2, error);
            TEST_ASSERT_MESSAGE (false, "Result out of range!\n");
        }
    }

    free(data);
    free(data_ref);
    free(data_fft);

}

TEST_CASE("dsps_dct_f32 functionality Fast DCT", "[dsps]")
{
    esp_err_t ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret  != ESP_OK) {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return;
    }

    float* data = calloc(1024*2, sizeof(float));
    float* data_ref = calloc(1024*2, sizeof(float));
    float* data_fft = calloc(1024*2, sizeof(float));

    int N = 64;
    int check_bin = 4;
    for (int i = 0 ; i < N ; i++) {
        data[i] = 2 * sin(M_PI / N * check_bin * 2 * i);
        data_ref[i] = data[i];
        data_fft[i] = data[i];
        data[i + N] = 0;
        data_ref[i + N] = 0;
        data_fft[i + N] = 0;
    }

    dsps_dct_f32_ref(data, N, &data[N]);
    ret = dsps_dct_f32(data_fft, N);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error!");
        return;
    }
    float abs_tol = 1e-5;
    for (size_t i = 0; i < N; i++) {
        ESP_LOGD(TAG, "DCT data[%i] = %2.3f, data_fft = %2.3f\n", i, data[N + i], data_fft[i]);
        float error = abs(data[N + i] - data_fft[i]);
        if (error > abs_tol) {
            ESP_LOGE(TAG, "DCT data[%i] = %f, data_fft = %f, error = %f\n", i, data[N + i], data_fft[i], error);
            TEST_ASSERT_MESSAGE (false, "Result out of range!\n");
        }
    }

    dsps_dct_inv_f32(data_fft, N);

    for (size_t i = 0; i < N; i++) {
        ESP_LOGD(TAG, "IDCT data[%i] = %2.3f, data_fft = %2.3f\n", i, data[i], data_fft[i] / N * 2);
        float error = abs(data[i] - data_fft[i] / N * 2);
        if (error > abs_tol) {
            ESP_LOGE(TAG, "IDCT data[%i] = %f, data_fft = %f, error = %f\n", i, data[i], data_fft[i] / N * 2, error);
            TEST_ASSERT_MESSAGE (false, "Result out of range!\n");
        }
    }
    dsps_fft2r_deinit_fc32();
    free(data);
    free(data_ref);
    free(data_fft);
}

TEST_CASE("dsps_dct_f32 benchmark", "[dsps]")
{
    esp_err_t ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret  != ESP_OK) {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return;
    }

    float* data = calloc(1024*2, sizeof(float));
    float* data_ref = calloc(1024*2, sizeof(float));
    float* data_fft = calloc(1024*2, sizeof(float));

    int N = 64;
    int check_bin = 4;
    for (int i = 0 ; i < N ; i++) {
        data[i] = 2 * sin(M_PI / N * check_bin * 2 * i);
        data[i + N] = 0;
    }

    unsigned int start_b = xthal_get_ccount();
    ret = dsps_dct_f32(data, N);
    unsigned int end_b = xthal_get_ccount();

    if (ret != ESP_OK) {
        TEST_ASSERT_MESSAGE (false, "Error!\n");
        return;
    }

    float total_b = end_b - start_b;
    float cycles = total_b;
    ESP_LOGI(TAG, "Benchmark dsps_dct_f32 - %6i cycles for %6i DCT points FFT.", (int)cycles, N);
    dsps_fft2r_deinit_fc32();
    free(data);
    free(data_ref);
    free(data_fft);
}
