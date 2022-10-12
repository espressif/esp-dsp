/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <malloc.h>
#include <stdint.h>
#include "unity.h"
#include "dsp_platform.h"
#include "esp_log.h"
#include "esp_dsp.h"

#include "dsps_tone_gen.h"
#include "dsps_d_gen.h"
#include "dsps_fir.h"
#include "dsp_tests.h"
#include "dsps_wind.h"
#include "dsps_view.h"
#include "dsps_fft2r.h"

#define COEFFS 256
#define N_IN_SAMPLES 256
#define DECIMATION 16
#define Q15_MAX INT16_MAX
#define LEAKAGE_BINS 10
#define FIR_BUFF_LEN 16

#define MAX_DEC 64
#define MAX_FIR_LEN 128

static const char *TAG = "dsps_fird_s16_ae32";

const static int32_t len = N_IN_SAMPLES;
const static int32_t N_FFT = (N_IN_SAMPLES / DECIMATION);
const static int16_t decim = DECIMATION;
const static int16_t fir_len = COEFFS;
const static int32_t fir_buffer = (N_IN_SAMPLES + FIR_BUFF_LEN);


TEST_CASE("dsps_fird_s16_ae32 functionality", "[dsps]")
{
    const int32_t max_len[2] = {2048, 2520};                        // 2520 can be divided by 3, 6, 9, 12, 15, 18 and 21
    const int16_t max_dec[2] = {32, 21};
    const int16_t min_dec[2] = {2, 3};

    int16_t *x = (int16_t *)memalign(16, max_len[1] * sizeof(int16_t));
    int16_t *y = (int16_t *)memalign(16, max_len[1] * sizeof(int16_t));
    int16_t *y_compare = (int16_t *)memalign(16, max_len[1] * sizeof(int16_t));

    int16_t *coeffs = (int16_t *)memalign(16, MAX_FIR_LEN * sizeof(int16_t));
    int16_t *delay = (int16_t *)memalign(16, MAX_FIR_LEN * sizeof(int16_t));
    int16_t *delay_compare = (int16_t *)memalign(16, MAX_FIR_LEN * sizeof(int16_t));

    const int16_t init_start_pos = 0;
    const int16_t shift = 0;
    const int16_t init_dec = decim;
    int32_t combinations = 0;
    int32_t loop_len = 0;

    fir_s16_t fir1;
    fir_s16_t fir2;

    for (int i = 0 ; i < MAX_FIR_LEN ; i++) {
        coeffs[i] = 0x100;
    }
    
    for (int i = 0 ; i < max_len[1] ; i++) {
        x[i] = 0x10;
    }

    dsps_fird_init_s16(&fir1, coeffs, delay, MAX_FIR_LEN, init_dec, init_start_pos, shift);
    dsps_fird_init_s16(&fir2, coeffs, delay_compare, MAX_FIR_LEN, init_dec, init_start_pos, shift);

    for(int variations = 0; variations < 2; variations++){
        // Test for different combinations of decimations, FIR filter lengths, and starting positions

        ESP_LOGI(TAG, ": %"PRId32" input samples, coefficients range from 2 to %"PRId16", decimation range from %"PRId16" to %"PRId16" and start positions within the coeffs range", 
             max_len[variations], (int16_t)MAX_FIR_LEN, min_dec[variations], max_dec[variations]);

        for (int dec = min_dec[variations]; dec <= max_dec[variations]; ((variations) ? (dec+=3) : (dec*=2))){

            fir1.decim = (int16_t)dec;
            fir2.decim = (int16_t)dec;
            loop_len = max_len[variations]/dec;

            for (int fir_length = 2; fir_length <= MAX_FIR_LEN; fir_length++){

                fir1.coeffs_len = (int16_t)fir_length;
                fir2.coeffs_len = (int16_t)fir_length;

                for (int start_pos = 0; start_pos < dec; start_pos++){

                    fir1.d_pos = (int16_t)start_pos;
                    fir2.d_pos = (int16_t)start_pos;

                    for(int j = 0; j < fir_length; j++){
                        fir1.delay[j] = 0;
                        fir2.delay[j] = 0;
                    }

                    fir1.pos = 0;
                    fir2.pos = 0;

                    const int32_t total1 = dsps_fird_s16(&fir1, x, y, loop_len);
                    const int32_t total2 = dsps_fird_s16_ansi(&fir2, x, y_compare, loop_len);

                    TEST_ASSERT_EQUAL(total1, total2);

                    for (int i = 0 ; i < total1 ; i++) {
                        TEST_ASSERT_EQUAL(y[i], y_compare[i]);
                    }
                    combinations++;
                }
            }
        }
    }

    ESP_LOGI(TAG, ": %"PRId32" total filter combinations\n", combinations);

    free(x);
    free(y);
    free(y_compare);
    free(coeffs);
    free(delay);
    free(delay_compare);
}


