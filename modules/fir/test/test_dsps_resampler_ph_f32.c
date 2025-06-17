/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <malloc.h>
#include <stdint.h>
#include "unity.h"
#include "dsp_platform.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_dsp.h"
#include "dsp_tests.h"

static const char *TAG = "dsps_resampler_ph_f32";

TEST_CASE("dsps_resampler_ph_f32 functionality", "[dsps]")
{
    int32_t len = 1000;
    float *x = (float *)memalign(16, len * sizeof(float));
    float *y = (float *)memalign(16, 2 * len * sizeof(float));

    for (int i = 0; i < len; i++) {
        x[i] = sinf((2 * M_PI * 0.125 / 4 * i));
    }

    float samplerate_factor = 1.1f;
    dsps_resample_ph_t resampler;

    esp_err_t status = dsps_resampler_ph_init(&resampler, samplerate_factor);
    TEST_ESP_OK(status);

    int frame_size = 100;
    int result_len = 0;
    for (int i = 0; i < len / frame_size; i++) {
        int32_t result = dsps_resampler_ph_exec(&resampler, &x[i * frame_size], &y[result_len], frame_size);
        result_len += result;
        ESP_LOGD(TAG, "result_len: %d, phase: %f, delay_pos: %d", result_len, resampler.phase, resampler.delay_pos);
    }

    if (abs(result_len - (int)(len * samplerate_factor)) > 1) {
        ESP_LOGE(TAG, "Expected: %i, reached: %i", (int)(len * samplerate_factor), (int)result_len);
        TEST_ASSERT_MESSAGE (false, "Length difference more than expected! ");
    }
    free(x);
    free(y);
}


TEST_CASE("dsps_resampler_ph_f32 functionality view", "[dsps]")
{
    int32_t len = 1500;
    float *x = (float *)memalign(16, 2048 * sizeof(float));
    float *y = (float *)memalign(16, 2 * 2048 * sizeof(float));
    float check_freq = 0.125;
    for (int i = 0; i < len; i++) {
        x[i] = sinf((2 * M_PI * check_freq * i));
    }

    float samplerate_factor = 1.2f;
    dsps_resample_ph_t resampler;

    esp_err_t ret = dsps_resampler_ph_init(&resampler, samplerate_factor);
    TEST_ESP_OK(ret);
    ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    TEST_ESP_OK(ret);

    int frame_size = 100;
    int result_len = 0;
    for (int i = 0; i < len / frame_size; i++) {
        int32_t result = dsps_resampler_ph_exec(&resampler, &x[i * frame_size], &y[result_len], frame_size);
        result_len += result;
        ESP_LOGD(TAG, "result_len: %d, phase: %f, delay_pos: %d", result_len, resampler.phase, resampler.delay_pos);
    }

    float *window = (float *)malloc(1024 * sizeof(float));
    dsps_wind_hann_f32(window, 1024);

    for (int i = 0; i < 1024; i++) {
        x[i * 2 + 0] = (y[i] + sinf((2 * M_PI * check_freq * i))) * window[i];
        x[i * 2 + 1] = 0;
    }

    int n_fft = 1024;
    dsps_fft2r_fc32_ansi(x, n_fft);
    dsps_bit_rev_fc32_ansi(x, n_fft);
    for (int i = 0; i < n_fft / 4; i++) {
        y[i] = 10 * log10f(x[i * 2 + 0] * x[i * 2 + 0] + x[i * 2 + 1] * x[i * 2 + 1]);
        ESP_LOGD(TAG, "y[%d]: %f", i, y[i]);
    }
    dsps_view_spectrum(y, n_fft / 4, -20, 60);
    int f1 = roundf(n_fft * check_freq);
    int f2 = roundf(n_fft * check_freq / samplerate_factor);
    ESP_LOGI(TAG, "f1: %d, f2: %d, diff: %f", f1, f2, fabs(y[f1] - y[f2]));
    if (fabs(y[f1] - y[f2]) > 2) {
        TEST_ASSERT_MESSAGE(false, "Signal difference more than expected! ");
    }

    free(window);
    free(x);
    free(y);
}
