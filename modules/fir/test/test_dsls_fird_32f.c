#include <string.h>
#include "unity.h"
#include "test_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portable.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_clk.h"
#include "soc/cpu.h"
#include "esp_log.h"

#include "dsls_tone_gen.h"
#include "dsls_d_gen.h"
#include "dsls_fir.h"

static const char *TAG = "dsls_fird_32f_ansi";

float x[1024];
float y[1024];

float coeffs[32];
float delay[32];

TEST_CASE("dsls_fird_32f_ansi functionality", "[dsls]")
{
    // In the test we generate filter with cutt off frequency 0.1
    // and then filtering 0.1 and 0.3 frequencis.
    // Result must be better then 24 dB
    int len = sizeof(x)/sizeof(float);
    int fir_len = sizeof(coeffs)/sizeof(float);
    int decim = 4;
    
    fir_32f_t fir1;
    for (int i=0 ; i< fir_len ; i++)
    {
        coeffs[i] = 0;
    }
    coeffs[0] = 1;

    for (int i=0 ; i< len ; i++)
    {
        x[i] = i%decim;
    }

    dsls_fird_init_32f(&fir1, coeffs, delay, fir_len, decim);
    int total = dsls_fird_32f_ansi(&fir1, x, y, len);
    ESP_LOGI(TAG,"Total result = %i from %i", total, len);
    TEST_ASSERT_EQUAL(total, len/decim);
    // for (int i=0 ; i< total ; i++)
    // {
    //     printf("data[%i] = %f\n", i, y[i]);
    // }    
    for (int i=0 ; i< total ; i++)
    {
        if (y[i] != (decim-1))
        {
            TEST_ASSERT_EQUAL(y[i], (decim-1));
        }
    }
    

}

