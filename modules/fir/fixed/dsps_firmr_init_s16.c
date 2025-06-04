/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "dsps_fir.h"
#include "malloc.h"
#include <string.h>
#include "dsp_tests.h"
#include "dsp_common.h"

#define ROUNDING_VALUE  0x7fff

esp_err_t dsps_firmr_init_s16(fir_s16_t *fir, int16_t *coeffs, int16_t *delay, int16_t coeffs_len, int16_t interp, int16_t decim, int16_t start_pos, int16_t shift)
{
    fir->coeffs = coeffs;
    fir->delay = delay;


    fir->coeffs_len = coeffs_len;
    fir->pos = 0;
    fir->decim = decim;
    fir->d_pos = start_pos;
    fir->shift = shift;
    fir->rounding_val = (int16_t)(ROUNDING_VALUE);
    fir->free_status = 0;

    if (delay == NULL) {
#ifdef CONFIG_IDF_TARGET_ESP32S3
        fir->delay = (int16_t *)memalign(16, (fir->delay_size + 4) * sizeof(int16_t));
#else
        fir->delay = (int16_t *)malloc((fir->delay_size + 4) * sizeof(int16_t));
#endif // CONFIG_IDF_TARGET_ESP32S3
        fir->free_status = 1;
    } else {
        fir->free_status = 0;
    }


    fir->interp = interp;
    fir->interp_pos = 0;
    fir->start_pos = start_pos;
    fir->delay_size = coeffs_len / interp;


    if (fir->coeffs_len < 2) {                                          // number of coeffcients must be higer than 1
        return ESP_ERR_DSP_INVALID_LENGTH;
    }

    if ((fir->shift > 40) || (fir->shift < -40)) {                      // shift amount must be within a range from -40 to 40
        return ESP_ERR_DSP_PARAM_OUTOFRANGE;
    }

    if (fir->d_pos >= fir->decim) {                                     // start position must be lower than decimation
        return ESP_ERR_DSP_PARAM_OUTOFRANGE;
    }

    for (int i = 0; i < fir->delay_size; i++) {                                  // Initialize the delay line to zero
        fir->delay[i] = 0;
    }

    return ESP_OK;
}