TEST_CASE("dsps_fird_s16_ae32 benchmark", "[dsps]")
{

    const int32_t local_dec = 2;
    const int32_t local_len = (len % 16) ? (4096) : (len);                          // length must be devisible by 16

    int16_t *x = (int16_t *)memalign(16, local_len * sizeof(int16_t));
    int16_t *y = (int16_t *)memalign(16, local_len * sizeof(int16_t));

    int16_t *coeffs = (int16_t *)memalign(16, fir_len * sizeof(int16_t));
    int16_t *delay = (int16_t *)memalign(16, fir_len * sizeof(int16_t));

    const int repeat_count = 100;    
    const int16_t start_pos = 0;
    const int16_t shift = 0;
    int32_t loop_len = 0;

    fir_s16_t fir1;
    for (int i = 0 ; i < fir_len ; i++) {
        coeffs[i] = 0;
    }
    coeffs[0] = 0x4000;

    for (int i = 0 ; i < local_len ; i++) {
        x[i] = 0x4000;
    }

    dsps_fird_init_s16(&fir1, coeffs, delay, fir_len, local_dec, start_pos, shift);

    // Test for decimations 2, 4, 8, 16
     for(int i = 0; i < 4; i++){

        loop_len = local_len / fir1.decim;
        const unsigned int start_b = xthal_get_ccount();
        for (int i = 0 ; i < repeat_count ; i++) {
            dsps_fird_s16(&fir1, x, y, loop_len);
        }
        const unsigned int end_b = xthal_get_ccount();

        const float total_b = end_b - start_b;
        const float cycles = total_b / (float)(repeat_count);
        const float cycles_per_sample = total_b / (float)(local_len * repeat_count);
        const float cycles_per_decim_tap = cycles_per_sample / (float)(fir1.coeffs_len * fir1.decim);

        ESP_LOGI(TAG, ": %.2f total cycles, %.2f cycles per sample, %.2f per decim tap, for %"PRId32" input samples, %"PRId16" coefficients and decimation %"PRId16"\n", 
                 cycles, cycles_per_sample, cycles_per_decim_tap, local_len, fir_len, fir1.decim);

        const float min_exec = ((local_len / fir1.decim) * fir1.coeffs_len) / 2;
        const float max_exec = min_exec * 10;
        TEST_ASSERT_EXEC_IN_RANGE(min_exec, max_exec, cycles);
        fir1.decim *= 2;
    }

    free(x);
    free(y);
    free(coeffs);
    free(delay);
}


