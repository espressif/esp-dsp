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

#ifndef _dslm_mult_H_
#define _dslm_mult_H_

#include "dsl_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Matrix multiplication for two floating point matrices: C[m][k] = A[m][n] * B[n][k]
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param A  input matrix A[m][n]
 * @param B  input matrix B[n][k]
 * @param C  result matrix C[m][k]
 * @param m  matrix dimension
 * @param n  matrix dimension
 * @param k  matrix dimension
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dslm_mult_32f_ansi(float *A, float *B, float *C, int m, int n, int k);

/**
 * Matrix multiplication for two floating point matrices: C[m][k] = A[m][n] * B[n][k]
 * The implementation is optimized for ESP32 chip.
 *
 * @param A  input matrix A[m][n]
 * @param B  input matrix B[n][k]
 * @param C  result matrix C[m][k]
 * @param m  matrix dimension
 * @param n  matrix dimension
 * @param k  matrix dimension
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dslm_mult_32f_ae32(float *A, float *B, float *C, int m, int n, int k);

/**
 * Matrix multiplication for two floating point matrices 3x3 and 3x1: C[1][3] = A[3][3] * B[3][1]
 * The implementation is optimized for ESP32 chip.
 *
 * @param A  input matrix A[3][3]
 * @param B  input matrix/vector B[3][1]
 * @param C  result matrix/vector C[3][1]
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dslm_mult_3x3x1_32f_ae32(float *A, float *B, float *C);

/**
 * Matrix multiplication for two square 3x3 floating point matrices: C[3][3] = A[3][3] * B[3][3]
 * The implementation is optimized for ESP32 chip.
 *
 * @param A  input matrix A[3][3]
 * @param B  input matrix B[3][3]
 * @param C  result matrix C[3][3]
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dslm_mult_3x3x3_32f_ae32(float *A, float *B, float *C);
/**
 * Matrix multiplication for two floating point matrices 4x4 and 4x1: C[1][4] = A[4][4] * B[4][1]
 * The implementation is optimized for ESP32 chip.
 *
 * @param A  input matrix A[4][4]
 * @param B  input matrix/vector B[4][1]
 * @param C  result matrix/vector C[4][1]
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */

esp_err_t dslm_mult_4x4x1_32f_ae32(float *A, float *B, float *C);
/**
 * Matrix multiplication for two square 3x3 floating point matrices: C[4][4] = A[4][4] * B[4][4]
 * The implementation is optimized for ESP32 chip.
 *
 * @param A  input matrix A[4][4]
 * @param B  input matrix B[4][4]
 * @param C  result matrix C[4][4]
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dslm_mult_4x4x4_32f_ae32(float *A, float *B, float *C);

/**
 * Matrix multiplication for two signed 16 bit fixed point matrices: C[m][k] = (A[m][n] * B[n][k]) >> (15- shift)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param A  input matrix A[m][n]
 * @param B  input matrix B[n][k]
 * @param C  result matrix C[m][k]
 * @param m  matrix dimension
 * @param n  matrix dimension
 * @param k  matrix dimension
 * @param shift every result will be shifted and stored as 16 bit signed value.
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dslm_mult_16s_ansi(int16_t *A, int16_t *B, int16_t *C, int m, int n, int k, int shift);

/**
 * Matrix multiplication for two signed 16 bit fixed point matrices: C[m][k] = (A[m][n] * B[n][k]) >> (15- shift)
 * The implementation is optimized for ESP32 chip.
 *
 * @param A  input matrix A[m][n]
 * @param B  input matrix B[n][k]
 * @param C  result matrix C[m][k]
 * @param m  matrix dimension
 * @param n  matrix dimension
 * @param k  matrix dimension
 * @param shift every result will be shifted and stored as 16 bit signed value.
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dslm_mult_16s_ae32(int16_t *A, int16_t *B, int16_t *C, int m, int n, int k, int shift);

#ifdef __cplusplus
}
#endif


#ifdef CONFIG_DSP_OPTIMIZED
#define dslm_mult_32f dslm_mult_32f_ae32
#define dslm_mult_16s dslm_mult_16s_ae32
#define dslm_mult_3x3x1_32f dslm_mult_3x3x1_32f_ae32
#define dslm_mult_3x3x3_32f dslm_mult_3x3x3_32f_ae32
#define dslm_mult_4x4x1_32f dslm_mult_4x4x1_32f_ae32
#define dslm_mult_4x4x4_32f dslm_mult_4x4x4_32f_ae32
#endif
#ifdef CONFIG_DSP_ANSI
#define dslm_mult_32f dslm_mult_32f_ansi
#define dslm_mult_16s dslm_mult_16s_ansi

#define dslm_mult_3x3x1_32f(A,B,C) dslm_mult_32f_ansi(A,B,C, 3, 3, 1)
#define dslm_mult_3x3x3_32f(A,B,C) dslm_mult_32f_ansi(A,B,C, 3, 3, 3)
#define dslm_mult_4x4x1_32f(A,B,C) dslm_mult_32f_ansi(A,B,C, 4, 4, 1)
#define dslm_mult_4x4x4_32f(A,B,C) dslm_mult_32f_ansi(A,B,C, 4, 4, 4)
#endif


#endif // _dslm_mult_H_