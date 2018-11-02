#include "dsls_tone_gen.h"
#include <math.h>

esp_err_t dsls_tone_gen_f32(float* x, int len, float A, float f, float ph)
{
    if (f>= 1) return ESP_ERR_DSL_INVALID_PARAM;
    if (f<=-1) return ESP_ERR_DSL_INVALID_PARAM;
    float phase = ph/180*M_PI; 
    float freq  = 2*M_PI*f; 
    for (int i=0 ; i< len ; i++)
    {
        x[i] = A*sin(phase);
        phase += freq;
        if (phase > 2*M_PI) phase -= 2*M_PI;
        if (phase < -2*M_PI) phase += 2*M_PI;
    }
    return ESP_OK;
}