TEST_CASE("dsps_fird_s16_ae32 noise_snr", "[dsps]")
{

    // In the SNR-noise test we are generating a sine wave signal, filtering the signal using a fixed point FIRD filter
    // and do the FFT of the filtered signal. Afterward, a noise and SNR calculated from the FFT spectrum    

    // FIR Coeffs
    int16_t *s_coeffs = (int16_t *)memalign(16, fir_len * sizeof(int16_t));         // fixed point coefficients
    int16_t *delay_line = (int16_t *)memalign(16, fir_len * sizeof(int16_t));       // fixed point delay line
    float *f_coeffs = (float *)memalign(16, fir_len * sizeof(float));               // floating point coefficients

    // Coefficients windowing
    dsps_wind_hann_f32(f_coeffs, fir_len);
    const float fir_order = (float)fir_len - 1;
    const float ft = 0.25;                                                          // Transition frequency
    for(int i = 0; i < fir_len; i++){
        f_coeffs[i] *= sinf((2 * M_PI * ft * (i - fir_order / 2))) / (M_PI * (i - fir_order / 2));
    }

    // FIR coefficients conversion to q15
    for(int i = 0; i < fir_len; i++){
        s_coeffs[i] = f_coeffs[i] * Q15_MAX; 
    }

    free(f_coeffs);

    // Signal generation
    const float amplitude = 0.9;
    const float frequency = 0.05;
    const float phase = 0;
    float *f_in_signal = (float *)memalign(16, fir_buffer * sizeof(float));
    dsps_tone_gen_f32(f_in_signal, fir_buffer, amplitude, frequency, phase);

    // Input signal conversion to q15
    int16_t *fir_x = (int16_t *)memalign(16, fir_buffer * sizeof(int16_t));
    int16_t *fir_y = (int16_t *)memalign(16, fir_buffer * sizeof(int16_t));
    for(int i = 0; i < fir_buffer; i++){
        fir_x[i] = f_in_signal[i] * (int16_t)Q15_MAX;
    }  

    free(f_in_signal);

    // FIR
    const int16_t start_pos = 0;
    const int16_t shift = 0;
    const int32_t loop_len = (int32_t)(fir_buffer / decim);                         // loop_len result must be without remainder
    fir_s16_t fir1;
    dsps_fird_init_s16(&fir1, s_coeffs, delay_line, fir_len, decim, start_pos, shift);
    dsps_fird_s16(&fir1, fir_x, fir_y, loop_len);

    free(delay_line);
    free(s_coeffs);
    free(fir_x);

    // FIR Output conversion to float
    const unsigned int ignored_fir_samples = (FIR_BUFF_LEN / 2) - 1;
    float *fir_output = (float*)memalign(16, len * sizeof(float));
    for(int i = 0; i < N_FFT; i++){
        fir_output[i] = (float)(fir_y[ignored_fir_samples + i] / (float)Q15_MAX);
    }

    free(fir_y);

    // Signal windowing
    float *window = (float *)memalign(16, N_FFT * sizeof(float));
    dsps_wind_blackman_f32(window, N_FFT);

    // Prepare FFT input, real and imaginary part
    const int32_t fft_data_len = (N_IN_SAMPLES/DECIMATION) * 2;
    float *fft_data = (float *)memalign(16, fft_data_len * sizeof(float));
    for (int i = 0 ; i < N_FFT ; i++) {
        fft_data[i * 2 + 0] = fir_output[i] * window[i];
        fft_data[i * 2 + 1] = 0;
    }
    free(fir_output);
    free(window);

    // Initialize FFT
    esp_err_t ret = dsps_fft2r_init_fc32(NULL, N_FFT*2);
    if (ret  != ESP_OK)
    {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return;
    }

    // Do the FFT
    dsps_fft2r_fc32(fft_data, N_FFT);
    dsps_bit_rev_fc32(fft_data, N_FFT);
    dsps_cplx2reC_fc32(fft_data, N_FFT);

    // Convert the FFT spectrum from amplitude to watts, find the max value and its position
    float max_val = -1000000;
    int max_pos = 0;
    for (int i = 0 ; i < N_FFT/2 ; i++) {
        fft_data[i] = (fft_data[i * 2 + 0] * fft_data[i * 2 + 0] + fft_data[i * 2 + 1] * fft_data[i * 2 + 1])/(N_FFT * 3);
        if(fft_data[i] > max_val){
            max_val = fft_data[i];
            max_pos = i;
        }
    }

    // Calculate the power of the signal and noise of the spectrum and convert the spectrum to dB
    float signal_pow = 0, noise_pow = 0;
    for (int i = 0 ; i < N_FFT/2 ; i++) {
        if ((i >= max_pos - LEAKAGE_BINS) && (i <= max_pos + LEAKAGE_BINS))
            signal_pow += fft_data[i];
        else
            noise_pow += fft_data[i];

        fft_data[i] = 10 * log10f(0.0000000000001 + fft_data[i]);
    }    

    // Convert the signal power and noise power from watts to dB and calculate SNR
    const float snr = 10 * log10f(signal_pow/noise_pow);
    noise_pow = 10 * log10f(noise_pow);
    signal_pow = 10 * log10f(signal_pow);

    ESP_LOGI(TAG, "\nSignal Power: \t%f\nNoise Power: \t%f\nSNR: \t\t%f", signal_pow, noise_pow, snr);
    dsps_view(fft_data, N_FFT/2, 128, 16,  -140, 40, '|');
    free(fft_data);

    const float min_exec_snr = 50;
    const float max_exec_snr = 120;
    TEST_ASSERT_EXEC_IN_RANGE(min_exec_snr, max_exec_snr, snr);

}