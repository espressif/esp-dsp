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

#include <string.h>
#include "unity.h"
#include "dsp_platform.h"
#include "esp_log.h"
#include <malloc.h>
#include "esp_dsp.h"

#include "dsps_tone_gen.h"
#include "dsps_d_gen.h"
#include "dsps_fir.h"
#include "dsp_tests.h"

#include "dsps_wind.h"
#include "dsps_view.h"
#include "dsps_fft2r.h"

# define COEFFS 32
# define N_IN_SAMPLES 1024
# define DECIMATION 2
# define Q15_MAX 32768
# define LEAKAGE_BINS 10
# define FIR_BUFF_LEN 16

static const char *TAG = "dsps_fird_s16_ae32";

const static int32_t len = N_IN_SAMPLES;
const static int32_t N_FFT = N_IN_SAMPLES / DECIMATION;
const static int16_t decim = DECIMATION;
const static int16_t fir_len = COEFFS;
const static int32_t fir_buffer = (N_IN_SAMPLES + FIR_BUFF_LEN);


TEST_CASE("dsps_fird_s16_ae32 functionality", "[dsps]")
{
    // In the test we generate filter with cutt off frequency 0.1
    // and then filtering 0.1 and 0.3 frequencis.
    // Result must be better then 24 dB

    int16_t *x = (int16_t *)malloc(len * sizeof(int16_t));
    int16_t *y = (int16_t *)malloc(len * sizeof(int16_t));
    int16_t *y_compare = (int16_t *)malloc(len * sizeof(int16_t));

    int16_t *coeffs = (int16_t *)malloc(fir_len * sizeof(int16_t));
    int16_t *delay = (int16_t *)malloc(fir_len * sizeof(int16_t));
    int16_t *delay_compare = (int16_t *)malloc(fir_len * sizeof(int16_t));

    const int16_t start_pos = 0;
    const int16_t shift = 0;
    const int16_t dec = decim;

    fir_s16_t fir1;
    fir_s16_t fir2;

    for (int i = 0 ; i < fir_len ; i++) {
        coeffs[i] = 0;
    }
    coeffs[0] = 0x4000;

    for (int i = 0 ; i < len ; i++) {
        x[i] = 0x4000;
    }

    dsps_fird_init_s16(&fir1, coeffs, delay, fir_len, dec, start_pos, shift);
    dsps_fird_init_s16(&fir2, coeffs, delay_compare, fir_len, dec, start_pos, shift);
    int32_t total1 = dsps_fird_s16_ae32(&fir1, x, y, len);
    int32_t total2 = dsps_fird_s16_ansi(&fir2, x, y_compare, len);
    total1 += dsps_fird_s16_ae32(&fir1, x, y, len);
    total2 += dsps_fird_s16_ansi(&fir2, x, y_compare, len);
    total1 += dsps_fird_s16_ae32(&fir1, x, y, len);
    total2 += dsps_fird_s16_ansi(&fir2, x, y_compare, len);
    ESP_LOGI(TAG, "Total result = %"PRIi32", expected %"PRIi32" from %"PRIi32"\n", total1, total2, len);
    TEST_ASSERT_EQUAL(total1, total2);
    for (int i=0 ; i< total1 ; i++)
    {
        ESP_LOGD(TAG, "data[%d] = %d\n", i, y[i]);
    }
    for (int i = 0 ; i < total1 ; i++) {
        if (y[i] != y_compare[i]) {
            TEST_ASSERT_EQUAL(y[i], y_compare[i]);
        }
    }

}


TEST_CASE("dsps_fird_s16_ae32 benchmark", "[dsps]")
{

    int16_t *x = (int16_t *)malloc(len * sizeof(int16_t));
    int16_t *y = (int16_t *)malloc(len * sizeof(int16_t));

    int16_t *coeffs = (int16_t *)malloc(fir_len * sizeof(int16_t));
    int16_t *delay = (int16_t *)malloc(fir_len * sizeof(int16_t));

    const int16_t repeat_count = 1;
    const int16_t start_pos = 0;
    const int16_t shift = 0;
    int16_t dec = decim;

    fir_s16_t fir1;
    for (int i = 0 ; i < fir_len ; i++) {
        coeffs[i] = 0;
    }
    coeffs[0] = 0x4000;

    for (int i = 0 ; i < len ; i++) {
        x[i] = 0x4000;
    }

    for(int i = 0; i < 4; i++){
        dsps_fird_init_s16(&fir1, coeffs, delay, fir_len, dec, start_pos, shift);

        const unsigned int start_b = xthal_get_ccount();
        for (int i = 0 ; i < repeat_count ; i++) {
            dsps_fird_s16_ae32(&fir1, x, y, len);
        }
        const unsigned int end_b = xthal_get_ccount();

        const float total_b = end_b - start_b;
        float cycles = total_b / (float)(len * repeat_count);

        ESP_LOGI(TAG, "dsps_fir_s16_ae32 - %f per sample for for %d coefficients, decimation %d, %f per decim tap \n", cycles, fir_len, dec, cycles / (float)fir_len * dec);
        const float min_exec = 3;
        const float max_exec = 300;
        TEST_ASSERT_EXEC_IN_RANGE(min_exec, max_exec, cycles);
        dec *= 2;
    }
}


