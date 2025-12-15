/*
 * SPDX-FileCopyrightText: 2018-2025 Espressif Systems (Shanghai) CO LTD
 * SPDX-FileContributor: 2025 Schuwi
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "dsps_fir_platform.h"
#if (dsps_fird_f32_aes3_enabled == 1)

// This is FIR filter for Esp32s3 processor.
    .text
    .align  4
    .global dsps_fird_f32_aes3
    .type   dsps_fird_f32_aes3,@function
// The function implements the following C code:
//esp_err_t dsps_fird_f32_aes3(fir_f32_t* fir, const float* input, float* output, int len);

dsps_fird_f32_aes3:

// a2 - fir structure
// a3 - input
// a4 - output
// a5 - length

// a6 - fir->N - amount of coefficients
// a7 - fir->pos - position in delay line
// a8 - temp
// a9 - return value (= length)
// a10 - fir->coeffs - pointer to constant coefficients
// a11 - fir->delay - pointer to delay line
// a12 - constant: -16 (= 0xFFFFFFF0)
// a13 - constant:  15 (= 0x0000000F)
// a14 - temp for loops
// a15 - delay line rounded down to 16

    entry	a1, 16
    // Array increment for floating point data should be 4
    l32i    a7,  a2, 12 // a7  - pos

    l32i    a6,  a2, 8  // a6  - N - amount of coefficients
    l32i    a11, a2, 4  // a11 - delay line
    addx4	a11, a7, a11 // a11 = a11 + a7*4	

    mov.n   a9, a5

    movi.n	a12, -16
    movi.n	a13, 15
// Main loop for input samples
.fird_loop_len:
        // Store K values from input to delay line:

        l32i    a14,  a2, 16   // a14  - fir->decim = K (decimation factor)
        loopnez  a14, .fird_load_data // K loops
            // Store to delay line
            lsip	f15,  a3, 4		// a3  += 4, f15 = input[n]
            ssip	f15, a11, 4		// a11 += 4, *a11 = f15
            addi    a7,  a7,  1     // a7++ - position in delay line

            blt     a7, a6, .do_not_reset_a11
                l32i    a11, a2, 4	// Load delay line
                movi    a7,  0
            .do_not_reset_a11:
            and     a15, a11, a12
        .fird_load_data:
        //
        // Process data
        //

        l32i    a10, a2, 0  // a10 - coeffs

        // Clear f4, f5, f6, f7 for multiplications
        const.s f4, 0
        const.s f5, 0
        const.s f6, 0
        const.s f7, 0

        // Branch according to the current position (modulo 4) in delay line
        and		a8, a11, a13		// a8 = a11 & 15
        beqz   	a8, .offset_0
        addi   	a8, a8, -4
        beqz   	a8, .offset_1
        addi   	a8, a8, -4
        beqz   	a8, .offset_2
        addi   	a8, a8, -4
        beqz   	a8, .offset_3

// a10 - coeffs
// a11 - delay line
.offset_0:
        // a14 = (N - pos) / 4
        sub   a14, a6, a7   // a14 = N-pos
        srli  a14, a14, 2
        loopnez  a14, .first_fir_loop_0 // d in [pos,N[ (stride of 4) ; c in [0,N-pos[ (stride of 4)
            EE.LDF.128.IP f3, f2, f1, f0, a10, 16 // Load coeffs
            EE.LDF.128.IP f11, f10, f9, f8, a15, 16 // Load data from delay line
            madd.s  f4, f0, f8 // f4 += coeffs[c] * delay[d]
            madd.s  f5, f1, f9 // f5 += coeffs[c+1] * delay[d+1]
            madd.s  f6, f2, f10
            madd.s  f7, f3, f11
        .first_fir_loop_0:
        
        // Reset delay line pointer
        l32i  a15, a2, 4  // a15 - delay line [0]	
        
        // a14 = pos / 4
        srli  a14, a7, 2
        loopnez  a14, .second_fir_loop_0 // i in [0,pos[ (stride of 4) ; c in [N-pos,N[ (stride of 4)
            EE.LDF.128.IP f3, f2, f1, f0, a10, 16 // Load coeffs
            EE.LDF.128.IP f11, f10, f9, f8, a15, 16 // Load data from delay line
            madd.s  f4, f0, f8 // f4 += coeffs[c] * delay[d]
            madd.s  f5, f1, f9 // f5 += coeffs[c+1] * delay[d+1]
            madd.s  f6, f2, f10
            madd.s  f7, f3, f11
        .second_fir_loop_0:
        j    .store_fir_result;

.offset_1:
        // a14 = (N - pos + 3) / 4
        sub   a14, a6, a7   // a14 = N-pos
        addi  a14, a14, 3
        srli  a14, a14, 2

        const.s f3, 0 // f3 = 0
        EE.LDF.128.IP f11, f10, f9, f12, a15, 16 // Load data from delay line

        // f12 - delay[N-1], store for the last operation
        // f9..f11 - delay[0..2]
        loopnez  a14, .first_fir_loop_1 // pos...N-1
            madd.s  f4, f3, f8 // multiplies f8 by 0 in the first iteration

            EE.LDF.128.IP f3, f2, f1, f0, a10, 16 // Load coeffs
            madd.s  f5, f0, f9
            madd.s  f6, f1, f10
            madd.s  f7, f2, f11
            EE.LDF.128.IP f11, f10, f9, f8, a15, 16 // Load data from delay line (out of bounds access on last iteration! - delay[N], delay[N+1], delay[N+2], delay[N+3])
        .first_fir_loop_1:
        
        // Reset delay line pointer
        l32i  a15, a2, 4  // a15 - delay line [0]

        // a14 = pos / 4
        srli  a14, a7, 2
        loopnez  a14, .second_fir_loop_1 // 0..pos
            EE.LDF.128.IP f11, f10, f9, f8, a15, 16 // Load data from delay line
            madd.s  f4, f3, f8

            EE.LDF.128.IP f3, f2, f1, f0, a10, 16 // Load coeffs
            madd.s  f5, f0, f9
            madd.s  f6, f1, f10
            madd.s  f7, f2, f11
        .second_fir_loop_1:

        // Both loops together evaluate to N/4 iterations (N madds)

        madd.s  f4, f3, f12
        j    .store_fir_result;

.offset_2:
        // a14 = (N - pos + 3) / 4
        sub   a14, a6, a7   // a14 = N-pos
        addi  a14, a14, 3
        srli  a14, a14, 2

        const.s f2, 0 // f2 = 0
        const.s f3, 0 // f3 = 0
        EE.LDF.128.IP f11, f10, f13, f12, a15, 16 // Load data from delay line

        // f12, f13 - delay[N-1], delay[N-2], store for the last operation
        // f10..f11 - delay[0..1]
        loopnez  a14, .first_fir_loop_2 // pos...N-1
            madd.s  f4, f2, f8 // multiplies f8 by 0 in the first iteration
            madd.s  f5, f3, f9 // multiplies f9 by 0 in the first iteration
            
            EE.LDF.128.IP f3, f2, f1, f0, a10, 16 // Load coeffs
            madd.s  f6, f0, f10
            madd.s  f7, f1, f11
            EE.LDF.128.IP f11, f10, f9, f8, a15, 16 // Load data from delay line (out of bounds access on last iteration! - delay[N], delay[N+1], delay[N+2], delay[N+3])
        .first_fir_loop_2:
        
        // Reset delay line pointer
        l32i  a15, a2, 4  // a11 - delay line [0]

        srli  a14, a7, 2
        loopnez  a14, .second_fir_loop_2 // 0..pos
            EE.LDF.128.IP f11, f10, f9, f8, a15, 16 // Load data from delay line
            madd.s  f4, f2, f8
            madd.s  f5, f3, f9

            EE.LDF.128.IP f3, f2, f1, f0, a10, 16 // Load coeffs
            madd.s  f6, f0, f10
            madd.s  f7, f1, f11
        .second_fir_loop_2:

        // Both loops together evaluate to N/4 iterations (N madds)

        madd.s  f4, f2, f12
        madd.s  f5, f3, f13
        j    .store_fir_result;

.offset_3:
        // a14 = (N - pos + 3) / 4
        sub   a14, a6, a7   // a14 = N-pos
        addi  a14, a14, 3
        srli  a14, a14, 2

        const.s f1, 0 // f1 = 0
        const.s f2, 0 // f2 = 0
        const.s f3, 0 // f3 = 0
        EE.LDF.128.IP f11, f14, f13, f12, a15, 16 // Load data from delay line

        // f12, f13, f14 - delay[N-1], delay[N-2], delay[N-3], store for the last operation
        // f11 - delay[0]
        loopnez  a14, .first_fir_loop_3 // pos...N-1
            madd.s  f4, f1, f8
            madd.s  f5, f2, f9
            madd.s  f6, f3, f10

            EE.LDF.128.IP f3, f2, f1, f0, a10, 16 // Load coeffs
            madd.s  f7, f0, f11
            EE.LDF.128.IP f11, f10, f9, f8, a15, 16 // Load data from delay line (out of bounds access on last iteration! - delay[N], delay[N+1], delay[N+2], delay[N+3])
        .first_fir_loop_3:
        
        // Reset delay line pointer
        l32i  a15, a2, 4  // a11 - delay line [0]

        srli  a14, a7, 2
        loopnez  a14, .second_fir_loop_3 // 0..pos
            EE.LDF.128.IP f11, f10, f9, f8, a15, 16 // Load data from delay line
            madd.s  f4, f1, f8
            madd.s  f5, f2, f9
            madd.s  f6, f3, f10

            EE.LDF.128.IP f3, f2, f1, f0, a10, 16 // Load coeffs
            madd.s  f7, f0, f11
        .second_fir_loop_3:

        // Both loops together evaluate to N/4 iterations (N madds)

        madd.s  f4, f1, f12
        madd.s  f5, f2, f13
        madd.s  f6, f3, f14

.store_fir_result:

    add.s   f4, f4, f5
    add.s   f6, f6, f7
    add.s   f4, f4, f6

    // Store result
    ssip     f4, a4, 4  // y++ - save result and increment output pointer

    // Check loop break condition
    addi   a5, a5, -1
    bnez    a5, .fird_loop_len

    // Store state
    s32i    a7,  a2, 12 // pos = a7
    mov.n    a2,  a9
    retw.n

#endif // dsps_fir_f32_aes3_enabled
