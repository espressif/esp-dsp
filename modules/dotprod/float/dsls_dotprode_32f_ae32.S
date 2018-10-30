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

#include "dsls_dotprod_32f_m_ae32.S"

// This is dot product function for Esp32 processor.
    .text
    .align  4
    .global dsls_dotprode_32f_ae32
    .type   dsls_dotprode_32f_ae32,@function
// The function implements next C code:
//esp_err_t dsls_dotprod_32f_ae32(float* src1, float* src2, float* dest, int len)
//{
//    float acc = 0;
//    for (int i=0 ; i< len ; i++)
//    {
//        acc += src1[i]*src2[i];
//    }
//    *dest = acc;
//    return ESP_OK;
//}

dsls_dotprode_32f_ae32: 
// src1 - a2
// src2 - a3
// dest - a4
// len  - a5
// step1- a6
// step2- a7

    entry	a1, 32
    // Array increment for floating point data should be 4
    
//    movi.n	a8, 4
    slli    a6,a6, 2
    slli    a7,a7, 2
    // Clear initial state of the result register
    movi.n	a9, 0
    wfr	    f1, a9
    // a2 - input1
    // a3 - input2
    // a5 - length
    // a6,a7,  step in arrays
    dotprod_32f_ae32 a2, a3, a5, a6, a7;
    
    ssi	f1, a4, 0 // Store result from f1 to memory at a4
    
    movi.n	a2, 0 // return status ESP_OK
    retw.n
