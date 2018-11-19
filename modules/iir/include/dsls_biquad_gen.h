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

#ifndef _dsls_biquad_gen_H_
#define _dsls_biquad_gen_H_

#include "dsl_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

// coeffs[0] = b0;
// coeffs[1] = b1;
// coeffs[2] = b2;
// coeffs[3] = a1;
// coeffs[4] = a2;
// a0 - always == 1

/**
 * @function dsls_biquad_gen_lpf_32f
 * Coefficients for low pass 2nd order IIR filter (bi-quad)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param coeffs: result coefficients. b0,b1,b2,a1,a2, a0 expected as 1
 * @param f: filter cut off frequency in range of 0..0.5 (normalized to sample frequency)
 * @param qFactor: Q factor of filter
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_gen_lpf_32f(float *coeffs, float f, float qFactor);

/**
 * @function dsls_biquad_gen_hpf_32f
 * Coefficients for high pass 2nd order IIR filter (bi-quad)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param coeffs: result coefficients. b0,b1,b2,a1,a2, a0 expected as 1
 * @param f: filter cut off frequency in range of 0..0.5 (normalized to sample frequency)
 * @param qFactor: Q factor of filter
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_gen_hpf_32f(float *coeffs, float f, float qFactor);

/**
 * @function dsls_biquad_gen_bpf_32f
 * Coefficients for band pass 2nd order IIR filter (bi-quad)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param coeffs: result coefficients. b0,b1,b2,a1,a2, a0 expected as 1
 * @param f: filter center frequency in range of 0..0.5 (normalized to sample frequency)
 * @param qFactor: Q factor of filter
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_gen_bpf_32f(float *coeffs, float f, float qFactor);

/**
 * @function dsls_biquad_gen_bpf0db_32f
 * Coefficients for band pass 2nd order IIR filter (bi-quad) with 0 dB gain in passband
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param coeffs: result coefficients. b0,b1,b2,a1,a2, a0 expected as 1
 * @param f: filter center frequency in range of 0..0.5 (normalized to sample frequency)
 * @param qFactor: Q factor of filter
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_gen_bpf0db_32f(float *coeffs, float f, float qFactor);

/**
 * @function dsls_biquad_gen_notch_32f
 * Coefficients for notch 2nd order IIR filter (bi-quad)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param coeffs: result coefficients. b0,b1,b2,a1,a2, a0 expected as 1
 * @param f: filter notch frequency in range of 0..0.5 (normalized to sample frequency)
 * @param gain: gain in stopband in dB
 * @param qFactor: Q factor of filter
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_gen_notch_32f(float *coeffs, float f, float gain, float qFactor);
/**
 * @function dsls_biquad_gen_allpass360_32f
 * Coefficients for all pass 2nd order IIR filter (bi-quad) with 360 degree phase shift
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param coeffs: result coefficients. b0,b1,b2,a1,a2, a0 expected as 1
 * @param f: filter notch frequency in range of 0..0.5 (normalized to sample frequency)
 * @param qFactor: Q factor of filter
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_gen_allpass360_32f(float *coeffs, float f, float qFactor);
/**
 * @function dsls_biquad_gen_allpass180_32f
 * Coefficients for all pass 2nd order IIR filter (bi-quad) with 180 degree phase shift
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param coeffs: result coefficients. b0,b1,b2,a1,a2, a0 expected as 1
 * @param f: filter notch frequency in range of 0..0.5 (normalized to sample frequency)
 * @param qFactor: Q factor of filter
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_gen_allpass180_32f(float *coeffs, float f, float qFactor);
/**
 * @function dsls_biquad_gen_peakingEQ_32f
 * Coefficients for peak 2nd order IIR filter (bi-quad)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param coeffs: result coefficients. b0,b1,b2,a1,a2, a0 expected as 1
 * @param f: filter notch frequency in range of 0..0.5 (normalized to sample frequency)
 * @param qFactor: Q factor of filter
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_gen_peakingEQ_32f(float *coeffs, float f, float qFactor);
/**
 * @function dsls_biquad_gen_lowShelf_32f
 * Coefficients for low pass Shelf 2nd order IIR filter (bi-quad)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param coeffs: result coefficients. b0,b1,b2,a1,a2, a0 expected as 1
 * @param f: filter notch frequency in range of 0..0.5 (normalized to sample frequency)
 * @param gain: gain in stopband in dB
 * @param qFactor: Q factor of filter
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_gen_lowShelf_32f(float *coeffs, float f, float gain, float qFactor);
/**
 * @function dsls_biquad_gen_highShelf_32f
 * Coefficients for high pass Shelf 2nd order IIR filter (bi-quad)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param coeffs: result coefficients. b0,b1,b2,a1,a2, a0 expected as 1
 * @param f: filter notch frequency in range of 0..0.5 (normalized to sample frequency)
 * @param gain: gain in stopband in dB
 * @param qFactor: Q factor of filter
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsls_biquad_gen_highShelf_32f(float *coeffs, float f, float gain, float qFactor);

#ifdef __cplusplus
}
#endif

#endif // _dsls_biquad_gen_H_