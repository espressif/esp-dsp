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
#include "dsls_dotprode_32f_m_ae32.S"

// This is matrix multipliction function for Esp32 processor.

	.text
	.align  4
	.global dsls_fft2r_32fc_ae32
	.type   dsls_fft2r_32fc_ae32,@function

    //.extern dsls_fft_w_table_32fc;
    .global dsls_fft_w_table_32fc;

// The function implements next C code:
//esp_err_t dsls_fft2r_32fc_ansi(float *input, int N)
//{
//    float *w = dsls_fft_w_table_32fc;
//
//    int ie, ia, m;
//    float re_temp, im_temp;
//    float c, s;
//    int N2 = N;
//    ie = 1;
//    for (int N2 = N/2; N2 > 0; N2 >>= 1) {
//        ia = 0;
//        for (int j = 0; j < ie; j++) {
//            c = w[2 * j];
//            s = w[2 * j + 1];
//            for (int i = 0; i < N2; i++) {
//                m = ia + N2;
//                re_temp = c * input[2 * m] + s * input[2 * m + 1];
//                im_temp = c * input[2 * m + 1] - s * input[2 * m];
//                input[2 * m] = input[2 * ia] - re_temp;
//                input[2 * m + 1] = input[2 * ia + 1] - im_temp;
//                input[2 * ia] = input[2 * ia] + re_temp;
//                input[2 * ia + 1] = input[2 * ia + 1] + im_temp;
//                ia++;
//            }
//            ia += N2;
//        }
//        ie <<= 1;
//    }
//    return result;
//}


dsls_fft2r_32fc_ae32: 
//esp_err_t dsls_fft2r_32fc_ansi(float *input, int N, float* dsls_fft_w_table_32fc)

	entry	a1, 32
	// Array increment for floating point data should be 4
// input - a2
// N - a3
// dsls_fft_w_table_32fc - a4 - for now

// a6 - k, main loop counter; N2 - for (int N2 = N/2; N2 > 0; N2 >>= 1)
// a7 - ie
// a8 - j
// a9 - test
// a10 - (j*2)<<2,  or a10 - j<<3
// f0 - c or w[2 * j]
// f1 - s or w[2 * j + 1]
// a11 - ia
// a12 - m
// a13 - ia pointer
// a14 - m pointer
// f6  - re_temp
// f7  - im_temp

// a15 - debug

    // This instruction are not working. Have to be fixed!!!
    #l32r    a4, dsls_fft_w_table_32fc
    
    // Load shift register with 1
    movi.n  a5, 1   // a5 = 1;
    ssr a5          // load shift register with 1
    
    srli a6, a3, 1 // a6 = N2 = N/2
    movi a7, 1     // a7 - ie

fft2r_l1: 
    movi a8, 0     // a8 - j
    movi a11,0     // a11 = ia = 0;

fft2r_l2:           // loop for j, a8 - j
        slli    a10, a8, 3 // a10 = j<<3 // shift for cos ()   -- c = w[2 * j];
        add.n   a10, a10, a4 // a10 - pointer to cos
        lsi     f0, a10, 0   
        lsi     f1, a10, 4

        movi a9, 0 // just for debug
        loopnez a6, fft2r_l3
            add.n    a12, a11, a6   // a12 = m = ia + N2

            slli     a14, a12, 3    // a14 - pointer for m*2
            slli     a13, a11, 3    // a13 - pointer for ia*2
            add.n    a14, a14, a2   // pointers to input arrays
            add.n    a13, a13, a2   //

            lsi      f4, a14, 0     // input[2 * m]
            mul.s    f6, f0, f4     // re_temp =  c * input[2 * m]
            lsi      f5, a14, 4     // input[2 * m + 1]
            mul.s    f7, f0, f5     // im_temp =  c * input[2 * m + 1]

            lsi      f2, a13, 0     // input[2 * ia] 
            madd.s   f6, f1, f5     // re_temp += s * input[2 * m + 1];
            lsi      f3, a13, 4     // input[2 * ia + 1]
            msub.s   f7, f1, f4     // im_temp -= s * input[2 * m];
            
            sub.s    f8, f2, f6     // = input[2 * ia] - re_temp;
            sub.s    f9, f3, f7     // = input[2 * ia + 1] - im_temp;

            add.s    f10, f2, f6    // = input[2 * ia] + re_temp;
            ssi      f8, a14, 0 
            add.s    f11, f3, f7    // = input[2 * ia + 1] + im_temp;            
            ssi      f9, a14, 4 

            ssi      f10, a13, 0 
            addi     a11, a11, 1// ia++
            ssi      f11, a13, 4 
fft2r_l3:
        add     a11, a11, a6

        addi    a8, a8, 1     // j++
        BNE     a8, a7, fft2r_l2 // 
    slli    a7, a7, 1  // ie = ie<<1
// main loop: for (int k = N/2; k > 0; k >>= 1)
    srli    a6, a6, 1  // a6 = a6>>1
    BNEZ    a6, fft2r_l1// Jump if > 0

	movi.n	a2, 0 // return status ESP_OK
	retw.n
