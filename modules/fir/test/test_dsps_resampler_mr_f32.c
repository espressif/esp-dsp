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

static const char *TAG = "dsps_resampler_mr_f32";

#define INTERPOLATION 10
#define DECIMATION 9
#define N_IN_SAMPLES 4096
#define COEFFS INTERPOLATION*12

static int32_t len = N_IN_SAMPLES;
static int16_t fir_len = COEFFS;
static float samplerate_factor = (float)480 / 441;

TEST_CASE("dsps_resampler_mr_f32 functionality", "[dsps]")
{

    float *x = (float *)memalign(16, len * sizeof(float));
    float *y = (float *)memalign(16, 2 * len * sizeof(float));

    float *coeffs = (float *)memalign(16, fir_len * sizeof(float));
    float *delay = (float *)memalign(16, fir_len * sizeof(float));

    dsps_resample_mr_t fir_resampler;

    for (int i = 0 ; i < fir_len ; i++) {
        coeffs[i] = 0;
    }
    coeffs[0] = 0x4000;

    for (int i = 0 ; i < len ; i++) {
        x[i] = i;
    }

    esp_err_t status = dsps_resampler_mr_init(&fir_resampler, coeffs, fir_len, INTERPOLATION, samplerate_factor, 0, 0);
    TEST_ESP_OK(status);

    int out_pos = 0;
    int setp = 32;
    int out_count = 0;
    float expected_count = 0;
    int32_t correct_length = 0;
    for (int i = 0 ; i < 44100 ; i++) {
        correct_length = 0;
        if (expected_count > (out_pos + 1)) {
            correct_length = 1;
        }
        if (expected_count < (out_pos - 1)) {
            correct_length = -1;
        }

        out_count = dsps_resampler_mr_exec(&fir_resampler, &x[0], &y[0], setp, correct_length);
        expected_count = setp * (i + 1) * samplerate_factor;
        out_pos += out_count;
    }
    ESP_LOGI(TAG, "current count = %d, expected_count=%i", (int)out_pos, (int)expected_count);
    int diff_count = abs(out_pos - (int)expected_count);
    if (diff_count > 2) {
        ESP_LOGE(TAG, "Expected: %i, reached: %i", (int)expected_count, (int)out_pos);
        TEST_ASSERT_MESSAGE (false, "Length difference more than expected! ");
    }

    dsps_resampler_mr_free(&fir_resampler);
    free(x);
    free(y);
    free(coeffs);
    free(delay);
}
