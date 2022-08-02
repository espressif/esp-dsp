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
#include "esp_err.h"
#include "esp_dsp.h"

#include "dsps_tone_gen.h"
#include "dsps_d_gen.h"
#include "dsps_fir.h"
#include "dsp_tests.h"
#include "dsps_wind.h"
#include "dsps_view.h"
#include "dsps_fft2r.h"

# define N_IN_SAMPLES 1024
# define DECIMATION 2
# define Q15_MAX 32768
# define LEAKAGE_BINS 10
# define FIR_BUFF_LEN 16
# define FIR_BUFF (N_IN_SAMPLES + FIR_BUFF_LEN)

const static int N_IN = N_IN_SAMPLES;
const static int N_FFT = N_IN_SAMPLES / DECIMATION;
const static int dec = DECIMATION;

static const char *TAG = "dsps_fird_s16_ansi";

static int16_t x[1024];
static int16_t y[1024];

static float fft_data[(N_IN_SAMPLES/DECIMATION)*2];
static float f_in_signal[FIR_BUFF];
static int16_t fir_x[FIR_BUFF], fir_y[FIR_BUFF];


TEST_CASE("dsps_fird_s16_ansi functionality", "[dsps]")
{
    // In the test we generate filter with cutt off frequency 0.1
    // and then filtering 0.1 and 0.3 frequencis.
    // Result must be better then 24 dB
    int16_t coeffs[32];
    int16_t delay[32];

    int32_t len = sizeof(x) / sizeof(int16_t);
    int16_t fir_len = sizeof(coeffs) / sizeof(int16_t);
    int16_t decim = 4;
    int16_t shift = 0;
    int16_t start_pos = 0;

    fir_s16_t fir1;
    for (int i = 0 ; i < fir_len ; i++) {
        coeffs[i] = 0; 
    }
    coeffs[0] = 0x4000;

    for (int i = 0 ; i < len ; i++) {
        x[i] = 0x4000;
    }

    dsps_fird_init_s16(&fir1, coeffs, delay, fir_len, decim, start_pos, shift);
    int32_t total = dsps_fird_s16_ansi(&fir1, x, y, len);
    ESP_LOGI(TAG, "Total result = %"PRIx32" from %"PRIx32"\n", total, len);
    TEST_ASSERT_EQUAL(total, len / decim);
    for (int i=0 ; i< total ; i++)
    {
        ESP_LOGD(TAG, "data[%i] = %d\n", i, y[i]);
    }
    for (int i = 0 ; i < total ; i++) {
        if (y[i] != (0x2000)) {
            TEST_ASSERT_EQUAL(y[i], (0x2000));
        }
    }
}


TEST_CASE("dsps_fird_s16_ansi benchmark", "[dsps]")
{

    int16_t coeffs[64];
    int16_t delay[64];
    int32_t len = sizeof(x) / sizeof(int16_t);
    int16_t fir_len = sizeof(coeffs) / sizeof(int16_t);
    int16_t repeat_count = 4;
    int16_t decim = 1;
    int16_t start_pos = 0;
    int16_t shift = 0;

    fir_s16_t fir1;
    for (int i = 0 ; i < fir_len ; i++) {
        coeffs[i] = i;
    }

    for (int i = 0 ; i < len ; i++) {
        x[i] = 0;
    }
    x[0] = 1;

    // Decimation 1, 2, 4, 8
    for (int i = 0 ; i < 4 ; i++){
        dsps_fird_init_s16(&fir1, coeffs, delay, fir_len, decim, start_pos, shift);

        unsigned int start_b = xthal_get_ccount();
        for (int i = 0 ; i < repeat_count ; i++) {
            dsps_fird_s16_ansi(&fir1, x, y, len);
        }
        unsigned int end_b = xthal_get_ccount();

        float total_b = end_b - start_b;
        float cycles = total_b / (len * repeat_count);

        ESP_LOGI(TAG, "dsps_fir_s16_ansi - %f per sample for for %d coefficients, %f per decim tap \n", cycles, fir_len, cycles / (float)fir_len * decim);
        float min_exec = 10;
        float max_exec = 1500;
        TEST_ASSERT_EXEC_IN_RANGE(min_exec, max_exec, cycles);
        decim *= 2;
    }
}


TEST_CASE("dsps_fird_s16_ansi noise_snr", "[dsps]")
{
    int16_t s_coeffs[32];
    int16_t delay_line[32];
    int fir_len = sizeof(s_coeffs) / sizeof(int16_t);
    float ft = 0.25;                                            // Transition frequency
    int start_pos = 0;
    int shift = 0;
    fir_s16_t fir1;

    // FIR Coeffs
    float *f_coeffs = (float *)malloc(fir_len * sizeof(float));
    dsps_wind_hann_f32(f_coeffs, fir_len);
    float fir_order = (float)fir_len - 1;
    for(int i = 0; i < fir_len; i++){
        f_coeffs[i] *= sinf((2 * M_PI * ft * (i - fir_order / 2))) / (M_PI * (i - fir_order / 2));
    }

    // FIR coefficients conversion to q15
    for(int i = 0; i < fir_len; i++){
        s_coeffs[i] = f_coeffs[i] * (int16_t)Q15_MAX; 
    }

    // Signal generation
    dsps_tone_gen_f32(f_in_signal, FIR_BUFF, 0.9, 0.05, 0);

    // Input signal conversion to q15
    for(int i = 0; i < FIR_BUFF; i++){
        fir_x[i] = f_in_signal[i] * (int16_t)Q15_MAX;
    }  

    // FIR
    dsps_fird_init_s16(&fir1, s_coeffs, delay_line, fir_len, dec, start_pos, shift);
    dsps_fird_s16_ansi(&fir1, fir_x, fir_y, FIR_BUFF);

    // FIR Output conversion to float
    const unsigned int ignored_fir_samples = (FIR_BUFF_LEN / 2) - 1;
    float *fir_output = (float*)malloc(N_IN * sizeof(float));
    for(int i = 0; i < N_FFT; i++){
        fir_output[i] = (float)(fir_y[ignored_fir_samples + i] / (float)Q15_MAX);
    }

    // Signal windowing
    float *window = (float *)malloc((N_IN_SAMPLES/DECIMATION) * sizeof(float));
    dsps_wind_blackman_f32(window, N_FFT);

    // Prepare FFT input, real and imaginary part
    for (int i = 0 ; i < N_FFT ; i++) {
        fft_data[i * 2 + 0] = fir_output[i] * window[i];
        fft_data[i * 2 + 1] = 0;
    }

    esp_err_t ret = dsps_fft2r_init_fc32(NULL, N_FFT*2);
    if (ret  != ESP_OK)
    {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return;
    }

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

    float min_exec_snr = 50;
    float max_exec_snr = 120;
    TEST_ASSERT_EXEC_IN_RANGE(min_exec_snr, max_exec_snr, snr);

}
