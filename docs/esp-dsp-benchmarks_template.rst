Espressif DSP Library Benchmarks
================================

The table bellow contains benchmarks of functions provided by ESP-DSP library. The values are CPU cycle counts taken to execute each of the functions. 
The Values in the column "O2" are made with compiler optimization for speed, and in the column "Os" column are made with compiler optimization for size. 
The values in "ESP32" and "ESP32S3" column are for the optimized (assembly) implementation, values in "ANSI" column are for the non-optimized implementation.

MAIN_TABLE

The benchmark test could be reproduced by executing test cases found in :repo_file:`test/test_dsp.c`.
