# Espressif DSP Library

ESP-DSP is the official DSP library for the [ESP32](https://espressif.com/en/products/hardware/esp32/overview) chip.

## Overview

ESP-DSP is intended to be used as an [ESP-IDF](https://github.com/espressif/esp-idf) component. For the introduction to ESP-IDF, refer to the [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/).

The ESP-DSP library includes implementations of the following functions:
- Matrix multiplication: [reference](https://docs.espressif.com/projects/esp-dsp/en/latest/esp-dsp-apis.html#matrix-operations-apis)
- Dot product: [reference](https://docs.espressif.com/projects/esp-dsp/en/latest/esp-dsp-apis.html#dot-product), [example](https://github.com/espressif/esp-dsp/tree/master/examples/dotprod)
- FFT: [reference](https://docs.espressif.com/projects/esp-dsp/en/latest/esp-dsp-apis.html#fft), [example](https://github.com/espressif/esp-dsp/tree/master/examples/fft)
- IIR: [reference](https://docs.espressif.com/projects/esp-dsp/en/latest/esp-dsp-apis.html#iir), [example](https://github.com/espressif/esp-dsp/tree/master/examples/iir)
- FIR: [reference](https://docs.espressif.com/projects/esp-dsp/en/latest/esp-dsp-apis.html#fir)
- Vector math operations: [reference](https://docs.espressif.com/projects/esp-dsp/en/latest/esp-dsp-apis.html#math)

Many of the library functions are written in assembly and are optimized for the CPU configuration used in the ESP32. In addition to the optimized implementations, reference implementations written in ANSI C are provided.

Function implementations are provided for single precision floating point (32-bit float), and 16-bit signed integers.

## Documentation

- [ESP-DSP Overview](https://docs.espressif.com/projects/esp-dsp/en/latest/esp-dsp-library.html)
- [ESP-DSP API Reference](https://docs.espressif.com/projects/esp-dsp/en/latest/esp-dsp-apis.html)
- [ESP-DSP Benchmarks](https://docs.espressif.com/projects/esp-dsp/en/latest/esp-dsp-benchmarks.html)

Documentation found in the above links is automatically generated from the contents of this repository. If you find that some information is missing or incomplete, please report an issue.

## Installation and Usage

The ESP-DSP library is a component for the [ESP-IDF build system](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html). It also works with the [new CMake-based build system](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system-cmake.html).

### Trying out ESP-DSP examples

If you haven't created an ESP-IDF project yet, and wish to try the examples provided with ESP-DSP, you can clone ESP-DSP repository into any directory, and then run examples from there:

    cd ~/esp
    git clone https://github.com/espressif/esp-dsp.git
    cd esp-dsp/examples/dotprod
    make -j4 flash monitor ESPPORT=PORT

or, if you are using CMake based build system,

    idf.py -p PORT flash monitor

where `PORT` is the UART port name of your development board, such as `/dev/ttyUSB0` or `COM1`. 

Note that you need to set up environment variables (`IDF_PATH`, `PATH`) before building the project. Refer to the [ESP-IDF Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) if you don't have the environment set up yet.

For the list of the examples, please see [README.md](examples/README.md) in the examples directory.

### Including ESP-DSP into your own project

To include ESP-DSP into your ESP-IDF project, clone ESP-DSP repository (or add it as a submodule) into the components directory of the project:

    cd your-project-directory
    mkdir -p components
    cd components
    git clone https://github.com/espressif/esp-dsp.git

ESP-IDF build system also allows including components which are not located in the project components directory, using `EXTRA_COMPONENT_DIRS` project variable. Please refer to the [ESP-IDF build system documentation](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html) for details.

## Reporting Issues

If you have found an issue in ESP-DSP, or wish to submit an enhancement request, please use the [Issues](https://github.com/espressif/esp-dsp/issues) section on Github.

For general questions related to this library, please use the [esp32.com forum](https://esp32.com/).

## Copyrights and License

All original source code in this repository is Copyright (C) 2018-2019 Espressif Systems. This source code is licensed under the Apache License 2.0 as described in the file LICENSE.
