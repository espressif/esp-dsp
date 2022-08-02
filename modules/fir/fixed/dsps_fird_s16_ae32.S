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
#if (dsps_fird_s16_ae32_enabled == 1)

#include "dsps_fir_s16_m_ae32.S"
 
// This is FIR filter for ESP32 processor.
	.text
	.align  4
	.global dsps_fird_s16_ae32
	.type   dsps_fird_s16_ae32,@function
// The function implements the following C code:
//int32_t dsps_fird_s16_ansi(fir_s16_t *fir, const int16_t *input, int16_t *output, int32_t len)

dsps_fird_s16_ae32: 
// Input params					Variables
//
// fir      - a2				N			- a6
// input    - a3				pos			- a7
// output   - a4				res			- a8
// len      - a5				d_pos		- a9
//								&coeffs[N]	- a10
//								delay		- a11
//								decim		- a12
//								delay index - a13	
//								shift		- a14

	entry    a1,   16										
	l16si    a7,   a2,   10			// a7  - pos			-- Load to a7 (pos) from a2 (fir struct), offset 10 bits
	movi.n   a10,  2				//						-- a10 = 2			
	mul16s   a13,  a7,   a10       	// a13 - a7*2			-- 16-bit singed mul of 2 (2 byte varialbe) and position = delay index
	l16si    a6,   a2,   8         	// a6  - N				-- Load to a6 (N) from a2 (fir), offset 8 bits
	mul16s   a6,   a6,   a10       	// a6  - a6*2			-- 16-bit singed mul of 2 (2 byte varialbe) and N, save to N
	l32i	 a10,  a2,   0         	// a10 - coeffs			-- 32-bit load to a10 (coeffs) from a2 (fir), offset 0 bits
	add.n    a10,  a10,  a6       	// a10 - &coeffs[N];	-- 16-bit add of N and coeffs, result as 32bit
	addi     a10,  a10,  -2       	// a10 - &coeffs[N-1];	-- Add a10 (&coeffs[N]) and immediate (-2) = result in a10
	l32i     a11,  a2,   4         	// a11 - delay line		-- Offset 4 bits
	l16si    a6,   a2,   8         	// a6  - N				-- Offset 8 bits
	l16si    a12,  a2,   12        	// a12 - decimation		-- Offset 12 bits
	l16si    a9,   a2,   14        	// a9  - d_pos			-- Offset 14 bits
	l16si    a14,  a2,   16			// a14 - shift		    -- Offset 16 bits
	movi.n   a8,   0              	// result = 0

	add.n	 a11,  a11,  a13		// set initial position of the delay line - delay[delay_index] - a11[a13]
	

fird_loop_len:

	l16si	 a15,  a3,   0			// a11[a7] = a3[i]		-- delay[pos] = input[i]
	s16i	 a15,  a11,  0			// save
	addi	 a3,   a3,   2			// i++					-- increment pointer
	addi 	 a13,  a13,  2			// a13++				-- delay index++
	addi 	 a7,   a7,   1			// a7++					-- position++

	// check if position of delay line is lower than length of the delay line N
	blt	     a7,   a6,   do_not_reset_delay_index
	
		movi.n	 a13,  0			//						-- reset delay_index
		movi.n 	 a7,   0			//						-- reset pos
		l32i     a11,  a2,   4		//						-- pointer of the dealy line to the beggining
		addi	 a11,  a11,  -2
	do_not_reset_delay_index: 	

	// decimation
	addi 	 a9,  a9,   1			// a9++					-- d_pos++
	
	// checko d_pos vs decimation
	blt		 a9,  a12,  next_iter

		addi 	 a8,   a8,   1		// a8++					-- result++
		movi.n 	 a9,   0			// a9 = 0				-- reset d_pos

		// prepare MAC unit
		wsr      a9,   acchi		// acchi = 0			-- Accumulator High = 0
		ssr	     a14				// SAR registers = shift		
		movi     a9,   0x7fff		// rounding value
		srl	     a9,   a9			// shift right logical
		wsr      a9,   acclo		// acclo = 07fff 		-- Accumulator Low = rounding value
		
		// calculate position of the
		sub      a9,   a6, a7   	// a9 = full_count = N(a6) - pos(a7)
		mov      a15,  a9			// a15 = count

		// (Count / 4) - 1
		srli     a15,  a15,   2		// Shift right = divide by 4
		addi     a15,  a15,  -1		// minus 1

		addi     a11,  a11,  -2 	// To arrange fist pointer, samples (array is being incremented)
		addi     a10,  a10,   2 	// To arrange fist pointer, coeffs  (array is being decremented)
		
		// MAC16 macro
		// x1, x2, count, full_count, ID
		fir_s16_ae32_full a11, a10, a15, a9, __LINE__
		l32i 	 a11,  a2,    4		// reset pointer to the beggining of the array

		mov      a9,   a7			// a7 = full_count = pos
		srli     a15,  a7,    2		// a15 = count
		addi     a15,  a15,  -1
		
		addi     a11,  a11,  -4 	// To arrange fist pointer, samples (array is being incremented)

		// MAC16 macro
		// x1, x2, count, full_count, ID
		fir_s16_ae32_full a11, a10, a15, a7, __LINE__

		// get the shifted value
		mov      a9,   a14		// a9 = shift	
		neg	     a9,   a9		// negate
		addi     a9,   a9,  15			
		ssr 	 a9
		rsr		 a9,   acchi	// Get 16 bit result from 40 bit accumulator
		rsr		 a15,  acclo
		src 	 a9,   a9,  a15
		s16i	 a9,   a4,  0	// Save to a4 (output)
		addi 	 a4,   a4,  2	// a4++					-- output++

		// reset coeffcient pointer
		l32i	 a10,  a2,  0
		movi	 a15,  2
		mul16s   a15,  a15,  a6
		add.n	 a10,  a10,  a15
		addi     a10,  a10,  -2

		movi	 a9,   0		// a9 back to zero

	next_iter:
		addi     a5,  a5,   -1	// a5--					-- len-- 
		
		l32i     a11,  a2, 4	// Load initial position of the delay line
		add 	 a11,  a11, a13 // add content of pos to the pointer
	bnez a5, fird_loop_len		// break if a5 == 0

	mov 	a2,  a8 			// return length of the result array
	retw.n

#endif // dsps_fird_f32_ae32_enabled







