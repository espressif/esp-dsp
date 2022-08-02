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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include <math.h>

#include "esp_dsp.h"


static const char *TAG = "main";

// This example shows how to use fir filters from esp-dsp library

#define FIR_COEFFS          64                              // Number of FIR filter coefficients
#define F_T                 0.25                            // Transition frequency of the FIR filter
#define DECIMATION          2                               // Decimation ratio of the FIR filter
#define N_SAMPLES           1024                            // Input samples
#define FIR_BUFF_LEN        (FIR_COEFFS / DECIMATION)
#define FIR_BUFF            N_SAMPLES + FIR_BUFF_LEN

const static unsigned int fir_taps = FIR_COEFFS;
const static float fir_ft = F_T;
const static unsigned int fir_decim = DECIMATION;
const static unsigned int N = N_SAMPLES;
const static unsigned int N_buff = FIR_BUFF;

// Input test array 1
__attribute__((aligned(16)))
float tone_1[FIR_BUFF];
// Input test array 2
__attribute__((aligned(16)))
float tone_2[FIR_BUFF];
// Input test array
__attribute__((aligned(16)))
float tone_combined[FIR_BUFF];
// Windowing array
__attribute__((aligned(16)))
float window[N_SAMPLES];
// Windowing array
__attribute__((aligned(16)))
float fir_out[FIR_BUFF];
// FFT input array
__attribute__((aligned(16)))
float fft_data[N_SAMPLES*2];


// Function shows result of the IIR filter
void show_FFT(float *input_signal, float *fft_input, int N){

    // Prepare the input signal for the FFT, 
    for (int i = 0 ; i < N ; i++){
        fft_input[i * 2 + 0] = input_signal[i];
        fft_input[i * 2 + 1] = 0;
    }

    // Do the FFT
    dsps_fft2r_fc32(fft_input, N);
    dsps_bit_rev_fc32(fft_input, N);
    dsps_cplx2reC_fc32(fft_input, N);

    float correction_factor = N * 3;

    // Calculating power of spectrum in dB
    for (int i = 0 ; i < N/2 ; i++) {
        fft_input[i] = 10 * log10f((fft_input[i * 2 + 0] * fft_input[i * 2 + 0] + fft_input[i * 2 + 1] * fft_input[i * 2 + 1])/correction_factor);
    }

    // Display power spectrum
    dsps_view(fft_input, N/2, 64, 10,  -120, 40, '|');
}

// Generate Windowed-Sinc filter coefficients
void generate_FIR_coefficients(float *fir_coeffs, int fir_len, float ft){

    // Even or odd length of the FIR filter
    bool is_odd = (fir_len % 2) ? (true) : (false);                
    float fir_order = (float)(fir_len - 1);

    // Window coefficients
    float *fir_window = (float*)malloc(fir_len*sizeof(float));
    dsps_wind_blackman_f32(fir_window, fir_len);

    for(int i = 0; i < fir_len; i++){
        if((i == fir_order / 2) && (is_odd))
            fir_coeffs[i] = 2 * ft;
        else
            fir_coeffs[i] = sinf((2 * M_PI * ft * (i - fir_order / 2))) / (M_PI * (i - fir_order / 2));

        fir_coeffs[i] *= fir_window[i];
    }
}


void app_main()
{
    float fir_coeffs[fir_taps];
    float delay_line[fir_taps];
    unsigned int fir_len = sizeof(fir_coeffs) / sizeof(float);
    fir_f32_t fir1;
    esp_err_t ret;

    ESP_LOGI(TAG, "Start Example.");

    // If user don't care about buffer allocation, the defalt 
    // initialization could be used as shown here:
    ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret  != ESP_OK)
    {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return;
    }

    // Generate input signal as 2 sine waves
    dsps_tone_gen_f32(tone_1, N_buff, 5, 0.2, 0);
    dsps_tone_gen_f32(tone_2, N_buff, 5, 0.4, 0);

    // Generage windowing coeffcients
    dsps_wind_blackman_harris_f32(window, N);

    // Add the two waves toghether
    for (int i = 0 ; i < N_buff ; i++){
        tone_combined[i] = tone_1[i] + tone_2[i];
    }

    // Apply the windowing
    for (int i = 0 ; i < N ; i++){
        window[i] *= tone_combined[i];
    }

    // Show FFT spectrum
    show_FFT(window, fft_data, N);

    // Calculate coefficinets for FIR filter
    generate_FIR_coefficients(fir_coeffs, fir_len, fir_ft);
    ESP_LOGI(TAG, "\n");

    // Filter the input signal with FIR filter
    dsps_fird_init_f32(&fir1, fir_coeffs, delay_line, fir_len, fir_decim, 0);

    unsigned int start_b = dsp_get_cpu_cycle_count();
    dsps_fird_f32_ansi(&fir1, tone_combined, fir_out, N_buff);
    unsigned int end_b = dsp_get_cpu_cycle_count();

    // Ignoring first set of samples, due to the delay line of the FIR filter
    int fir_out_offset = ((FIR_BUFF_LEN / 2) - 1);

    // Generage windowing coeffcients and apply the windowing
    dsps_wind_blackman_harris_f32(window, (N / fir_decim));
    for (int i = 0 ; i < N / fir_decim ; i++){
        window[i] *= fir_out[fir_out_offset + i];
    }

    // Show FFT spectrum, ignoring first samples from the delay line
    show_FFT(window, fft_data, N / fir_decim);
    ESP_LOGI(TAG, "FIR for %d samples and decimation %d takes %d cycles", N, fir_decim, end_b - start_b);
    ESP_LOGI(TAG, "End Example.");
}
