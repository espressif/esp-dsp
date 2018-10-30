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

//#include "../../dotprod/float/dsls_dotprod_32f_m_ae32.S"
#include "dsls_dotprod_16s_m_ae32.S"
#include "dslm_mult_16s_m_ae32_vector.S"
//esp_err_t dslm_mult_16s_ae32(int16_t* A, int16_t* B, int16_t* C, int m, int n, int k, int shift);

// This is dot product function for Esp32 processor.
    .text
    .align  4
    .global dslm_mult_16s_ae32
    .type   dslm_mult_16s_ae32,@function

dslm_mult_16s_ae32: 
// A - a2
// B - a3
// C - a4
// m - a5 - any > 0
// n - a6 - 1,2,3, any
// k - a7 - 1, any
// shift - stack (a8) 

// a14 - n*4 - pointer increment
//
    entry	a1, 48
// ======     process matrices when k == 1   ============
    l32i.n	a8, a1, 48 // Load shift to the a8 register
    

    // Prepare and load round value
    ssr a8 // store shift to ssa
	movi a15, 0x7fff
    srl a15, a15

	neg  a8, a8 
	addi a8, a8, 15
    ssr a8 // store shift to ssa
	movi a8, 0  // Clear a8 

    slli    a14, a6, 1 // Pointer increment for n
    movi.n	a10, 2 // Increment = 2
    movi.n	a9, 0  // initial counter loop1

    movi     a12, 1
    beq      a7, a12, vector_mult
    // We have normal path with k > 1

    movi.n	a2, 0 // return status ESP_OK
    retw.n

// The path where n > 1
vector_mult:
    dslm_mult_16s_m_ae32_vector;