TEST_CASE("dsps_fird_s16_ae32 noise_snr", "[dsps]")
{

    // FIR Coeffs
    int16_t *s_coeffs = (int16_t *)malloc(fir_len * sizeof(int16_t));       // fixed point coefficients
    int16_t *delay_line = (int16_t *)malloc(fir_len * sizeof(int16_t));     // fixed point delay line
    float *f_coeffs = (float *)malloc(fir_len * sizeof(float));             // floating point coeffciients

    // Coefficients windowing
    dsps_wind_hann_f32(f_coeffs, fir_len);
    const float fir_order = (float)fir_len - 1;
    const float ft = 0.25;                                                  // Transition frequency
    for(int i = 0; i < fir_len; i++){
        f_coeffs[i] *= sinf((2 * M_PI * ft * (i - fir_order / 2))) / (M_PI * (i - fir_order / 2));
    }

    // FIR coefficients conversion to q15
    for(int i = 0; i < fir_len; i++){
        s_coeffs[i] = f_coeffs[i] * (int16_t)Q15_MAX; 
    }

    free(f_coeffs);

    // Signal generation
    const float amplitude = 0.9;
    const float frequency = 0.05;
    const float phase = 0;
    float *f_in_signal = (float *)malloc(fir_buffer * sizeof(float));
    dsps_tone_gen_f32(f_in_signal, fir_buffer, amplitude, frequency, phase);

    // Input signal conversion to q15
    int16_t *fir_x = (int16_t *)malloc(fir_buffer * sizeof(int16_t));
    int16_t *fir_y = (int16_t *)malloc(fir_buffer * sizeof(int16_t));
    for(int i = 0; i < fir_buffer; i++){
        fir_x[i] = f_in_signal[i] * (int16_t)Q15_MAX;
    }  

    // FIR
    const int16_t start_pos = 0;
    const int16_t shift = 0;
    fir_s16_t fir1;
    dsps_fird_init_s16(&fir1, s_coeffs, delay_line, fir_len, decim, start_pos, shift);
    dsps_fird_s16_ae32(&fir1, fir_x, fir_y, fir_buffer);

    // FIR Output conversion to float
    const unsigned int ignored_fir_samples = (FIR_BUFF_LEN / 2) - 1;
    float *fir_output = (float*)malloc(len * sizeof(float));
    for(int i = 0; i < N_FFT; i++){
        fir_output[i] = (float)(fir_y[ignored_fir_samples + i] / (float)Q15_MAX);
    }

    free(fir_x);
    free(fir_y);
    free(delay_line);
    free(s_coeffs);

    // Signal windowing
    float *window = (float *)malloc(N_FFT * sizeof(float));
    dsps_wind_blackman_f32(window, N_FFT);

    // Prepare FFT input, real and imaginary part
    const int32_t fft_data_len = (N_IN_SAMPLES/DECIMATION) * 2;
    float *fft_data = (float *)malloc(fft_data_len * sizeof(float));
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

    
    float max = -1000000;
    int max_pos = 0;
    for (int i = 0 ; i < N_FFT/2 ; i++) {
        fft_data[i] = (fft_data[i * 2 + 0] * fft_data[i * 2 + 0] + fft_data[i * 2 + 1] * fft_data[i * 2 + 1])/(N_FFT * 3);
        if(fft_data[i] > max){
            max = fft_data[i];
            max_pos = i;
        }
    }

    float p_signal = 0, p_noise = 0, snr;
    for (int i = 0 ; i < N_FFT/2 ; i++) {
        if ((i >= max_pos - LEAKAGE_BINS) && (i <= max_pos + LEAKAGE_BINS))
            p_signal += fft_data[i] * fft_data[i];
        else
            p_noise += fft_data[i] * fft_data[i];

        fft_data[i] = 10 * log10f(fft_data[i]);
        ESP_LOGI(TAG, "%f", fft_data[i]);
        ESP_LOGD(TAG, "FFT Data[%i] =%8.4f dB", i, fft_data[i]);
    }    

    p_signal = sqrtf(p_signal);
    p_noise = sqrtf(p_noise);
    snr = 10 * log10f(p_signal/p_noise);
    p_noise = 10 * log10f(p_noise);
    p_signal = 10 * log10f(p_signal);

    ESP_LOGI(TAG, "\nSignal Power: %f\nNoise Power: %f\nSNR: %f", p_signal, p_noise, snr);
    dsps_view(fft_data, N_FFT/2, 128, 16,  -140, 40, '|');

    const float min_exec_snr = 50;
    const float max_exec_snr = 120;
    TEST_ASSERT_EXEC_IN_RANGE(min_exec_snr, max_exec_snr, snr);

}