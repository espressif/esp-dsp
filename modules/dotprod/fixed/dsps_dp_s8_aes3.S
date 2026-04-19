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

#include "dsps_dotprod_platform.h"
#if (dsps_dotprod_s16_aes3_enabled == 1)
#include "dsp_err_codes.h"

	.text
	.align  4
	.global dsps_dp_s8_aes3
	.type   dsps_dp_s8_aes3,@function


//esp_err_t dsps_dp_s8_aes3(const int8_t *src1, const int8_t *src2, int32_t *dest, int len);
dsps_dp_s8_aes3: 
// src1 - a2
// src2 - a3
// dest - a4
// len  - a5

	entry	a1, 48

	// Check minimum length
	movi a8, 4
	blt a5, a8, .dsps_dp_s8_aes3_error

	// len must be a multiple of 16 for the vectorized path
	extui	a8, a5, 0, 4
	bnez.n	a8, .dsps_dp_s8_aes3_via_ansi
	/* DEBUG: всегда ANSI; убрать перед релизом */

    movi.n	a14,0
    wur.accx_0	a14
    wur.accx_1	a14
    ee.vld.128.ip	q0,a2,16
    ee.vld.128.ip	q1,a3,16

	srli	a5, a5, 4

    loopnez	a5,.loop_dsps_dp_s8_aes3  
	    ee.vmulas.s8.accx.ld.ip	    q1,a3,16, q0,q1
.loop_dsps_dp_s8_aes3:		ee.vld.128.ip	            q0,a2,16
 

    rur.accx_0	a14
	s32i	a14, a4, 0

	movi.n	a2, 0
	retw.n

.dsps_dp_s8_aes3_error:
	movi.n	a2, ESP_ERR_DSP_INVALID_LENGTH
	retw.n

.dsps_dp_s8_aes3_via_ansi:
	mov.n	a10, a2
	mov.n	a11, a3
	mov.n	a12, a4
	mov.n	a13, a5
	.type	dsps_dp_s8_ansi, @function
	call8	dsps_dp_s8_ansi
	movi.n	a2, 0
	retw.n

#endif // dsps_dotprod_s16_ae32_enabled
