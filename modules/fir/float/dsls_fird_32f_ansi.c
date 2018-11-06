#include "dsls_fir.h"

int dsls_fird_32f_ansi(fir_32f_t* fir, float* x, float* y, int len)
{
    int result = 0;
    for (int i=0; i < len ; i++)
    {
        fir->delay[fir->pos++] = x[i];
        if (fir->pos > fir->N) fir->pos = 0;
        fir->d_pos++;
        if (fir->d_pos >= fir->d)
        {
            fir->d_pos = 0;
            float acc = 0;
            int coeff_pos = fir->N;
            for (int n=fir->pos; n <= fir->N ; n++)
            {
                acc += fir->coeffs[coeff_pos--]*fir->delay[n];
            }
            for (int n=0; n < fir->pos ; n++)
            {
                acc += fir->coeffs[coeff_pos--]*fir->delay[n];
            }
            y[result++] = acc;
        }
    }
    return result;
}