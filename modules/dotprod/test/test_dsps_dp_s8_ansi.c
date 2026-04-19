// Copyright 2026 Espressif Systems (Shanghai) PTE LTD
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
#include <malloc.h>

#include "dsps_dotprod.h"
#include "dsp_tests.h"

static const char *TAG = "dsps_dp_s8_ansi";

// Test dsps_dp_s8_ansi function
TEST_CASE("dsps_dp_s8_ansi functionality", "[dsps]")
{
    int32_t check_value = 123;
    int max_N = 1024;
    int8_t *x = (int8_t *)memalign(16, max_N * sizeof(int8_t));
    int8_t *y = (int8_t *)memalign(16, max_N * sizeof(int8_t));
    int32_t *z = (int32_t *)memalign(16, max_N * sizeof(int32_t));

    for (int i = 0 ; i < max_N ; i++) {
        x[i] = 1;
        y[i] = 10;
    }

    // Check result == 0
    for (int i = 1; i < max_N; i++) {
        esp_err_t status = dsps_dp_s8_ansi(x, y, &z[1], i);
        TEST_ASSERT_EQUAL(status, ESP_OK);
        check_value = i * 10;
        ESP_LOGD(TAG, "check_value for i = %d: %d, z[1]: %d", (int)i, (int)check_value, (int)z[1]);
        TEST_ASSERT_EQUAL(check_value, z[1]);
    }

    free(x);
    free(y);
    free(z);
}

TEST_CASE("dsps_dp_s8_ansi benchmark", "[dsps]")
{
    int max_N = 1024;
    int8_t *x = (int8_t *)memalign(16, max_N * sizeof(int8_t));
    int8_t *y = (int8_t *)memalign(16, max_N * sizeof(int8_t));
    int32_t *z = (int32_t *)memalign(16, max_N * sizeof(int32_t));

    for (int i = 0 ; i < max_N ; i++) {
        x[i] = 1;
        y[i] = 10;
    }

    int check_N_max[] = {127, 128, 255, 256, 511, 512, 1023, 1024};
    for (int i = 0 ; i < sizeof(check_N_max) / sizeof(check_N_max[0]) ; i++) {
        int N = check_N_max[i];
        unsigned int start_b = dsp_get_cpu_cycle_count();
        dsps_dp_s8_ansi(x, y, &z[1], N);
        unsigned int end_b = dsp_get_cpu_cycle_count();
        float cycles = end_b - start_b;
        ESP_LOGI(TAG, "dsps_dp_s8_ansi - %f cycles for %d samples", cycles, N);
    }

    free(x);
    free(y);
    free(z);
}
