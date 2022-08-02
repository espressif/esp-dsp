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
#include "dsps_fir_s16_m_ae32.S"
#if (dsps_fird_s16_ae32_mul_enabled == 1)

//#include "dsps_dotprod_f32_m_ae32.S"

// This is FIR filter for ESP32 processor.
	.text
	.align  4
	.global dsps_fird_s16_mul
	.type   dsps_fird_s16_mul,@function
// The function implements the following C code:
//int32_t dpss_fird_s16_mul(int16_t *input1, int16_t *input2, int16_t count, int16_t full_count, int16_t shift);

dsps_fird_s16_mul:
// input1       - a2
// input2       - a3
// fir_pos		- a4
// full_count   - a5
// shift        - a6
// This macro calculates fixed point dot product for ((count + 1)*4) int16 samples
// x1 - input array1 register (for example a2)
// x2 - input array2 register (for example a3)
// count - counter register (for example a7)
// count -   samples_count / 4 - 1
// acc += x1[i + 0]*x2[N - i - 1] + x1[i + 1]*x2[N - i - 2] + x1[i + 2]*x2[N - i - 3] + x1[i + 3]*x2[N - i - 4]; i: 0..count
// acchi, and acclo have to be initialize before
// Result - acchi || acclo
// Modifies: 
// m0, m1, m2, m3
// acchi || acclo - must be loaded before (for example 0x3fff to acclo). 

        entry a1, 16

        movi a8, 0
        wsr  a8, acchi

        movi a8, 0x7fff
        ssr  a6
        srl  a8, a8
        wsr  a8, acclo

        neg  a6, a6
        addi a6, a6, 15

		/*
		 * Data schedule. Each line represents instruction, columns represent
		 * register contents. Last column (MUL) shows the multiplication which
		 * takes place. Values loaded in the given cycle are shown in square brackets.
		 *
		 *  m0     m1         m2              m3          MUL
		 * -----------------  pre-load  --------------------------
		 *[x0 x1]								                  (no MULs in the first 3 instructions)
		 * x0 x1        [y(N-1) y(N-2)]
		 * x0 x1 [x2 x3] y(N-1) y(N-2)
		 * x0 x1  x2 x3  y(N-1) y(N-2) [y(N-3) y(N-4)] x0*y(N-1)
		 * --------------------   loop  ------------------------	 (the following 4 instructions are
		 *[x4 x5] x2 x3  y(N-1) y(N-2)  y(N-3) y(N-4)  x1*y(N-2)     repeated as much as needed)
		 * x4 x5  x2 x3 [y(N-5) y(M-6)] y(N-3) y(N-4)  x2*y(N-3)
		 * x4 x5 [x6 x7] y(N-5) y(M-6)  y(N-3) y(N-4)  x3*y(N-4)
		 * x4 x5  x6 x7  y(N-5) y(M-6) [y(N-7) y(M-8)] x4*y(N-5)
		 * -------------------  finalize  ----------------------
		 * x4 x5  x6 x7  y(N-5) y(M-6)  y(N-7) y(M-8)  x5*y(N-6)	(nothing is load)
		 * x4 x5  x6 x7  y(N-5) y(M-6)  y(N-7) y(M-8)  x6*y(N-7)
		 * x4 x5  x6 x7  y(N-5) y(M-6)  y(N-7) y(M-8)  x7*y(N-8)
		 */

		movi    a7, 2			// 2
		mul16s  a7, a7, a5		// full_count*2
		add.n   a3, a3, a7		// &input2[full_count]
		addi    a3, a3, -4		// &input2[full_count - 1]

		movi 	a7, 2
		mul16s  a7, a7, a4      // fir_pos * 2
		add.n	a2, a2, a7		// initial position of input samples

		sub 	a9, a5, a4		// count = full_count - fir_pos
		mov	    a15, a9			// count
		srli    a9, a9, 2		// count / 4 -1
		addi    a9, a9, -1

