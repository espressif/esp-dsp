#ifndef _dsls_h_gen_H_
#define _dsls_h_gen_H_
#include "dsl_err.h"


#ifdef __cplusplus
extern "C"
{
#endif

// x - input buffer
// len - length of the buffer
// pos - position of inpuls [0..len)

esp_err_t dsls_h_gen_f32(float* x, int len, int pos);

#ifdef __cplusplus
}
#endif

#endif // _dsls_h_gen_H_