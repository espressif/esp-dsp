/* DSL example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"


#include "dsl_err.h"
#include "dsls_dotprod.h"
#include "dslm_mult.h"


void app_main()
{
    esp_err_t status;

    const int m = 4;
    const int n = 4; 
    const int k = 2;

    int16_t A[m][n];
    int16_t B[n][k];
    int16_t C[m][k];
    int16_t C_ref[m][k];
    for (int i=0 ; i< m ; i++)
    {
        for (int j=0 ; j< n ; j++)
        {
            A[i][j] = (i+1)*0x100;
        }
    }
    for (int i=0 ; i< n ; i++)
    {
        for (int j=0 ; j< k ; j++)
        {
            B[i][j] = (j+1)*0x100;
        }
    }

    for (int s=0 ; s< 100 ; s++)
    {
        unsigned int start_b = xthal_get_ccount();
        status = dslm_mult_16s_ae32((int16_t*)A, (int16_t*)B, (int16_t*)C, m, n, k, 0);
        unsigned int end_b = xthal_get_ccount();
        status = dslm_mult_16s_ansi((int16_t*)A, (int16_t*)B, (int16_t*)C_ref, m, n, k, 0);
        float total_b = end_b - start_b;
        float cycles = total_b;
        printf("Benchmark dslm_mult_32f_ansi - %f per multiplication 4x4 + overhead.\n", cycles);
        for (int i=0 ; i< m ; i++)
        {
            for (int j=0 ; j< k ; j++)
            {
                printf("C[%i][%i] = 0x%04x,   C_ref=0x%04x\n", i,j, C[i][j], C_ref[i][j]);
            }
        }
        // status = dsls_dotprod_16s_ae32(x, y, z, 1021, 1);
        // status = dsls_dotprod_16s_ae32(x, y, z, 1022, 1);
        // status = dsls_dotprod_16s_ae32(x, y, z, 1023, 1);
        // printf("status= %i\n", status);
    }
}
