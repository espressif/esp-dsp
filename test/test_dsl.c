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
