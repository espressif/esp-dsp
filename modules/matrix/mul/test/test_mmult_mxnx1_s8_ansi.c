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
#include "esp_dsp.h"
#include "dsp_platform.h"
#include "esp_log.h"
#include <malloc.h>
#include "dspm_mult.h"
#include "esp_attr.h"
#include "dsp_tests.h"

static const char *TAG = "dspm_mult_mxn_1xm_int8_ansi";

// Test dspm_mult_mxn_1xm_int8_ansi functionality
TEST_CASE("dspm_mult_mxn_1xm_int8_ansi functionality", "[dspm]")
{
    int m = 32;
    int n = 32;

    int8_t *A = (int8_t *)memalign(16, m * n * sizeof(int8_t));
    int8_t *B = (int8_t *)memalign(16, n * sizeof(int8_t));
    int32_t *C = (int32_t *)memalign(16, m * sizeof(int32_t));

    for (int i = 0 ; i < m; i++) {
        for (int j = 0 ; j < n; j++) {
            A[i * n + j] = i;
        }
    }
    for (int i = 0 ; i < n; i++) {
        B[i] = 1;
    }
    dspm_mult_mxn_1xm_int8_ansi(A, B, C, m, n);

    for (int i = 0 ; i < m ; i++) {
        ESP_LOGD(TAG, "C[%i] calc=%i, expected =%i", i, (int)C[i], i * n);
        TEST_ASSERT_EQUAL(i * n, C[i]);
    }
    free(A);
    free(B);
    free(C);
}

// Test dspm_mult_mxn_1xm_int8_ansi functionality
TEST_CASE("dspm_mult_mxn_1xm_int8_ansi full functionality", "[dspm]")
{
    int m = 16;
    int n = 16;

    int8_t *A = (int8_t *)memalign(16, m * n * sizeof(int8_t));
    int8_t *B = (int8_t *)memalign(16, n * sizeof(int8_t));
    int32_t *C = (int32_t *)memalign(16, m * sizeof(int32_t));

    for (int i = 0 ; i < m; i++) {
        for (int j = 0 ; j < n; j++) {
            A[i * n + j] = 4;
        }
    }
    A[0] = 1;
    A[15] = 16;
    A[15 * 16] = 112;
    A[15 * 16 + 15] = 127;


    for (int i = 0 ; i < n; i++) {
        B[i] = 3;
    }
    B[0] = 1;
    B[7] = 8;
    B[8] = 121;
    B[15] = 127;


    dspm_mult_mxn_1xm_int8_ansi(A, B, C, 16, 16);
    ESP_LOGI(TAG, "A matrix:");
    for (int i = 0 ; i < 16 ; i++) {
        for (int j = 0 ; j < 16 ; j++) {
            printf("%4d ", (int)A[i * 16 + j]);
        }
        printf("\n");
    }
    ESP_LOGI(TAG, "B matrix:");
    for (int i = 0 ; i < 16 ; i++) {
        ESP_LOGI(TAG, "B[%i] = %i", i, (int)B[i]);
    }

    ESP_LOGI(TAG, "C matrix:");
    for (int i = 0 ; i < 16 ; i++) {
        ESP_LOGI(TAG, "C[%i] = %i", i, (int)C[i]);
    }
    free(A);
    free(B);
    free(C);
}

TEST_CASE("dspm_mult_mxn_1xm_int8_ansi benchmark", "[dspm]")
{
    int m = 128;
    int n = 128;

    int8_t *A = (int8_t *)memalign(16, m * n * sizeof(int8_t));
    int8_t *B = (int8_t *)memalign(16, n * sizeof(int8_t));
    int32_t *C = (int32_t *)memalign(16, m * sizeof(int32_t) + 16);

    for (int i = 0 ; i < m; i++) {
        for (int j = 0 ; j < n; j++) {
            A[i * n + j] = i;
        }
    }
    for (int i = 0 ; i < n; i++) {
        B[i] = 1;
    }
    int check_m_max[] = {16, 17, 32, 33, 64, 65, 128, 129, 80};
    int check_n_max[] = {16, 17, 32, 33, 64, 65, 128, 129, 128};
    for (int i = 0 ; i < sizeof(check_m_max) / sizeof(check_m_max[0]) ; i++) {
        int m = check_m_max[i];
        int n = check_n_max[i];
        unsigned int start_b = dsp_get_cpu_cycle_count();
        int repeat_count = 128;
        for (int i = 0 ; i < repeat_count ; i++) {
            dspm_mult_mxn_1xm_int8(A, B, C, m, n);
        }
        unsigned int end_b = dsp_get_cpu_cycle_count();
        float cycles = end_b - start_b;
        ESP_LOGI(TAG, "dspm_mult_mxn_1xm_int8_ansi - %f cycles for %d rows and %d columns", cycles / repeat_count, m, n);
    }
    free(A);
    free(B);
    free(C);
}
