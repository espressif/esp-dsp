#include "dsls_h_gen.h"

esp_err_t dsls_h_gen_f32(float* x, int len, int pos)
{
    if (pos >= len) return ESP_ERR_DSL_PARAM_OUTOFRANGE;
    if (pos <    0) return ESP_ERR_DSL_PARAM_OUTOFRANGE;
    for (int i=0 ; i< pos ; i++)   x[i] = 0;
    for (int i=pos ; i< len ; i++) x[i] = 1;
    return ESP_OK;
}
