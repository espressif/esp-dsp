#ifndef _dsp_types_H_
#define _dsp_types_H_
#include <stdint.h>
#include <stdbool.h>

// union to simplify access to the 16 bit data
typedef union sc16_u
{
    struct
    {
        int16_t re;
        int16_t im;
    };
    uint32_t data;
}sc16_t;

typedef union fc32_u
{
    struct
    {
        float re;
        float im;
    };
    uint64_t data;
}fc32_t;

#endif // _dsp_types_H_