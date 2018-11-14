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

#include <string.h>
#include "unity.h"
#include "test_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portable.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_clk.h"
#include "soc/cpu.h"

#include "dslm_mult.h"
#include "esp_attr.h"

// Test dsls_dotprod_16s_ansi function
TEST_CASE("dslm_mult_32f_ae32 functionality", "[dslm]")
{
	int m = 4;
	int n = 3;
	int k = 4;


	float A[m][n];
	float* A_ptr = (float*)A;

	float B[n][k];
	float* B_ptr = (float*)B;

	float C[m][k];
	float* C_ptr = (float*)C;
	float C_compare[m][k];
	float* Cc_ptr = (float*)C_compare;

	for (int i=0 ; i< m*n; i++)
	{
		A_ptr[i] = i;
		B_ptr[i] = i;
	}
	for (int i=0 ; i< m ; i++)
	{
		for (int j=0 ; j< k ; j++)
		{
			C_compare[i][j] = 0;
			for (int s=0 ; s< n ; s++)
			{
				C_compare[i][j] += A[i][s]*B[s][j];
			}
		}
	}
	dslm_mult_32f_ae32(A_ptr, B_ptr, C_ptr, m, n, k);

	// for (int i=0 ; i< m ; i++)
	// {
	//     for (int j=0 ; j< k ; j++)
	//     {
	//         printf("[%i][%i] calc=%f, expected =%f\n",i,j, C[i][j], C_compare[i][j]);
	//     }
	// }
	// Compare and check results
	for (int i = 0 ; i< m*k ; i++)
	{
		if (Cc_ptr[i] != C_ptr[i])
		{
			TEST_ASSERT_EQUAL( C_ptr[i], Cc_ptr[i]);
		}
	}
}

static portMUX_TYPE testnlock = portMUX_INITIALIZER_UNLOCKED;

TEST_CASE("dslm_mult_32f_ae32 benchmark", "[dslm]")
{
	int m = 4;
	int n = 4;
	int k = 4;

	float A[m][n];
	float* A_ptr = (float*)A;

	float B[n][k];
	float* B_ptr = (float*)B;

	float C[m][k];
	float* C_ptr = (float*)C;


	portENTER_CRITICAL(&testnlock);

	unsigned int start_b = xthal_get_ccount();
	int repeat_count = 1024;
	for (int i=0 ; i< repeat_count ; i++)
	{
		dslm_mult_32f_ae32(A_ptr, B_ptr, C_ptr, m, n, k);
	}
	unsigned int end_b = xthal_get_ccount();
	portEXIT_CRITICAL(&testnlock);

	float total_b = end_b - start_b;
	float cycles = total_b/(repeat_count);
	printf("Benchmark dslm_mult_32f_ae32 - %f per multiplication 4x4 + overhead.\n", cycles);
	float min_exec = 100;
	float max_exec = 700;
	if (cycles >= max_exec) { 
		TEST_ASSERT_MESSAGE (false, "Exec time takes more then expected!");
	}
	if (cycles < min_exec) { 
		TEST_ASSERT_MESSAGE (false, "Exec time takes less then expected!");
	}
}