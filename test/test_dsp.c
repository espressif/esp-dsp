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

#include "esp_dsp.h"
#include "report.inc"

static const char *TAG = "common";

// This is template test for future extensions
// Test on this level should test complex functionality
// for many modules
TEST_CASE("test template", "[dsp][ignore]")
{
	size_t size_before = xPortGetFreeHeapSize();
	size_t size_after = xPortGetFreeHeapSize();

	ptrdiff_t heap_diff = size_before - size_after;
	heap_diff = abs(heap_diff);
	if (heap_diff > 8) TEST_ASSERT_EQUAL(0, heap_diff);
}


#define dspm_mult_3x3x1_f32_ansi(data1, data2, data3) dspm_mult_f32_ansi(data1, data2, data3, 3, 3, 1)
#define dspm_mult_3x3x3_f32_ansi(data1, data2, data3) dspm_mult_f32_ansi(data1, data2, data3, 3, 3, 3)
#define dspm_mult_4x4x1_f32_ansi(data1, data2, data3) dspm_mult_f32_ansi(data1, data2, data3, 4, 4, 1)
#define dspm_mult_4x4x4_f32_ansi(data1, data2, data3) dspm_mult_f32_ansi(data1, data2, data3, 4, 4, 4)

TEST_CASE("DSP Libary benchmark table", "[dsp]")
{
    // This test generates benchmark rst table for all available functions
    const size_t test_size = 1024;

    esp_err_t ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Not possible to initialize floating point FFT-R2. Error = 0x%x", ret);
        abort();
    }
    ret = dsps_fft2r_init_sc16(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Not possible to initialize fixed point FFT-R2. Error = 0x%x", ret);
        abort();
    }

    ret = dsps_fft4r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Not possible to initialize fixed point FFT-R4. Error = 0x%x", ret);
        abort();
    }

    float* data1 = (float*) malloc(test_size * 2 * sizeof(float));
    float* data2 = (float*) malloc(test_size * 2 * sizeof(float));
    float* data3 = (float*) malloc(test_size * 2 * sizeof(float));
    if (!data1 || !data2 || !data3) {
        ESP_LOGE(TAG, "Failed to allocate buffers");
        abort();
    }

    fir_f32_t fir1;
    dsps_fir_init_f32(&fir1, data1, data2, 256);

    fir_f32_t fir2;
    dsps_fird_init_f32(&fir2, data1, data2, 256, 4, 0);

    float coeffs[5];
    dsps_biquad_gen_lpf_f32(coeffs, 0.1, 1);

    REPORT_HEADER();
    REPORT_SECTION("**Dot Product**");
    REPORT_BENCHMARK("dsps_dotprod_f32 for N=256 points",
                     dsps_dotprod_f32_ae32,
                     dsps_dotprod_f32_ansi,
                     data1, data2, data3, 256);

    REPORT_BENCHMARK("dsps_dotprode_f32 for N=256 points, with step 1",
                     dsps_dotprode_f32_ae32,
                     dsps_dotprode_f32_ansi,
                     data1, data2, data3, 256, 1, 1);

    REPORT_BENCHMARK("dsps_dotprod_s16 for N=256 points",
                     dsps_dotprod_s16_ae32,
                     dsps_dotprod_s16_ansi,
                     (int16_t *)data1, (int16_t *)data2, (int16_t *)data3, 256, 0);


    REPORT_SECTION("**FIR Filters**");

    REPORT_BENCHMARK("dsps_fir_f32 1024 input samples and 256 coefficients",
                     dsps_fir_f32_ae32,
                     dsps_fir_f32_ansi,
                     &fir1, data1, data2, 1024);

    REPORT_BENCHMARK("dsps_fird_f32 1024 samples, 256 coeffs and decimation 4",
                     dsps_fird_f32_ae32,
                     dsps_fird_f32_ansi,
                     &fir1, data1, data2, 1024);

    REPORT_SECTION("**FFTs Radix-2 32 bit Floating Point**");

    REPORT_BENCHMARK("dsps_fft2r_fc32 for  64 complex points",
                     dsps_fft2r_fc32_ae32,
                     dsps_fft2r_fc32_ansi,
                     data1, 64);

    REPORT_BENCHMARK("dsps_fft2r_fc32 for 128 complex points",
                     dsps_fft2r_fc32_ae32,
                     dsps_fft2r_fc32_ansi,
                     data1, 128);

    REPORT_BENCHMARK("dsps_fft2r_fc32 for 256 complex points",
                     dsps_fft2r_fc32_ae32,
                     dsps_fft2r_fc32_ansi,
                     data1, 256);

    REPORT_BENCHMARK("dsps_fft2r_fc32 for 512 complex points",
                     dsps_fft2r_fc32_ae32,
                     dsps_fft2r_fc32_ansi,
                     data1, 512);

    REPORT_BENCHMARK("dsps_fft2r_fc32 for 1024 complex points",
                     dsps_fft2r_fc32_ae32,
                     dsps_fft2r_fc32_ansi,
                     data1, 1024);

    REPORT_SECTION("**FFTs Radix-4 32 bit Floating Point**");

    REPORT_BENCHMARK("dsps_fft4r_fc32 for  64 complex points",
                     dsps_fft4r_fc32_ae32,
                     dsps_fft4r_fc32_ansi,
                     data1, 64);

    REPORT_BENCHMARK("dsps_fft4r_fc32 for 256 complex points",
                     dsps_fft4r_fc32_ae32,
                     dsps_fft4r_fc32_ansi,
                     data1, 256);

    REPORT_BENCHMARK("dsps_fft4r_fc32 for 1024 complex points",
                     dsps_fft4r_fc32_ae32,
                     dsps_fft4r_fc32_ansi,
                     data1, 1024);

    REPORT_SECTION("**FFTs 16 bit Fixed Point**");

    REPORT_BENCHMARK("dsps_fft2r_sc16 for  64 complex points",
                     dsps_fft2r_sc16_ae32,
                     dsps_fft2r_sc16_ansi,
                     (int16_t*)data1, 64);

    REPORT_BENCHMARK("dsps_fft2r_sc16 for 128 complex points",
                     dsps_fft2r_sc16_ae32,
                     dsps_fft2r_sc16_ansi,
                     (int16_t*)data1, 128);

    REPORT_BENCHMARK("dsps_fft2r_sc16 for 256 complex points",
                     dsps_fft2r_sc16_ae32,
                     dsps_fft2r_sc16_ansi,
                     (int16_t*)data1, 256);

    REPORT_BENCHMARK("dsps_fft2r_sc16 for 512 complex points",
                     dsps_fft2r_sc16_ae32,
                     dsps_fft2r_sc16_ansi,
                     (int16_t*)data1, 512);

    REPORT_BENCHMARK("dsps_fft2r_sc16 for 1024 complex points",
                     dsps_fft2r_sc16_ae32,
                     dsps_fft2r_sc16_ansi,
                     (int16_t*)data1, 1024);

    REPORT_SECTION("**IIR Filters**");

    REPORT_BENCHMARK("dsps_biquad_f32 - biquad filter for 1024 input samples",
                     dsps_biquad_f32_ae32,
                     dsps_biquad_f32_ansi,
                     data1, data2, 1024, coeffs, data3);

    REPORT_SECTION("**Matrix Multiplication**");

    REPORT_BENCHMARK("dspm_mult_f32 - C[16,16] = A[16,16]*B[16,16];",
                     dspm_mult_f32_ae32,
                     dspm_mult_f32_ansi,
                     data1, data2, data3, 16, 16, 16);

    REPORT_BENCHMARK("dspm_mult_s16 - C[16,16] = A[16,16]*B[16,16];",
                     dspm_mult_s16_ae32,
                     dspm_mult_s16_ansi,
                     (int16_t *)data1, (int16_t *)data2, (int16_t *)data3, 16, 16, 16, 0);

    REPORT_BENCHMARK("dspm_mult_3x3x1_f32 - C[3,1] = A[3,3]*B[3,1];",
                     dspm_mult_3x3x1_f32_ae32,
                     dspm_mult_3x3x1_f32_ansi,
                     data1, data2, data3);

    REPORT_BENCHMARK("dspm_mult_3x3x3_f32 - C[3,3] = A[3,3]*B[3,3];",
                     dspm_mult_3x3x3_f32_ae32,
                     dspm_mult_3x3x3_f32_ansi,
                     data1, data2, data3);

    REPORT_BENCHMARK("dspm_mult_4x4x1_f32 - C[4,1] = A[4,4]*B[4,1];",
                     dspm_mult_4x4x1_f32_ae32,
                     dspm_mult_4x4x1_f32_ansi,
                     data1, data2, data3);

    REPORT_BENCHMARK("dspm_mult_4x4x4_f32 - C[4,4] = A[4,4]*B[4,4];",
                     dspm_mult_4x4x4_f32_ae32,
                     dspm_mult_4x4x4_f32_ansi,
                     data1, data2, data3);

    dsps_fft2r_deinit_fc32();
    dsps_fft4r_deinit_fc32();
    dsps_fft2r_deinit_sc16();
    free(data1);
    free(data2);
    free(data3);
}
