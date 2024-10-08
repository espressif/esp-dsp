// Copyright 2024 Espressif Systems (Shanghai) PTE LTD
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

#include "dspi_dotprod_platform.h"
#if (dspi_dotprod_arp4_enabled == 1)
#include "dsp_err_codes.h"

    .text
    .align  4
    .global dspi_dotprod_off_u16_arp4
    .global dspi_dotprod_off_u16_ansi
    .type   dspi_dotprod_off_u16_arp4,@function

// esp_err_t dspi_dotprod_off_u16_arp4(image2d_t *in_image, image2d_t *filter, int16_t *out_value, int count_x, int count_y, int shift, unt16_t offset);
dspi_dotprod_off_u16_arp4: 
// in_image     - a0
// filter       - a1
// out_value    - a2
// count_x      - a3
// count_y      - a4
// shift        - a5
// offset       - a6

// i_data       - t0
// f_data       - t1
// i_step       - t2
// f_step       - t3
// t4           - current i_data
// t5           - current f_data

    lw t1, 4(a0) // load  in_image->step_x
    lw t2, 4(a1) // load  filter->step_x
    or t1, t1, t2
    addi t1, t1, -1 // should be 0 now
    andi t2, a3, 7
    or   t1, t1, t2
    
    beqz    t1, .dspi_dotprod_off_u16_arp4_body
    j   dspi_dotprod_off_u16_ansi

.dspi_dotprod_off_u16_arp4_body:
    add sp, sp, -16

    sw  a6, 0(sp)
    mv  t6, sp
    esp.vldbc.16.ip   q2, t6, 0 

    lw  t0, 0(a0)   // i_data
    lw  t1, 0(a1)   // f_data


    lw  t2, 8(a0)   // step_y
    lw  t4, 12(a0)  // stride_x
    mul t2, t4, t2
    slli t2, t2, 1      // i_step = i_step<<1

    lw  t3, 8(a1)       // step_y
    lw  t5, 12(a1)      // stride_x
    mul t3, t5, t3
    slli t3, t3, 1      // f_step = f_step<<1

    srli t6, a3, 3      // t5 = len/8
    

    addi    a7, a5, -1
    li      t4, 1
    sll     t4, t4, a7
    esp.zero.xacc
    esp.movx.w.xacc.l   t4

.loop_count_y:
        mv      t4, t0
        mv      t5, t1
        esp.vld.128.ip      q1, t5, 16          // q0 - i_data

        esp.lp.setup    0, t6, .loop_count_x
            esp.vld.128.ip          q0, t4, 16  // q1 - f_data
            esp.vadd.u16            q3, q2, q1
.loop_count_x:  esp.vmulas.u16.xacc.ld.ip       q1, t5, 16, q0, q3  // q0 - i_data

        add     t0, t0, t2
        add     t1, t1, t3
        add     a4,a4, -1
    bgtz a4, .loop_count_y

    esp.srs.u.xacc       t5, a5 // shift accx register by final_shift amount (a5), save the lower 32bits to t5
    sh  t5, 0(a2)               // store result to output buffer 

    li  a0,0
    add sp,sp,16
    ret

#endif // dspi_dotprod_arp4_enabled