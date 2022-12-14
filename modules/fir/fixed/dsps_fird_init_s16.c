/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include "dsps_fir.h"

esp_err_t dsps_fird_init_s16(fir_s16_t *fir, int16_t *coeffs, int16_t *delay, int16_t coeffs_len, int16_t decim, int16_t start_pos, int16_t shift)
{
    fir->coeffs = coeffs;
    fir->delay = delay;
    fir->coeffs_len = coeffs_len;
    fir->pos = 0;
    fir->decim = decim;
    fir->d_pos = start_pos;
    fir->shift = shift;
    if (fir->d_pos >= fir->decim) {
        return ESP_ERR_DSP_PARAM_OUTOFRANGE;
    }

    if ((fir->shift > 40) || (fir->shift < -40)) {
        return ESP_ERR_DSP_PARAM_OUTOFRANGE;
    }

    for (int i = 0 ; i < fir->coeffs_len; i++) {
        fir->delay[i] = 0;
    }
    return ESP_OK;
}
