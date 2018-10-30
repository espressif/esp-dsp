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

int16_t x[1024];
int16_t y[1024];
int16_t z[1024];

void app_main()
{
    esp_err_t status;
    for (int i=0 ; i< 1024 ; i++)
    {
        x[i] = 0x0100;
        y[i] = 0x0100;
        z[i] = 0;
    }
    for (int i=0 ; i< 100 ; i++)
    {
        status = dslm_mult_16s_ansi(x, y, z, 4, 4, 1, 0);
        status = dsls_dotprod_16s_ae32(x, y, z, 1021, 1);
        status = dsls_dotprod_16s_ae32(x, y, z, 1022, 1);
        status = dsls_dotprod_16s_ae32(x, y, z, 1023, 1);
        printf("status= %i\n", status);
    }
}
