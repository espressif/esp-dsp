#include "dsls_fir.h"

esp_err_t dsls_fir_32f_ansi(fir_32f_t* fir, float* x, float* y, int len)
{
    for (int i=0 ; i< len ; i++)
    {
        float acc = 0;
        int coeff_pos = fir->N;
        fir->delay[fir->pos] = x[i];
        fir->pos++;
        if (fir->pos > fir->N) fir->pos = 0;
        for (int n=fir->pos; n <= fir->N ; n++)
        {
            acc += fir->coeffs[coeff_pos--]*fir->delay[n];
        }
        for (int n=0; n < fir->pos ; n++)
        {
            acc += fir->coeffs[coeff_pos--]*fir->delay[n];
        }
        y[i] = acc;
    }
    return ESP_OK;
}

