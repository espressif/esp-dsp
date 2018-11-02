#ifndef _dsls_d_gen_H_
#define _dsls_d_gen_H_
#include "dsl_err.h"


#ifdef __cplusplus
extern "C"
{
#endif

// x - input buffer
// len - length of the buffer
// pos - position of inpuls

esp_err_t dsls_d_gen_f32(float* x, int len, int pos);

#ifdef __cplusplus
}
#endif

#endif // _dsls_d_gen_H_