# Espressif DSP Library

ESP-DSP is the official DSP library for the [ESP32](https://espressif.com/en/products/hardware/esp32/overview) chip.

The esp-dsp library is designed as an extension for the [ESP-IDF](https://github.com/espressif/esp-idf) library and could be used together with it.

The esp-dsp library contains Signal (1D) Application APIs:

 - Common APIs - common APIs that could be used by any domain
 - Dot-product - calculates dot-product of two vectors
 - FFT - Fast Frequency Transform functionality
 - IIR - IIR Filter functionality
 - FIR - FIR Filter functionality
 - Math - Basic Vector operations
 - Support - Support functions
 - Windows - window generating functions

The esp-dsp library contains ANSI C implementation of the core DSP functions and Esp32 optimized implementation. The implementation to be used can be switched by KConfig option.

The esp-dsp library is a component that have to be placed as submodule project into the user project components folder, and handle it there:  
 
    cd $(your project path)/components
    git clone https://github.com/espressif/esp-dsp.git dsp

The esp-dsp library contains the documentation in esp-dsp/docs folder.

The esp-dsp library contains next examples that could be used to get an overview of the library:

- Dotprod - demonstrates how to use dotprod dsps_dotprod_f32 from esp-dsp library.
- IIR - demonstrates how to use IIR filters functionality from esp-dsp library.
- FFT - demonstrates how to use FFT functionality from esp-dsp library.

For more details please read README.md for each example.