///////////////////////////////////////////////////////////////////////////////////////////////////
		
		addi  a2, a2, -4 // To arrange fist pointer
		addi  a3, a3, 4  // To arrange fist pointer

		ldinc m0, a2
		lddec m2, a3
		ldinc m1, a2
	
		mula.dd.lh.lddec m3, a3, m0, m2
		loopnez a9, .loop_end
		.loop:
			mula.dd.hl.ldinc m0, a2, m0, m2
			mula.dd.lh.lddec m2, a3, m1, m3
			mula.dd.hl.ldinc m1, a2, m1, m3
			mula.dd.lh.lddec m3, a3, m0, m2
		.loop_end:
	
		mula.dd.hl m0, m2
		mula.dd.lh m1, m3
		mula.dd.hl m1, m3


        bbci  a15, 1, .mod2chk
		    ldinc m0, a2
		    lddec m2, a3
		    mula.dd.hl m0, m2
		    mula.dd.lh m0, m2
	    .mod2chk:

		bbci  a15, 0, .mod1chk
		    ldinc m0, a2
		    lddec m2, a3
		    mula.dd.lh m0, m2
	    .mod1chk:

		movi a7, 2
		mul16s a7, a7, a5
		addi a7, a7, -4
		sub a2, a2, a7		// reset pointer to the beginning of the array

		srli   a9, a4, 2
		addi   a9, a9, -1


///////////////////////////////////////////////////////////////////////////////////////////////////
		
		addi  a2, a2, -4 // To arrange fist pointer
		//addi  a3, a3, 4  // To arrange fist pointer

		blti a4, 4, .lees_than_4

		ldinc m0, a2
		lddec m2, a3
		ldinc m1, a2
		mula.dd.lh.lddec m3, a3, m0, m2
		loopnez a9, .loop1_end
		.loop1:
			mula.dd.hl.ldinc m0, a2, m0, m2
			mula.dd.lh.lddec m2, a3, m1, m3
			mula.dd.hl.ldinc m1, a2, m1, m3
			mula.dd.lh.lddec m3, a3, m0, m2
		.loop1_end:
	
		mula.dd.hl m0, m2
		mula.dd.lh m1, m3
		mula.dd.hl m1, m3

		.lees_than_4:

        bbci  a4, 1, .mod2chk1
		    ldinc m0, a2
		    lddec m2, a3
		    mula.dd.hl m0, m2
		    mula.dd.lh m0, m2
	    .mod2chk1:

		bbci  a4, 0, .mod1chk1
		    ldinc m0, a2
		    lddec m2, a3
		    mula.dd.lh m0, m2
	    .mod1chk1:

		srli   a9, a4, 2
		addi   a9, a9, -1


        ssr a6
    	rsr a2, acchi
    	rsr a3, acclo
    	src a2, a2, a3
		
/*
.macro dotprod_s16_ae32_full x1, x2, count, full_count
// This macro calculates fixed point dot product for ((count + 1)*4) int16 samples
// x1 - input array1 register (for example a2)
// x2 - input array2 register (for example a3)
// count - counter register (for example a7)
// count -   samples_count / 4 - 1
// full_count - samples_count
// acc += x1[i + 0]*x2[N - i - 1] + x1[i + 1]*x2[N - i - 2] + x1[i + 2]*x2[N - i - 3] + x1[i + 3]*x2[N - i - 4]; i: 0..count
// acchi, and acclo have to be initialize before
// Result - acchi || acclo
// Modifies: 
// m0, m1, m2, m3
// acchi || acclo - must be loaded before (for example 0x3fff to acclo). 

		dotprod_s16_ae32 \x1, \x2, \count

		bbci  \full_count, 1, .mod2chk
		ldinc m0, \x1
		ldinc m2, \x2
		mula.dd.hh m0, m2
		mula.dd.ll m0, m2
	.mod2chk:
		bbci  \full_count, 0, .mod1chk
		ldinc m0, \x1
		ldinc m2, \x2
		mula.dd.ll m0, m2
	.mod1chk:

.endm // dotprod_s16_ae32_full
*/

#endif