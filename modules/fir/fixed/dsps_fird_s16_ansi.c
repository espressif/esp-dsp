/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "dsps_fir.h"
#include "esp_dsp.h"


int32_t dsps_fird_s16_ansi(fir_s16_t *fir, const int16_t *input, int16_t *output, int32_t len)
{
    int32_t result = 0;
    int32_t input_pos = 0;
    long long rounding_val = 0;
    const int32_t final_shift = fir->shift -15;
    
    if(fir->shift >= 0){
        rounding_val = (long long)(0x7fff >> fir->shift);
    }
    else{
        rounding_val = (long long)(0x7fff << (-fir->shift));
    }

    // len is already a length of the *output array, calculated as (length of the input array / decimation)
    for (int i = 0; i < len; i++) {

        for (int j = 0; j < fir->decim - fir->d_pos; j++) {

            if (fir->pos >= fir->coeffs_len) {
                fir->pos = 0;
            }
            fir->delay[fir->pos++] = input[input_pos++];
        }
        fir->d_pos = 0;

        long long acc = rounding_val;
        int16_t coeff_pos = fir->coeffs_len - 1;

        for (int n = fir->pos; n < fir->coeffs_len ; n++) {
            acc += (int32_t)fir->coeffs[coeff_pos--] * (int32_t)fir->delay[n];
        }
        for (int n = 0; n < fir->pos ; n++) {
            acc += (int32_t)fir->coeffs[coeff_pos--] * (int32_t)fir->delay[n];
        }

        if (final_shift > 0) {
            output[result++] = (int16_t)(acc << final_shift);
        } else {
            output[result++] = (int16_t)(acc >> (-final_shift));
        }
        
    }
    return result;
}
