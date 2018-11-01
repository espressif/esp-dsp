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
#include "esp_log.h"

// Test dsls_dotprod_16s_ansi function
TEST_CASE("dslm_mult_16s_ae32 functionality", "[dslm]")
{
    for (int m=1 ; m< 8 ; m++)
    {
        for (int n=1 ; n< 8 ; n++)
        {
            int k = 1;

            int16_t A[m][n];
            int16_t* A_ptr = (int16_t*)A;

            int16_t B[n][k];
            int16_t* B_ptr = (int16_t*)B;

            int16_t C[m][k];
            int16_t* C_ptr = (int16_t*)C;
            int16_t C_compare[m][k];
            int16_t* Cc_ptr = (int16_t*)C_compare;
            for (int shift = -8 ; shift< 8 ; shift++)
            {
               

                //int shift = 2;
                // for (int i=0 ; i< m*n; i++)
                // {
                //     A_ptr[i] = 0x1000;
                //     B_ptr[i] = 0x200;
                // }
                for (int i=0 ; i< m ; i++)
                {
                    for (int j=0 ; j< n; j++)
                    {
                        A[i][j] = 0x1234;
                    }
                }
                for (int i=0 ; i< n ; i++)
                {
                    for (int j=0 ; j< k; j++)
                    {
                        B[i][j] = 0x1234;
                    }
                }

                dslm_mult_16s_ansi(A_ptr, B_ptr, Cc_ptr, m, n, k, shift);
                dslm_mult_16s_ae32(A_ptr, B_ptr, C_ptr,  m, n, k, shift);    

                // for (int i=0 ; i< m ; i++)
                // {
                //     for (int j=0 ; j< k ; j++)
                //     {
                //         printf("[%i][%i] calc=%i, expected =%i\n",i,j, C[i][j], C_compare[i][j]);
                //     }
                // }
                // Compare and check results
                for (int i = 0 ; i< m*k ; i++)
                {
                    if (Cc_ptr[i] != C_ptr[i])
                    {
                        ESP_LOGE("dslm_mult_16s_ae32","Process path m=%i, n=%i, k=%i,  shift=%i\n", m, n, k, shift);
                        TEST_ASSERT_EQUAL(Cc_ptr[i], C_ptr[i]);
                    }
                }
            }
        }
    }
}

static portMUX_TYPE testnlock = portMUX_INITIALIZER_UNLOCKED;

TEST_CASE("DSL check dslm_mult_16s_ae32 benchmark", "[dsl]")
{
    int m = 8*2;
    int n = 8*2;
    int k = 1;

    int16_t A[m][n];
    int16_t* A_ptr = (int16_t*)A;

    int16_t B[m][n];
    int16_t* B_ptr = (int16_t*)B;

    int16_t C[m][k];
    int16_t* C_ptr = (int16_t*)C;

    portENTER_CRITICAL(&testnlock);

    unsigned int start_b = xthal_get_ccount();
    int repeat_count = 1024;
    for (int i=0 ; i< repeat_count ; i++)
    {
        dslm_mult_16s_ae32(A_ptr, B_ptr, C_ptr, m, n, k, 0);
    }
    unsigned int end_b = xthal_get_ccount();
    portEXIT_CRITICAL(&testnlock);

    float total_b = end_b - start_b;
    float cycles = total_b/(repeat_count);
    printf("Benchmark dslm_mult_16s_ae32 - %f per multiplication %ix%ix%i + overhead.\n", cycles, m,n,k);
    // float min_exec = 2000;
    // float max_exec = 3000;
    // if (cycles >= max_exec) { 
    //     TEST_ASSERT_MESSAGE (false, "Exec time takes more then expected!");
    // }
    // if (cycles < min_exec) { 
    //     TEST_ASSERT_MESSAGE (false, "Exec time takes less then expected!");
    // }

}