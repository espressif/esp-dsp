#include "dsls_fir.h"


esp_err_t dsls_fird_init_32f(fir_32f_t* fir, float* coeffs, float* delay, int N, int d)
{
    fir->coeffs = coeffs;
    fir->delay = delay;
    fir->N = N-1;
    fir->pos = 0;
    fir->d = d;
    fir->d_pos = 0;
    
    for (int i=0 ; i< N; i++)
    {
        fir->delay[i] = 0;
    }
    return ESP_OK;
}
