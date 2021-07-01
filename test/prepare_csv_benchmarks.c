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
#include <malloc.h>

#include "esp_dsp.h"
#include "sdkconfig.h"
#include "report.inc"
#include "dsp_tests.h"


static const char *TAG = "Build csv data";

#define dspm_mult_3x3x1_f32_ansi(data1, data2, data3) dspm_mult_f32_ansi(data1, data2, data3, 3, 3, 1)
#define dspm_mult_3x3x3_f32_ansi(data1, data2, data3) dspm_mult_f32_ansi(data1, data2, data3, 3, 3, 3)
#define dspm_mult_4x4x1_f32_ansi(data1, data2, data3) dspm_mult_f32_ansi(data1, data2, data3, 4, 4, 1)
#define dspm_mult_4x4x4_f32_ansi(data1, data2, data3) dspm_mult_f32_ansi(data1, data2, data3, 4, 4, 4)

TEST_CASE("DSP Benchmarks data", "[dsp]")
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

    float* data1 = (float*) memalign(16, test_size * 2 * sizeof(float));
    float* data2 = (float*) memalign(16, test_size * 2 * sizeof(float));
    float* data3 = (float*) memalign(16, test_size * 2 * sizeof(float));

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

    uint32_t compiler_opt = 0;
    uint32_t chip_id = 0;

// The values here linked to the build_mb_table.py script
#ifdef CONFIG_COMPILER_OPTIMIZATION_PERF
    compiler_opt = 1;
#endif 
#ifdef CONFIG_COMPILER_OPTIMIZATION_SIZE
    compiler_opt = 2;
#endif
#ifdef CONFIG_IDF_TARGET_ESP32
    chip_id = 1;
#endif 
#ifdef CONFIG_IDF_TARGET_ESP32S2
    chip_id = 2;
#endif 
#ifdef CONFIG_IDF_TARGET_ESP32S3
    chip_id = 3;
