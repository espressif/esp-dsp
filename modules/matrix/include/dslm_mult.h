#ifndef _dslm_mult_H_
#define _dslm_mult_H_

#include "dsl_err.h"

esp_err_t dslm_mult_32f_ansi(float* A, float* B, float* C, int m, int n, int k);
esp_err_t dslm_mult_32f_ae32(float* A, float* B, float* C, int m, int n, int k);
esp_err_t dslm_mult_3x3x3_32f_ae32(float* A, float* B, float* C);
esp_err_t dslm_mult_3x3x1_32f_ae32(float* A, float* B, float* C);
esp_err_t dslm_mult_4x4x1_32f_ae32(float* A, float* B, float* C);
esp_err_t dslm_mult_4x4x4_32f_ae32(float* A, float* B, float* C);

esp_err_t dslm_mult_16s_ansi(int16_t* A, int16_t* B, int16_t* C, int m, int n, int k, int shift);
esp_err_t dslm_mult_16s_ae32(int16_t* A, int16_t* B, int16_t* C, int m, int n, int k, int shift);

#endif // _dslm_mult_H_