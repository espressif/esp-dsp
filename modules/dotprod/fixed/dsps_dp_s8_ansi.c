#include "dsps_dotprod.h"

esp_err_t dsps_dp_s8_ansi(const int8_t *src1, const int8_t *src2, int32_t *dest, int len)
{
    int32_t acc = 0;
    for (int i = 0 ; i < len ; i++) {
        acc += (int32_t)src1[i] * (int32_t)src2[i];
    }
    *dest = acc;
    return ESP_OK;
}
