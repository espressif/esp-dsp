/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "dsps_fir.h"
#include "dsp_common.h"
#include "dsp_tests.h"

int32_t dsps_firmr_s16_ansi(fir_s16_t *fir, const int16_t *input, int16_t *output, int32_t input_len)
{
    int32_t result = 0;
    long long rounding = 0;
    const int32_t final_shift = fir->shift - 15;
    rounding = (long long)(fir->rounding_val);

    if (fir->shift >= 0) {
        rounding = (rounding >> fir->shift) & 0xFFFFFFFFFF;         // 40-bit mask
    } else {
        rounding = (rounding << (-fir->shift)) & 0xFFFFFFFFFF;      // 40-bit mask
    }

    int32_t m = fir->start_pos;

    for (int i = 0; i < input_len; i++) {
        fir->delay[fir->pos] = input[i];

        for (m = fir->start_pos; m < fir->interp; m += fir->decim) {
            long long acc = rounding;
            int coeff_pos = 0;
            for (int n = fir->pos; n < fir->delay_size; n++) {
                acc += (int32_t)fir->delay[n] * (int32_t)fir->coeffs[coeff_pos++ * fir->interp + m];
            }
            for (int n = 0; n < fir->pos; n++) {
                acc += (int32_t)fir->delay[n] * (int32_t)fir->coeffs[coeff_pos++ * fir->interp + m];
            }

            if (final_shift > 0) {
                output[result++] = (int16_t)(acc << final_shift);
            } else {
                output[result++] = (int16_t)(acc >> (-final_shift));
            }
        }
        fir->start_pos = m - fir->interp;

        fir->pos--;
        if (fir->pos < 0) {
            fir->pos = fir->delay_size - 1;
        }
    }

    return result;
}
