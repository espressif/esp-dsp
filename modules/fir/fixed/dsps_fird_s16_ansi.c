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

#include "dsps_fir.h"

int32_t dsps_fird_s16_ansi(fir_s16_t *fir, const int16_t *input, int16_t *output, int32_t len)
{
    int32_t result = 0;
    for (int i = 0; i < len ; i++) {
        fir->delay[fir->pos++] = input[i];
        if (fir->pos >= fir->N) {
            fir->pos = 0;
        }
        fir->d_pos++;
        if (fir->d_pos >= fir->decim) {
            fir->d_pos = 0;

            long long acc = 0x7fff >> fir->shift;
            int16_t coeff_pos = fir->N - 1;

            for (int n = fir->pos; n < fir->N ; n++) {
                acc += (int32_t)fir->coeffs[coeff_pos--] * (int32_t)fir->delay[n];
            }
            for (int n = 0; n < fir->pos ; n++) {
                acc += (int32_t)fir->coeffs[coeff_pos--] * (int32_t)fir->delay[n];
            }

            int32_t final_shift = fir->shift - 15;
            if (final_shift > 0) {
                output[result++] = (int16_t)(acc << final_shift);
            } else {
                output[result++] = (int16_t)(acc >> (-final_shift));
            }
        }
    }
    return result;
}