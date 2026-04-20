/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "dspm_mult.h"
#include "dsps_dotprod.h"


esp_err_t dspm_mult_mxn_1xm_int8_ansi(const int8_t *A, const int8_t *B, int32_t *C, int M, int N)
{
    for (int i = 0 ; i < M ; i++) {
        int32_t acc = 0;
        dsps_dp_s8(A + i * N, B, &acc, N);
        C[i] = acc;
    }
    return ESP_OK;
}
