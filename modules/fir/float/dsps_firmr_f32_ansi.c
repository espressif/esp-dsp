/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */



#include "dsps_fir.h"
#include "dsp_common.h"
#include <malloc.h>
#include "dsp_tests.h"

int dsps_firmr_f32_ansi(fir_f32_t *fir, const float *input, float *output, int input_len)
{
    int m = fir->start_pos;
    int result = 0;

    for (int i = 0; i < input_len; i++) {
        fir->delay[fir->pos] = input[i];

        for (m = fir->start_pos; m < fir->interp; m += fir->decim) {
            float fir_sum = 0;
            int coeff_pos = 0;
            for (int n = fir->pos; n < fir->delay_size; n++) {
                fir_sum += fir->delay[n] * fir->coeffs[coeff_pos++ * fir->interp + m];
            }
            for (int n = 0; n < fir->pos; n++) {
                fir_sum += fir->delay[n] * fir->coeffs[coeff_pos++ * fir->interp + m];
            }
            output[result++] = fir_sum;
        }
        fir->start_pos = m - fir->interp;

        fir->pos--;
        if (fir->pos < 0) {
            fir->pos = fir->delay_size - 1;
        }
    }
    return result;
}
