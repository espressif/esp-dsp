#ifndef _dsls_tone_gen_H_
#define _dsls_tone_gen_H_
#include "dsl_err.h"


#ifdef __cplusplus
extern "C"
{
#endif

// x - input buffer
// len - length of the buffer
// A - amplitude
// f - frequency -1...1 
// ph - phase in degree
//
// x[i] = A*sin(2*PI*i + ph/180*PI)
esp_err_t dsls_tone_gen_f32(float* x, int len, float A, float f, float ph);

#ifdef __cplusplus
}
#endif

#endif // _dsls_tone_gen_H_