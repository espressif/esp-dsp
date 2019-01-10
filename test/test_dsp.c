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
#include "test_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portable.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_clk.h"
#include "soc/cpu.h"

#include "esp_dsp.h"

static const char *TAG = "common";

// This is template test for future extensions
// Test on this level should test complex functionality 
// for many modules 
TEST_CASE("DSP Libary doesn't leak memory", "[dsp]")
{
	size_t size_before = xPortGetFreeHeapSize();
	size_t size_after = xPortGetFreeHeapSize();

	ptrdiff_t stack_diff = size_before - size_after; 
	stack_diff = abs(stack_diff);
	if (stack_diff > 8) TEST_ASSERT_EQUAL(0, stack_diff);
}

#define TEST_SIZE 1024
// Some buffers
float data1[TEST_SIZE*2];
float data2[TEST_SIZE*2];
float data3[TEST_SIZE*2];

#define PRINT_UNDER_LINE(col1) \
    printf("+%s+----------+----------+\n", col1);

#define PRINT_MAIN__LINE(col1, func1, func2) \
    func1; \
    start_test = xthal_get_ccount(); \
    func1; \
    end_test = xthal_get_ccount(); \
    cycles1 = end_test - start_test;\
    func2; \
    start_test = xthal_get_ccount(); \
    func2; \
    end_test = xthal_get_ccount(); \
    cycles2 = end_test - start_test; \
    printf("|%s|%10i|%10i|\n", col1, cycles1, cycles2);

#define PRINT_TEXT__LINE(col1) \
    printf("|%s|          |          |\n", col1);