#endif 


    REPORT_SECTION_NAME("**Dot Product**");
    REPORT_BENCHMARK_CSV("dsps_dotprod_f32 for N=256 points",
                     dsps_dotprod_f32,
                     dsps_dotprod_f32_ansi,
                     data1, data2, data3, 256);

    REPORT_BENCHMARK_CSV("dsps_dotprode_f32 for N=256 points with step 1",
                     dsps_dotprode_f32,
                     dsps_dotprode_f32_ansi,
                     data1, data2, data3, 256, 1, 1);

    REPORT_BENCHMARK_CSV("dsps_dotprod_s16 for N=256 points",
                     dsps_dotprod_s16,
                     dsps_dotprod_s16_ansi,
                     (int16_t *)data1, (int16_t *)data2, (int16_t *)data3, 256, 0);


    REPORT_SECTION_NAME("**FIR Filters**");

    REPORT_BENCHMARK_CSV("dsps_fir_f32 1024 input samples and 256 coefficients",
                     dsps_fir_f32,
                     dsps_fir_f32_ansi,
                     &fir1, data1, data2, 1024);

    REPORT_BENCHMARK_CSV("dsps_fird_f32 1024 samples 256 coeffs and decimation 4",
                     dsps_fird_f32,
                     dsps_fird_f32_ansi,
                     &fir2, data1, data2, 1024);

    REPORT_SECTION_NAME("**FFTs Radix-2 32 bit Floating Point**");

    REPORT_BENCHMARK_CSV("dsps_fft2r_fc32 for  64 complex points",
                     dsps_fft2r_fc32,
                     dsps_fft2r_fc32_ansi,
                     data1, 64);

    REPORT_BENCHMARK_CSV("dsps_fft2r_fc32 for 128 complex points",
                     dsps_fft2r_fc32,
                     dsps_fft2r_fc32_ansi,
                     data1, 128);

    REPORT_BENCHMARK_CSV("dsps_fft2r_fc32 for 256 complex points",
                     dsps_fft2r_fc32,
                     dsps_fft2r_fc32_ansi,
                     data1, 256);

    REPORT_BENCHMARK_CSV("dsps_fft2r_fc32 for 512 complex points",
                     dsps_fft2r_fc32,
                     dsps_fft2r_fc32_ansi,
                     data1, 512);

    REPORT_BENCHMARK_CSV("dsps_fft2r_fc32 for 1024 complex points",
                     dsps_fft2r_fc32,
                     dsps_fft2r_fc32_ansi,
                     data1, 1024);

    REPORT_SECTION_NAME("**FFTs Radix-4 32 bit Floating Point**");

    REPORT_BENCHMARK_CSV("dsps_fft4r_fc32 for  64 complex points",
                     dsps_fft4r_fc32,
                     dsps_fft4r_fc32_ansi,
                     data1, 64);

    REPORT_BENCHMARK_CSV("dsps_fft4r_fc32 for 256 complex points",
                     dsps_fft4r_fc32,
                     dsps_fft4r_fc32_ansi,
                     data1, 256);

    REPORT_BENCHMARK_CSV("dsps_fft4r_fc32 for 1024 complex points",
                     dsps_fft4r_fc32,
                     dsps_fft4r_fc32_ansi,
                     data1, 1024);

    REPORT_SECTION_NAME("**FFTs 16 bit Fixed Point**");

    REPORT_BENCHMARK_CSV("dsps_fft2r_sc16 for  64 complex points",
                     dsps_fft2r_sc16,
                     dsps_fft2r_sc16_ansi,
                     (int16_t*)data1, 64);

    REPORT_BENCHMARK_CSV("dsps_fft2r_sc16 for 128 complex points",
                     dsps_fft2r_sc16,
                     dsps_fft2r_sc16_ansi,
                     (int16_t*)data1, 128);

    REPORT_BENCHMARK_CSV("dsps_fft2r_sc16 for 256 complex points",
                     dsps_fft2r_sc16,
                     dsps_fft2r_sc16_ansi,
                     (int16_t*)data1, 256);

    REPORT_BENCHMARK_CSV("dsps_fft2r_sc16 for 512 complex points",
                     dsps_fft2r_sc16,
                     dsps_fft2r_sc16_ansi,
                     (int16_t*)data1, 512);

    REPORT_BENCHMARK_CSV("dsps_fft2r_sc16 for 1024 complex points",
                     dsps_fft2r_sc16,
                     dsps_fft2r_sc16_ansi,
                     (int16_t*)data1, 1024);

    REPORT_SECTION_NAME("**IIR Filters**");

    REPORT_BENCHMARK_CSV("dsps_biquad_f32 - biquad filter for 1024 input samples",
                     dsps_biquad_f32,
                     dsps_biquad_f32_ansi,
                     data1, data2, 1024, coeffs, data3);

    REPORT_SECTION_NAME("**Matrix Multiplication**");

    REPORT_BENCHMARK_CSV("dspm_mult_f32 - C[16;16] = A[16;16]*B[16;16]",
                     dspm_mult_f32,
                     dspm_mult_f32_ansi,
                     data1, data2, data3, 16, 16, 16);

    REPORT_BENCHMARK_CSV("dspm_mult_s16 - C[16;16] = A[16;16]*B[16;16]",
                     dspm_mult_s16,
                     dspm_mult_s16_ansi,
                     (int16_t *)data1, (int16_t *)data2, (int16_t *)data3, 16, 16, 16, 0);

    REPORT_BENCHMARK_CSV("dspm_mult_3x3x1_f32 - C[3;1] = A[3;3]*B[3;1]",
                     dspm_mult_3x3x1_f32,
                     dspm_mult_3x3x1_f32_ansi,
                     data1, data2, data3);

    REPORT_BENCHMARK_CSV("dspm_mult_3x3x3_f32 - C[3;3] = A[3;3]*B[3;3]",
                     dspm_mult_3x3x3_f32,
                     dspm_mult_3x3x3_f32_ansi,
                     data1, data2, data3);

    REPORT_BENCHMARK_CSV("dspm_mult_4x4x1_f32 - C[4;1] = A[4;4]*B[4;1]",
                     dspm_mult_4x4x1_f32,
                     dspm_mult_4x4x1_f32_ansi,
                     data1, data2, data3);

    REPORT_BENCHMARK_CSV("dspm_mult_4x4x4_f32 - C[4;4] = A[4;4]*B[4;4]",
                     dspm_mult_4x4x4_f32,
                     dspm_mult_4x4x4_f32_ansi,
                     data1, data2, data3);

