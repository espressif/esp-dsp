/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "dsps_fir.h"
#include "dsp_common.h"
#include <malloc.h>
#include "dsp_tests.h"

esp_err_t dsps_firmr_init_f32(fir_f32_t *fir, float *coeffs, float *delay, int length, int interp, int decim, int start_pos)
{
    fir->coeffs = coeffs;
    fir->delay = delay;
    fir->N = length;
    fir->pos = 0;
    fir->decim = decim;
    fir->use_delay = 0;
    fir->interp = interp;
    fir->interp_pos = 0;
    fir->start_pos = start_pos;
    fir->delay_size = length / interp;

    if (delay == NULL) {
#ifdef CONFIG_IDF_TARGET_ESP32S3
        fir->delay = (float *)memalign(16, (fir->delay_size + 4) * sizeof(float));
#else
        fir->delay = (float *)malloc((fir->delay_size + 4) * sizeof(float));
#endif // CONFIG_IDF_TARGET_ESP32S3
        fir->use_delay = 1;
    } else {
        fir->use_delay = 0;
    }

    if (decim == 0) {
        return ESP_ERR_DSP_INVALID_PARAM;
    }
    if (interp == 0) {
        return ESP_ERR_DSP_INVALID_PARAM;
    }
    if (length % interp != 0) {
        return ESP_ERR_DSP_INVALID_LENGTH;
    }
    if (start_pos < 0 || start_pos >= decim) {
        return ESP_ERR_DSP_INVALID_PARAM;
    }

#ifdef CONFIG_IDF_TARGET_ESP32S3
    // The delay array should be aligned to 16
    if (((uint32_t)delay) & 0x0f) {
        return ESP_ERR_DSP_ARRAY_NOT_ALIGNED;
    }
#endif // CONFIG_IDF_TARGET_ESP32S3

    for (int i = 0 ; i < fir->delay_size; i++) {
        fir->delay[i] = 0;
    }
    return ESP_OK;
}
