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

#include "esp_dsp.h"

static const char *TAG = "main";

// This example shows how to use dsls_dotprod_32f and dsls_dotprode_32f functions

#define N_SAMPLES 256
int N = N_SAMPLES;
float input1[N_SAMPLES];
float input2[N_SAMPLES];

void app_main()
{
    esp_err_t ret;
    ESP_LOGI(TAG, "Start Example.");

    // The example will calculate n!
    //Initialize an input arrays
    for (int i=0 ; i< N ; i++)
    {
        input1[i] = 1;
        input2[i] = i;
    }
    float result1 = 0;
    ret = dsls_dotprod_32f_ae32(input1, input2, &result1, 101);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Operation error = %i", ret);
    }
    ESP_LOGI(TAG, "The (101)! = %f", result1);

    ESP_LOGI(TAG, "End Example.");
}
