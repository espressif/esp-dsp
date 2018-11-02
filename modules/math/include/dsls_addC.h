#ifndef _dsls_addC_H_
#define _dsls_addC_H_
#include "dsl_err.h"


#ifdef __cplusplus
extern "C"
{
#endif

// x - input buffer
// len - length of the buffer
// pos - position of inpuls [0..len)
// step1 - step through 1 array (by default - 1)
// step2 - step through 2 array (by default - 1)

esp_err_t dsls_addC_32f_ansi(float* x, float* y, int len, float C, int step1, int step2);

#ifdef __cplusplus
}
#endif

#endif // _dsls_addC_H_