#include <string.h>
#include "unity.h"
#include "test_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portable.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_clk.h"
#include "soc/cpu.h"

TEST_CASE("DSL doesn't leak memory", "[dsl]")
{
    size_t size_before = xPortGetFreeHeapSize();
    size_t size_after = xPortGetFreeHeapSize();

    ptrdiff_t stack_diff = size_before - size_after; 
    stack_diff = abs(stack_diff);
    if (stack_diff > 8) TEST_ASSERT_EQUAL(0, stack_diff);
}

// TEST_CASE("partition parameters", "[dsl][ignore]")
// {
//     size_t size_before = xPortGetFreeHeapSize();
//     size_t size_after = xPortGetFreeHeapSize();

//     ptrdiff_t stack_diff = size_before - size_after; 
//     stack_diff = abs(stack_diff);
//     if (stack_diff > 8) TEST_ASSERT_EQUAL(0, stack_diff);
// }
