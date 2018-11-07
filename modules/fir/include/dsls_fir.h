#ifndef _dsls_fir_H_
#define _dsls_fir_H_


#include "dsl_err.h"


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct fir_32f_s
{
    float*  coeffs;
    float*  delay;
    int     N;
    int     pos;
    int     d;
    int     d_pos;
}fir_32f_t;

esp_err_t dsls_fir_init_32f(fir_32f_t* fir, float* coeffs, float* delay, int N);

// d - decimation factor
// start_pos - initial position for delay sample
esp_err_t dsls_fird_init_32f(fir_32f_t* fir, float* coeffs, float* delay, int N, int d, int start_pos);

esp_err_t dsls_fir_32f_ansi(fir_32f_t* fir, float* x, float* y, int len);

// return - amount of processed samples
int dsls_fird_32f_ansi(fir_32f_t* fir, float* x, float* y, int len);

#ifdef __cplusplus
}
#endif

#endif // _dsls_fir_H_