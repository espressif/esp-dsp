/*
 * SPDX-FileCopyrightText: 2018-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _dsps_resampl_H_
#define _dsps_resampl_H_

#include "dsp_err.h"
#include "dsps_fir.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Data struct of f32 multi-rate resampler
 *
 * This structure is used by a resampler internally. A user should access this structure only in case of
 * extensions for the DSP Library.
 * To initialize the resampler, use the dsps_resampler_mr_init() function.
 * To execute the resampler, use the dsps_resampler_mr_exec() function.
 * To free the resampler, use the dsps_resampler_mr_free() function.
 */
typedef struct dsps_resample_mr_s {
    void *filter;               /*!< FIR filter structure*/
    float samplerate_factor;    /*!< Sample rate factor */
    int16_t decim_c;            /*!< Decimation factor for ceil */
    int16_t decim_f;            /*!< Decimation factor for floor */
    int16_t active_decim;       /*!< Active decimation factor */
    float   decim_avg_in;       /*!< Average decimation factor for input */
    float   decim_avg_out;      /*!< Average decimation factor for output */
    int32_t (*dsps_firmr)(void *fir, void *input, void *output, int32_t length); /*!< Pointer to FIR filter function */
    int32_t fixed_point;        /*!< Fixed point flag */
} dsps_resample_mr_t;

/**
 * @brief Data struct of f32 poly-phase resampler
 *
 * This structure is used by a poly-phase resampler internally. A user should access this structure only in case of
 * extensions for the DSP Library.
 * To initialize the resampler, use the dsps_resampler_ph_init() function.
 * To execute the resampler, use the dsps_resampler_ph_exec() function.
 *
 * It is possible to correct the sample rate by adjust the phase parameter "on the fly".
 */
typedef struct dsps_resample_ph_s {
    float phase;
    float step;
    float delay[4];
    int   delay_pos;
} dsps_resample_ph_t;

/**
 * @brief Initialize the multi-rate resampler
 *
 * @param resampler Pointer to the resampler structure
 * @param coeffs Pointer to the filter coefficients (float or int16_t for fixed point)
 * @param length Length of the filter coefficients
 * @param interp Interpolation factor for the filter
 * @param samplerate_factor Sample rate factor
 * @param fixed_point Fixed point flag, 0 for float, 1 for fixed point
 * @param shift Shift value for fixed point
 *
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG if the parameters are invalid
 */
esp_err_t dsps_resampler_mr_init(dsps_resample_mr_t *resampler, void *coeffs, int16_t length, int16_t interp, float samplerate_factor, int32_t fixed_point, int16_t shift);

/**
 * @brief Execute the multi-rate resampler
 *
 * This function executes the multi-rate resampler. The input and output buffers can be the same.
 * The function based on multi-rate FIR filter.
 * The decimation factor is updated for each execution. The current decimation factor calculated
 * as division of average input and average output sample rate.
 * To correct the output sample rate, the length_correction parameter is used. To increase
 * the output sample rate, the length_correction parameter should be positive. To decrease
 * the output sample rate, the length_correction parameter should be negative. This parameter
 * is used when the input and output sample rates are comes from different sources.
 *
 * @param resampler: Pointer to the resampler structure
 * @param input: Pointer to the input buffer
 * @param output: Pointer to the output buffer
 * @param length: Length of the input buffers
 * @param length_correction: Length correction for the current execution. Positive value
 *                           increases the output sample rate, negative value decreases
 *                           the output sample rate.
 *
 * @return Length of the output buffer
 */
int32_t dsps_resampler_mr_exec(dsps_resample_mr_t *resampler, void *input, void *output, int32_t length, int32_t length_correction);

/**
 * @brief Free the multi-rate resampler
 *
 * @param resampler Pointer to the resampler structure
 */
void dsps_resampler_mr_free(dsps_resample_mr_t *resampler);



/**
 * @brief Initialize the poly-phase resampler
 *
 * The poly-phase resampler is a implementation of the poly-phase Farrow filter
 * that use cubic interpolation with 4 coefficients.
 *
 * @param resampler Pointer to the resampler structure
 * @param samplerate_factor Sample rate factor
 *
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG if the parameters are invalid
 */
esp_err_t dsps_resampler_ph_init(dsps_resample_ph_t *resampler, float samplerate_factor);

/**
 * @brief Execute the poly-phase resampler
 *
 * @param resampler Pointer to the resampler structure
 * @param input Pointer to the input buffer
 * @param output Pointer to the output buffer
 * @param length Length of the input buffer
 *
 * @return Length of the output buffer
 */

int32_t dsps_resampler_ph_exec(dsps_resample_ph_t *resampler, float *input, float *output, int32_t length);

#ifdef __cplusplus
}
#endif
#endif //_dsps_resampl_H_
