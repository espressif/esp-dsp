/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "dsps_resampler.h"
#include <math.h>

static const float decim_avg_coeff = 0.9999;

esp_err_t dsps_resampler_mr_init(dsps_resample_mr_t *resampler, void *coeffs, int16_t length, int16_t interp, float samplerate_factor, int32_t fixed_point, int16_t shift)
{
    esp_err_t ret = ESP_OK;

    if (samplerate_factor < 1) {
        return ESP_ERR_DSP_INVALID_PARAM;
    }

    resampler->fixed_point = fixed_point;
    resampler->samplerate_factor = samplerate_factor;
    float decimation = (float)interp / samplerate_factor;
    resampler->decim_f = floor(decimation);
    resampler->decim_c = ceil(decimation);
    resampler->active_decim = resampler->decim_c;
    resampler->decim_avg_in = 1;
    resampler->decim_avg_out = resampler->samplerate_factor;

    if (fixed_point == 0) {
        resampler->dsps_firmr = (int32_t (*)(void *, void *, void *, int32_t))dsps_firmr_f32;
        resampler->filter = (void *)malloc(sizeof(fir_f32_t));

        ret = dsps_firmr_init_f32((fir_f32_t *)resampler->filter, coeffs, NULL, length, interp, resampler->decim_c, 0);
    } else {
        resampler->dsps_firmr = (int32_t (*)(void *, void *, void *, int32_t))dsps_firmr_s16;
        resampler->filter = (void *)malloc(sizeof(fir_s16_t));
        ret = dsps_firmr_init_s16((fir_s16_t *)resampler->filter, coeffs, NULL, length, interp, resampler->decim_c, 0, shift);
    }

    if (ret != ESP_OK) {
        return ret;
    }

    return ret;
}

int32_t dsps_resampler_mr_exec(dsps_resample_mr_t *resampler, void *input, void *output, int32_t length, int32_t length_correction)
{
    int32_t result = 0;

    if (resampler->fixed_point == 0) {
        ((fir_f32_t *)resampler->filter)->decim = resampler->active_decim;
    } else {
        ((fir_s16_t *)resampler->filter)->decim = resampler->active_decim;
    }

    result = resampler->dsps_firmr(resampler->filter, input, output, length);

    resampler->decim_avg_in = resampler->decim_avg_in * decim_avg_coeff + (float)length * (1 - decim_avg_coeff);
    resampler->decim_avg_out = resampler->decim_avg_out * decim_avg_coeff + (float)(result - length_correction) * (1 - decim_avg_coeff);

    if ((resampler->decim_avg_in * resampler->samplerate_factor) > (resampler->decim_avg_out)) {
        resampler->active_decim = resampler->decim_f;
    } else {
        resampler->active_decim = resampler->decim_c;
    }
    return result;
}

void dsps_resampler_mr_free(dsps_resample_mr_t *resampler)
{
    if (resampler->fixed_point == 0) {
        dsps_fir_f32_free((fir_f32_t *)(resampler->filter));
    } else {
        dsps_fird_s16_aexx_free((fir_s16_t *)(resampler->filter));
    }
    free(resampler->filter);
}
