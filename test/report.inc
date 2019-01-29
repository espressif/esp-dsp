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

#pragma once

#include <stdio.h>
#include <freertos/FreeRTOS.h>

/**
 * This file contains helper functions and macros used to generate
 * the benchmarks table in test_dsp.c.
 */

static char report_space_str[] = "                                                            ";
static char report_line_str[] = "------------------------------------------------------------";
static char report_double_line_str[] = "============================================================";

#define REPORT_COL1_WIDTH 58
#define REPORT_COL2_WIDTH 10
#define REPORT_COL3_WIDTH 10

static void report_text_line_2col(const char* col1, const char* col2)
{
    printf("| %-*.*s| %-*.*s|\n",
        REPORT_COL1_WIDTH - 1, REPORT_COL1_WIDTH - 1, col1,
        REPORT_COL2_WIDTH + REPORT_COL3_WIDTH, REPORT_COL2_WIDTH + REPORT_COL3_WIDTH, col2);
}
static void report_text_line(const char* col1, const char* col2, const char* col3)
{
    printf("| %-*.*s| %-*.*s| %-*.*s|\n",
        REPORT_COL1_WIDTH - 1, REPORT_COL1_WIDTH - 1, col1,
        REPORT_COL2_WIDTH - 1, REPORT_COL2_WIDTH - 1, col2,
        REPORT_COL3_WIDTH - 1, REPORT_COL3_WIDTH - 1, col3);
}

static void report_value_line(const char* col1, uint32_t col2, uint32_t col3)
{
    printf("| %-*.*s|%*u |%*u |\n",
        REPORT_COL1_WIDTH - 1, REPORT_COL1_WIDTH - 1, col1,
        REPORT_COL2_WIDTH - 1, col2,
        REPORT_COL3_WIDTH - 1, col3);
}

static void report_separator(const char* fill)
{
    printf("+%.*s+%.*s+%.*s+\n",
        REPORT_COL1_WIDTH, fill,
        REPORT_COL2_WIDTH, fill,
        REPORT_COL3_WIDTH, fill);
}

#define REPORT_BENCHMARK(title, func1, func2, ...) \
    do { \
        func1(__VA_ARGS__); /* warm up the cache */ \
        unsigned intlevel = portENTER_CRITICAL_NESTED(); \
        uint32_t func1_start = xthal_get_ccount(); \
        func1(__VA_ARGS__); \
        uint32_t func1_end = xthal_get_ccount(); \
        portEXIT_CRITICAL_NESTED(intlevel); \
        func2(__VA_ARGS__); /* warm up the cache */ \
        intlevel = portENTER_CRITICAL_NESTED(); \
        uint32_t func2_start = xthal_get_ccount(); \
        func2(__VA_ARGS__); \
        uint32_t func2_end = xthal_get_ccount(); \
        portEXIT_CRITICAL_NESTED(intlevel); \
        report_value_line(title, func1_end - func1_start, func2_end - func2_start); \
        report_separator(report_line_str); \
    } while(0);

#define REPORT_SECTION(title) \
    do { \
        report_text_line(report_space_str, report_space_str, report_space_str); \
        report_separator(report_line_str); \
        report_text_line(title, report_space_str, report_space_str); \
        report_separator(report_line_str); \
    } while(0);


#define REPORT_HEADER() \
    do { \
        report_separator(report_line_str); \
        report_text_line_2col("Function name and arguments", "CPU cycles"); \
        report_separator(report_line_str); \
        report_text_line(report_space_str, "ESP32", "ANSI C"); \
        report_separator(report_double_line_str); \
    } while(0);
