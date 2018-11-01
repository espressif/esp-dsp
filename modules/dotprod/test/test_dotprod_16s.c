#include <string.h>
#include "unity.h"
#include "test_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portable.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_clk.h"
#include "soc/cpu.h"

#include "dsls_dotprod.h"
#include "esp_attr.h"

// Test dsls_dotprod_16s_ansi function
TEST_CASE("dsls_dotprod_16s_ansi functionality", "[dsls]")
{
    int16_t check_value = 1235;
    int max_N = 1024;
    int16_t* x = (int16_t*)malloc(max_N*sizeof(int16_t));
    int16_t* y = (int16_t*)malloc(max_N*sizeof(int16_t));
    int16_t* z = (int16_t*)malloc(max_N*sizeof(int16_t));

    for (int i=0 ; i< max_N ; i++)
    {
        x[i] = 0;
        y[i] = 1000;
    }

    z[0] = check_value;
    z[2] = check_value + 1;

    // Check result == 0
    for (int i = 1; i < 1024; i++)
    {
        esp_err_t status = dsls_dotprod_16s_ansi(x, y, &z[1], i, 0);
        TEST_ASSERT_EQUAL(status, ESP_OK);
        TEST_ASSERT_EQUAL(check_value, z[0]);
        TEST_ASSERT_EQUAL(check_value + 1, z[2]);
        TEST_ASSERT_EQUAL(0, z[1]);
    }

    int16_t val_x = 0x080;
    int16_t val_y = 0x100;
    int16_t val_shift = 0;

    for (int i = 0; i < max_N; i++)
    {
        x[i] = val_x;
        y[i] = val_y;
    }

    // We check that dotproduct working with shift = 0;
    for (int i=1 ; i< 1024 ; i++)
    {
        esp_err_t status = dsls_dotprod_16s_ansi(x,y,&z[1],i,val_shift);

        TEST_ASSERT_EQUAL(status, ESP_OK);
        TEST_ASSERT_EQUAL(check_value, z[0]);
        TEST_ASSERT_EQUAL(check_value + 1, z[2]);
        TEST_ASSERT_EQUAL((i*(val_x*val_y) + (0x7fff>>val_shift))>>(15 - val_shift), z[1]);
    }
    val_shift = 2;
    for (int i=1 ; i< 1024 ; i++)
    {
        esp_err_t status = dsls_dotprod_16s_ansi(x,y,&z[1],i,val_shift);
        
        TEST_ASSERT_EQUAL(status, ESP_OK);
        TEST_ASSERT_EQUAL(check_value, z[0]);
        TEST_ASSERT_EQUAL(check_value + 1, z[2]);
        TEST_ASSERT_EQUAL(((long long)i*((long long)val_x*(long long)val_y) + ((long long)0x7fff>>val_shift))>>(15 - val_shift), z[1]);
    }

    free(x);
    free(y);
    free(z);
}

// Test dsls_dotprod_16s_ansi function
TEST_CASE("dsls_dotprod_16s_ae32 functionality", "[dsls]")
{
    int16_t check_value = 1235;
    int max_N = 1024;
    int16_t* x = (int16_t*)malloc(max_N*sizeof(int16_t));
    int16_t* y = (int16_t*)malloc(max_N*sizeof(int16_t));
    int16_t* z = (int16_t*)malloc(max_N*sizeof(int16_t));

    for (int i=0 ; i< max_N ; i++)
    {
        x[i] = 0;
        y[i] = 1000;
    }
    
    z[0] = check_value;
    z[2] = check_value + 1;

    // Check result == 0
    for (int i=1 ; i< 1024 ; i++)
    {
        esp_err_t status = dsls_dotprod_16s_ae32(x,y,&z[1],i,0);
        if (i < 4)
        {
            TEST_ASSERT_EQUAL(status, ESP_ERR_DSL_INVALID_LENGTH);            
        } else 
        {
            TEST_ASSERT_EQUAL(check_value, z[0]);
            TEST_ASSERT_EQUAL(check_value + 1, z[2]);
            TEST_ASSERT_EQUAL(0, z[1]);
        }
    }

    int16_t val_x = 0x080;
    int16_t val_y = 0x100;
    int16_t val_shift = 0;
    
    for (int i=0 ; i< max_N ; i++)
    {
        x[i] = val_x;
        y[i] = val_y;
    }
    // We check that dotproduct working with shift = 0;
    for (int i=1 ; i< 1024 ; i++)
    {
        esp_err_t status = dsls_dotprod_16s_ae32(x,y,&z[1],i,val_shift);
        if (i < 4)
        {
            TEST_ASSERT_EQUAL(status, ESP_ERR_DSL_INVALID_LENGTH);            
        } else 
        {
            TEST_ASSERT_EQUAL(status, ESP_OK);
            TEST_ASSERT_EQUAL(check_value, z[0]);
            TEST_ASSERT_EQUAL(check_value + 1, z[2]);
            TEST_ASSERT_EQUAL((i*(val_x*val_y) + (0x7fff>>val_shift))>>(15 - val_shift), z[1]);
        }
    }
    val_shift = 2;
    for (int i=1 ; i< 1024 ; i++)
    {
        esp_err_t status = dsls_dotprod_16s_ae32(x,y,&z[1],i,val_shift);
        if (i < 4)
        {
            TEST_ASSERT_EQUAL(status, ESP_ERR_DSL_INVALID_LENGTH);            
        } else 
        {
            TEST_ASSERT_EQUAL(status, ESP_OK);
            TEST_ASSERT_EQUAL(check_value, z[0]);
            TEST_ASSERT_EQUAL(check_value + 1, z[2]);
            TEST_ASSERT_EQUAL((i*(val_x*val_y) + ((int)0x7fff>>val_shift))>>(15 - val_shift), z[1]);
        }
    }

    free(x);
    free(y);
    free(z);
}

