/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "dsps_resampler.h"
#include <math.h>

esp_err_t dsps_resampler_ph_init(dsps_resample_ph_t *resampler, float samplerate_factor)
{
    esp_err_t ret = ESP_OK;

    if (samplerate_factor < 1) {
        return ESP_ERR_DSP_INVALID_PARAM;
    }

    for (int i = 0; i < 4; i++) {
        resampler->delay[i] = 0;
    }
    resampler->delay_pos = 0;

    resampler->step = 1.0f / samplerate_factor;
    resampler->phase = 0;
    return ret;
}

int32_t dsps_resampler_ph_exec(dsps_resample_ph_t *resampler, float *input, float *output, int32_t length)
{
    int32_t result = 0;
    int input_pos = 0;
    float in_x[4];

    int in_pos = 0;
    for (int pos = resampler->delay_pos; pos < 4; pos++) {
        in_x[in_pos++] = resampler->delay[pos];
    }
    for (int pos = 0; pos < resampler->delay_pos; pos++) {
        in_x[in_pos++] = resampler->delay[pos];
    }

    while (input_pos < length) {

        float c0 = in_x[1];
        float c1 = 0.5 * (in_x[2] - in_x[0]);
        float c2 = in_x[0] - 2.5 * in_x[1] + 2 * in_x[2] - 0.5 * in_x[3];
        float c3 = 0.5 * (in_x[3] - in_x[0]) + 1.5 * (in_x[1] - in_x[2]);

        output[result] = c0 + resampler->phase * (c1 + resampler->phase * (c2 + resampler->phase * c3));
        result++;
        resampler->phase += resampler->step;

        while (resampler->phase >= 1) {
            resampler->phase -= 1;
            resampler->delay[resampler->delay_pos] = input[input_pos];
            resampler->delay_pos++;
            if (resampler->delay_pos >= 4) {
                resampler->delay_pos = 0;
            }
            input_pos++;
            in_pos = 0;
            for (int pos = resampler->delay_pos; pos < 4; pos++) {
                in_x[in_pos++] = resampler->delay[pos];
            }
            for (int pos = 0; pos < resampler->delay_pos; pos++) {
                in_x[in_pos++] = resampler->delay[pos];
            }
        }
    }

    return result;
}
