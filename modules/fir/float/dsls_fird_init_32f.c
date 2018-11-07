#include "dsls_fir.h"


esp_err_t dsls_fird_init_32f(fir_32f_t* fir, float* coeffs, float* delay, int N, int d, int start_pos)
{
    fir->coeffs = coeffs;
    fir->delay = delay;
    fir->N = N-1;
    fir->pos = 0;
    fir->d = d;
    fir->d_pos = start_pos;
    if (fir->d_pos >= fir->d) return ESP_ERR_DSL_PARAM_OUTOFRANGE;

    for (int i=0 ; i< N; i++)
    {
        fir->delay[i] = 0;
    }
    return ESP_OK;
}