TEST_CASE("DSP Libary benchmark table", "[dsp]")
{
    // This test generates benchmark rst table for all available functions
    unsigned int start_test = 0;
    unsigned int end_test = 0;
    unsigned int cycles1 = 0;
    unsigned int cycles2 = 0;

    esp_err_t ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    fir_f32_t fir1;
    fir_f32_t fir2;
    dsps_fir_init_f32(&fir1, data1, data2, 256);
    dsps_fird_init_f32(&fir2, data1, data2, 256, 4, 0);
    float coeffs[5];
    dsps_biquad_gen_lpf_f32(coeffs, 0.1, 1);

    if (ret  != ESP_OK)
    {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return;
    }

    PRINT_UNDER_LINE("----------------------------------------------------------");
             printf("+Function description/ execution time in CPU cycles        |   Esp32  |  Ansi    +\n");
             printf("+==========================================================+==========+==========+\n");
    PRINT_TEXT__LINE("                                                          ");
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_TEXT__LINE(" **Dot Product**                                          ");
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_MAIN__LINE("dsps_dotprod_f32 for N= 256 points                        ", dsps_dotprod_f32_ae32(data1, data2, data3, 256), dsps_dotprod_f32_ansi(data1, data2, data3, 256));
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_MAIN__LINE("dsps_dotprode_f32 for N= 256 points, with step 1          ", dsps_dotprode_f32_ae32(data1, data2, data3, 256, 1, 1), dsps_dotprode_f32_ansi(data1, data2, data3, 256, 1, 1));
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_MAIN__LINE("dsps_dotprod_s16 for N= 256 points                        ", dsps_dotprod_s16_ae32((int16_t *)data1, (int16_t *)data2, (int16_t *)data3, 256, 0), dsps_dotprod_s16_ansi((int16_t *)data1, (int16_t *)data2, (int16_t *)data3, 256, 0));
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_TEXT__LINE("                                                          ");
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_TEXT__LINE(" **FIR filters**                                          ");
    PRINT_UNDER_LINE("----------------------------------------------------------");    
    PRINT_MAIN__LINE("dsps_fir_f32 1024 input samples and 256 coefficients      ", dsps_fir_f32_ae32(&fir1, data1, data2, 1024), dsps_fir_f32_ansi(&fir1, data1, data2, 1024));
    PRINT_UNDER_LINE("----------------------------------------------------------");    
    PRINT_MAIN__LINE("dsps_fird_f32 1024 samples, 256 coeffs and decimation 4   ", dsps_fird_f32_ae32(&fir2, data1, data2, 1024), dsps_fird_f32_ansi(&fir2, data1, data2, 1024));
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_TEXT__LINE("                                                          ");
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_TEXT__LINE(" **FFTs**                                                 ");
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_MAIN__LINE("dsps_fft2r_fc32 for  64 complex points                    ", dsps_fft2r_fc32_ae32(data1, 64), dsps_fft2r_fc32_ansi(data1, 64));
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_MAIN__LINE("dsps_fft2r_fc32 for 128 complex points                    ", dsps_fft2r_fc32_ae32(data1, 128), dsps_fft2r_fc32_ansi(data1, 128));
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_MAIN__LINE("dsps_fft2r_fc32 for 256 complex points                    ", dsps_fft2r_fc32_ae32(data1, 256), dsps_fft2r_fc32_ansi(data1, 256));
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_MAIN__LINE("dsps_fft2r_fc32 for 512 complex points                    ", dsps_fft2r_fc32_ae32(data1, 512), dsps_fft2r_fc32_ansi(data1, 512));
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_MAIN__LINE("dsps_fft2r_fc32 for 1023 complex points                   ", dsps_fft2r_fc32_ae32(data1, 1024), dsps_fft2r_fc32_ansi(data1, 1024));
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_TEXT__LINE("                                                          ");
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_TEXT__LINE(" **IIR filters**                                          ");
    PRINT_UNDER_LINE("----------------------------------------------------------");    
    PRINT_MAIN__LINE("dsps_biquad_f32 - biquad filter for 1024 input samples    ", dsps_biquad_f32_ae32(data1, data2, 1024, coeffs, data3), dsps_biquad_f32_ansi(data1, data2, 1024, coeffs, data3));
    PRINT_UNDER_LINE("----------------------------------------------------------");    
    PRINT_TEXT__LINE("                                                          ");
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_TEXT__LINE(" **Matrix Multiplication**                                ");
    PRINT_UNDER_LINE("----------------------------------------------------------");    
    PRINT_MAIN__LINE("dspm_mult_f32 - C[16,16] = A[16,16]*B[16,16];             ", dspm_mult_f32_ae32(data1, data2, data3, 16,16,16), dspm_mult_f32_ansi(data1, data2, data3, 16,16,16));
    PRINT_UNDER_LINE("----------------------------------------------------------");    
    PRINT_MAIN__LINE("dspm_mult_s16 - C[16,16] = A[16,16]*B[16,16];             ", dspm_mult_s16_ae32((int16_t *)data1, (int16_t *)data2, (int16_t *)data3, 16,16,16, 0), dspm_mult_s16_ansi((int16_t *)data1, (int16_t *)data2, (int16_t *)data3, 16,16,16, 0));
    PRINT_UNDER_LINE("----------------------------------------------------------");
    PRINT_MAIN__LINE("dspm_mult_3x3x1_f32 - C[3,3] = A[3,3]*B[3,1];             ", dspm_mult_3x3x1_f32_ae32(data1, data2, data3), dspm_mult_f32_ansi(data1, data2, data3, 3, 3, 1));
    PRINT_UNDER_LINE("----------------------------------------------------------");    
    PRINT_MAIN__LINE("dspm_mult_3x3x3_f32 - C[3,3] = A[3,3]*B[3,3];             ", dspm_mult_3x3x3_f32_ae32(data1, data2, data3), dspm_mult_f32_ansi(data1, data2, data3, 3, 3, 3));
    PRINT_UNDER_LINE("----------------------------------------------------------");    
    PRINT_MAIN__LINE("dspm_mult_4x4x1_f32 - C[4,4] = A[4,4]*B[4,1];             ", dspm_mult_4x4x1_f32_ae32(data1, data2, data3), dspm_mult_f32_ansi(data1, data2, data3, 4, 4, 1));
    PRINT_UNDER_LINE("----------------------------------------------------------");    
    PRINT_MAIN__LINE("dspm_mult_4x4x4_f32 - C[4,4] = A[4,4]*B[4,4];             ", dspm_mult_4x4x4_f32_ae32(data1, data2, data3), dspm_mult_f32_ansi(data1, data2, data3, 4, 4, 4));
    PRINT_UNDER_LINE("----------------------------------------------------------");    

    dsps_fft2r_deinit();
}
