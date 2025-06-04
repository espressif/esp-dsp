/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include "unity.h"
#include "esp_dsp.h"
#include "dsp_platform.h"
#include "esp_log.h"

#include "dsps_tone_gen.h"
#include "dsps_d_gen.h"
#include "dsps_fir.h"
#include "dsp_tests.h"

static const char *TAG = "dsps_firmr_f32_ansi";

static float *x;
static float *y;

static float coeffs[120];
static float delay[32];

TEST_CASE("dsps_firmr_f32_ansi functionality", "[dsps]")
{
    int len = 1024;
    int decim = 4;
    int interp = 5;
    int fir_len = interp * 12;

    x = (float *)malloc(len * sizeof(float));
    y = (float *)malloc(interp * len / decim * sizeof(float));

    fir_f32_t fir1;
    for (int i = 0 ; i < fir_len ; i++) {
        coeffs[i] = 0;
    }
    coeffs[0] = 1;

    for (int i = 0 ; i < len ; i++) {
        x[i] = i;
    }

    dsps_firmr_init_f32(&fir1, coeffs, delay, fir_len, interp, decim, 0);
    int total = dsps_firmr_f32_ansi(&fir1, x, y, len);
    ESP_LOGI(TAG, "Total result = %i from %i", total, len);
    TEST_ASSERT_EQUAL(total, len * interp / decim);
    for (int i = 0 ; i < total ; i++) {
        ESP_LOGD(TAG, "data[%i] = %f", i, y[i]);
    }
    int decim_count = 0;
    for (int i = 0 ; i < total ; i++) {
        if (i % interp == 0) {
            TEST_ASSERT_EQUAL(y[i], decim_count * decim);
            decim_count++;
        } else {
            TEST_ASSERT_EQUAL(y[i], 0);
        }
    }

    free(x);
    free(y);
}


TEST_CASE("dsps_firmr_f32_ansi benchmark", "[dsps]")
{
    int len = 1024;
    int fir_len = sizeof(coeffs) / sizeof(float);
    int decim = 4;
    int interp = 5;
    int repeat_count = 100;

    x = (float *)malloc(len * sizeof(float));
    y = (float *)malloc(interp * len / decim * sizeof(float));


    fir_f32_t fir1;
    for (int i = 0 ; i < fir_len ; i++) {
        coeffs[i] = i;
    }

    for (int i = 0 ; i < len ; i++) {
        x[i] = 0;
    }
    x[0] = 1;

    dsps_firmr_init_f32(&fir1, coeffs, delay, fir_len, interp, decim, 0);

    unsigned int start_b = dsp_get_cpu_cycle_count();
    for (int i = 0 ; i < repeat_count ; i++) {
        dsps_firmr_f32_ansi(&fir1, x, y, len);
    }
    unsigned int end_b = dsp_get_cpu_cycle_count();

    float total_b = end_b - start_b;
    float cycles = total_b / (len * repeat_count);

    ESP_LOGI(TAG, "dsps_firmr_f32_ansi - %f per sample for for %i coefficients, %f per decim tap \n", cycles, fir_len, cycles / (float)fir_len * decim / interp);
    float min_exec = 10;
    float max_exec = 300;
    TEST_ASSERT_EXEC_IN_RANGE(min_exec, max_exec, cycles);
}
