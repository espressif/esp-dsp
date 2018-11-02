#include "dsls_d_gen.h"


esp_err_t dsls_d_gen_f32(float* x, int len, int pos)
{
    if (pos >= len) return ESP_ERR_DSL_PARAM_OUTOFRANGE;
    if (pos <    0) return ESP_ERR_DSL_PARAM_OUTOFRANGE;
    for (int i=0 ; i< len ; i++)   x[i] = 0;
    x[pos] = 1;
    return ESP_OK;
}