static portMUX_TYPE testnlock = portMUX_INITIALIZER_UNLOCKED;
TEST_CASE("dsls_dotprod_16s_ae32 benchmark", "[dsls]")
{
    int max_N = 1024;

    int16_t* x = (int16_t*)malloc(max_N*sizeof(int16_t));
    int16_t* y = (int16_t*)malloc(max_N*sizeof(int16_t));
    int16_t* z = (int16_t*)malloc(max_N*sizeof(int16_t));

    for (int i=0 ; i< max_N ; i++)
    {
        x[i] = 0x100;
        y[i] = 0x200;
    }
    // Disable interrupt to get exect count
    
    portENTER_CRITICAL(&testnlock);

    unsigned int start_b = xthal_get_ccount();
    int repeat_count = 1024;
    for (int i=0 ; i< repeat_count ; i++)
    {
        dsls_dotprod_16s_ae32(x,y,&z[1],1024,0);
    }
    unsigned int end_b = xthal_get_ccount();
    portEXIT_CRITICAL(&testnlock);

    float total_b = end_b - start_b;
    float cycles = total_b/(1024*repeat_count);
    printf("Benchmark dsls_dotprod_16s - %f per sample + overhead. Result = %08x\n", cycles, z[1]);
    float min_exec = 1.5;
    float max_exec = 1.6;
    if (cycles >= max_exec) { 
        TEST_ASSERT_MESSAGE (false, "Exec time takes more then expected!");
    }
    if (cycles < min_exec) { 
        TEST_ASSERT_MESSAGE (false, "Exec time takes less then expected!");
    }

    free(x);
    free(y);
    free(z);
}

TEST_CASE("dsls_dotprod_16s_ansi benchmark", "[dsls]")
{
    int max_N = 1024;

    int16_t* x = (int16_t*)malloc(max_N*sizeof(int16_t));
    int16_t* y = (int16_t*)malloc(max_N*sizeof(int16_t));
    int16_t* z = (int16_t*)malloc(max_N*sizeof(int16_t));

    for (int i=0 ; i< max_N ; i++)
    {
        x[i] = 0x100;
        y[i] = 0x200;
    }
    // Disable interrupt to get exect count
    
    portENTER_CRITICAL(&testnlock);

    unsigned int start_b = xthal_get_ccount();
    int repeat_count = 1024;
    for (int i=0 ; i< repeat_count ; i++)
    {
        dsls_dotprod_16s_ansi(x,y,&z[1],1024,0);
    }
    unsigned int end_b = xthal_get_ccount();
    portEXIT_CRITICAL(&testnlock);

    float total_b = end_b - start_b;
    float cycles = total_b/(1024*repeat_count);
    printf("Benchmark dsls_dotprod_16s - %f per sample + overhead. Result = %08x\n", cycles, z[1]);
    float min_exec = 10;
    float max_exec = 30;
    if (cycles >= max_exec) { 
        TEST_ASSERT_MESSAGE (false, "Exec time takes more then expected!");
    }
    if (cycles < min_exec) { 
        TEST_ASSERT_MESSAGE (false, "Exec time takes less then expected!");
    }

    free(x);
    free(y);
    free(z);
}
