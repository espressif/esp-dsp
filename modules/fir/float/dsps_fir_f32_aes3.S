// Copyright 2018-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License. 

#include "dsps_fir_platform.h"
//#if (dsps_fir_f32_aes3_enabled == 1)

#include "dsps_dotprod_f32_m_ae32.S"

// This is FIR filter for ESP32 processor.
	.text
	.align  4
	.global dsps_fir_f32_aes3
	.type   dsps_fir_f32_aes3,@function
// The function implements the following C code:
//esp_err_t dsps_fir_f32_aes3(fir_f32_t* fir, const float* input, float* output, int len);

dsps_fir_f32_aes3: 
// fir      - a2
// input    - a3
// output   - a4
// len      - a5

	entry	a1, 16
	// Array increment for floating point data should be 4
	l32i    a7,  a2, 12 // a7  - pos
	movi    a10, 4
	mull    a13, a7, a10// a13 - a7*4
	l32i    a6,  a2, 8  // a6  - N
	mull    a6, a6, a10// a6 = a6*4
	l32i    a10, a2, 0  // a10 - coeffs
	add     a10, a10, a6 // a10 = &coeffs[N];
	addi    a10, a10, -4 // a10 = &coeffs[N-1];
	l32i    a11, a2, 4  // a11 - delay line
	l32i    a6,  a2, 8  // a6  - N
	movi.n	a9, 0

//  a13 - delay index
.fir_loop_len:
		// Store to delay line
		lsi     f0, a3, 0       // f0 = x[i]
		addi    a3, a3, 4       // x++
		ssx     f0, a11, a13    // delay[a13] = f0;
		addi    a13, a13, 4     // a13++
		addi    a7, a7, 1       // a7++
		// verify deley line
		blt     a7, a6, .do_not_reset_a13
			movi    a13, 0
			movi    a7,  0
	.do_not_reset_a13:
		// Calc amount for delay line before end
		mov     a15, a10        // a15 - coeffs
		wfr	    f2, a9 // f2 = 0;
		sub   a14, a6, a7   // a14 = N-pos
		loopnez  a14, .first_fir_loop // pos...N-1
//			lsi     f1, a15, 0
//			addi    a15, a15, -4    // a15--
//			lsx     f0, a11, a13    // load delay f0 = delay[pos]
//			addi    a13, a13, 4     // a13++, pos++
			madd.s  f2, f0, f1      // f2 += f0*f1
			madd.s  f3, f3, f4      // f2 += f0*f1
			madd.s  f5, f5, f6      // f2 += f0*f1
			madd.s  f7, f7, f8      // f2 += f0*f1
			//nop
.first_fir_loop:
		movi a13, 0    // load delay line counter to 0
		loopnez  a7, .second_fir_loop // 0..pos
//			lsi     f1, a15, 0
//			addi    a15, a15, -4    // a15--
//			lsx     f0, a11, a13    // load delay f0 = delay[pos]
//			addi    a13, a13, 4     // a13++, pos++
			madd.s  f2, f0, f1      // f2 += f0*f1
			madd.s  f3, f3, f4      // f2 += f0*f1
			madd.s  f5, f5, f6      // f2 += f0*f1
			madd.s  f7, f7, f8      // f2 += f0*f1
			//nop
.second_fir_loop:

		// and after end
		// Store result
		ssi     f2, a4, 0
		//s32i    a13, a4, 0// just for debug *a4 = a13
		addi    a4, a4, 4 // y++ - increment output pointer
		// Check loop 
		addi   a5, a5, -1
	bnez    a5, .fir_loop_len
	// store state

	s32i    a7,  a2, 12 // pos = a7
	movi.n	a2, 0 // return status ESP_OK
	retw.n

//#endif // dsps_fir_f32_aes3_enabled