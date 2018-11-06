#include "dsls_addC.h"

esp_err_t dsls_addC_32f_ansi(float* x, float* y, int len, float C, int step1, int step2)
{
    for (int i=0 ; i< len ; i++)
    {
        y[i*step2] = x[i*step1] + C;
    }
    return ESP_OK;
}