//#ifdef CONFIG_IDF_TARGET_ESP32S3
    REPORT_SECTION_NAME("**Image processing prototypes**");
    // s8
    {
        image2d_t image1 = {data1, 1, 1, 64, 64}; // Image 64x64
        image2d_t image2 = {data2, 1, 1, 16, 16}; // Umage 16x16

        REPORT_BENCHMARK_CSV("dspi_dotprod_s8/u8 - dotproduct of two images 16x16",
                        dspi_dotprod_s8,
                        dspi_dotprod_s8_ansi,
                        &image1, &image2, (int8_t*)data2, 16, 16, 1);
    }
    {
        image2d_t image1 = {data1, 1, 1, 64, 64}; // Image 64x64
        image2d_t image2 = {data2, 1, 1, 16, 16}; // Umage 16x16

        REPORT_BENCHMARK_CSV("dspi_dotprod_off_s8/u8 - dotproduct of two images 16x16",
                        dspi_dotprod_off_s8,
                        dspi_dotprod_off_s8_ansi,
                        &image1, &image2, (int8_t*)data2, 16, 16, 1, 10);
    }
    {
        image2d_t image1 = {data1, 1, 1, 64, 64}; // Image 64x64
        image2d_t image2 = {data2, 1, 1, 64, 64}; // Umage 64x64

        REPORT_BENCHMARK_CSV("dspi_dotprod_s8/u8- dotproduct of two images 64x64",
                        dspi_dotprod_s8,
                        dspi_dotprod_s8_ansi,
                        &image1, &image2, (int8_t*)data2, 64, 64, 1);
    }
    {
        image2d_t image1 = {data1, 1, 1, 64, 64}; // Image 64x64
        image2d_t image2 = {data2, 1, 1, 64, 64}; // Umage 64x64

        REPORT_BENCHMARK_CSV("dspi_dotprod_off_s8/u8 - dotproduct of two images 64x64",
                        dspi_dotprod_off_s8,
                        dspi_dotprod_off_s8_ansi,
                        &image1, &image2, (int8_t*)data2, 64, 64, 1, 10);
    }
    // s16
    {
        image2d_t image1 = {data1, 1, 1, 32, 32}; // Image 32x32
        image2d_t image2 = {data2, 1, 1, 8, 8}; // Umage 8x8

        REPORT_BENCHMARK_CSV("dspi_dotprod_s16/u16 - dotproduct of two images 8x8",
                        dspi_dotprod_s16,
                        dspi_dotprod_s16_ansi,
                        &image1, &image2, (int16_t*)data2, 8, 8, 1);
    }
    {
        image2d_t image1 = {data1, 1, 1, 32, 32}; // Image 32x32
        image2d_t image2 = {data2, 1, 1, 8, 8}; // Umage 8x8

        REPORT_BENCHMARK_CSV("dspi_dotprod_off_s16/u16 - dotproduct of two images 8x8",
                        dspi_dotprod_off_s16,
                        dspi_dotprod_off_s16_ansi,
                        &image1, &image2, (int16_t*)data2, 8, 8, 1, 10);
    }
    {
        image2d_t image1 = {data1, 1, 1, 32, 32}; // Image 64x64
        image2d_t image2 = {data2, 1, 1, 32, 32}; // Umage 64x64

        REPORT_BENCHMARK_CSV("dspi_dotprod_s16 - dotproduct of two images 32x32",
                        dspi_dotprod_s16,
                        dspi_dotprod_s16_ansi,
                        &image1, &image2, (int16_t*)data2, 32, 32, 1);
    }
    {
        image2d_t image1 = {data1, 1, 1, 32, 32}; // Image 32x32
        image2d_t image2 = {data2, 1, 1, 32, 32}; // Umage 32x32

        REPORT_BENCHMARK_CSV("dspi_dotprod_off_s16/u16 - dotproduct of two images 32x32",
                        dspi_dotprod_off_s16,
                        dspi_dotprod_off_s16_ansi,
                        &image1, &image2, (int16_t*)data2, 32, 32, 1, 10);
    }

//#endif

    dsps_fft2r_deinit_fc32();
    dsps_fft4r_deinit_fc32();
    dsps_fft2r_deinit_sc16();
    free(data1);
    free(data2);
    free(data3);
}
