Espressif DSP Library Examples
==============================

List of esp-dsp Examples
----------------------------

Signal processing APIs use ``dsps`` prefix. The following modules are available:

- `Basic math`_ - the example shows how to use basic vector math operations
- Dot-product_ - the example demonstrates how to use dot product functions
- FFT_ - the example demonstrates how to use FFT functionality
- `FFT Window`_ - the example demonstrates how to use Window and FFT functionality
- `FFT 4 Real`_ - the example demonstrates how to use FFT functionality for real input signals
- IIR_ - the example demonstrates how to use IIR filters functionality
- FIR_ -  the example demonstrates how to use FIR filter functionality
- `Kalman Filter`_ - Extended Kalman Filter (EKF) example
- Matrix_ - example demonstrates how to use Mat class functionality

Basic math
++++++++++

This example demonstrates how to use basic math functions from esp-dsp library.
The example does the following steps:

1. Initialize the library
2. Initialize input signals with 1024 samples
3. Apply window to input signal by standard C loop.
4. Calculate FFT for 1024 complex samples and show the result
5. Show results on the plots
6. Apply window to input signal by basic math functions dsps_mul_f32 and dsps_mulc_f32.
7. Calculate FFT for 1024 complex samples
8. Show results on the plots 

For more details please look to the :project_file:`examples/basic_math/README.md`

Dot-product
+++++++++++

The example demonstrates how to use dotprod dsps_dotprod_f32 from esp-dsp library. Example does the following steps:

1. Initialize the input arrays
2. Calculate dot product of two arrays
3. Compare results and calculate execution time in cycles. 

For more details please look to the :project_file:`examples/dotprod/README.md`

FFT
+++

This example demonstrates how to use FFT functionality from esp-dsp library. Example does the following steps:

1. Initialize the library
2. Initialize input signals with 1024 samples: one 0 dB, second with -20 dB
3. Combine two signals as one complex input signal and apply window to input signals paar.
4. Calculate FFT for 1024 complex samples
5. Apply bit reverse operation for output complex vector
6. Split one complex FFT output spectrum to two real signal spectrums 
7. Show results on the plots
8. Show execution time of FFT

For more details please look to the :project_file:`examples/fft/README.md`

FFT Window
++++++++++

This example demonstrates how to use Window and FFT functionality from esp-dsp library. Example does the following steps:

1. Initialize the library
2. Initialize input signals with 1024 samples
3. Apply window to input signal.
4. Calculate FFT for 1024 complex samples
5. Apply bit reverse operation for output complex vector
6. Split one complex FFT output spectrum to two real signal spectrums 
7. Show results on the plots 

For more details please look to the :project_file:`examples/fft_window/README.md`

FFT 4 Real
++++++++++

This example demonstrates how to use FFT functionality from esp-dsp library. Example does the following steps:

1. Initialize the library
2. Initialize input signals with 1024 samples: one 0 dB, second with -20 dB
3. Calculate FFT Radix-2 for 1024 complex samples
4. Calculate FFT Radix-4 for 1024 complex samples
5. Apply bit reverse operation for output complex vectors
6. Show results on the plots
7. Show execution time of FFTs

For more details please look to the :project_file:`examples/fft4real/README.md`

IIR
+++

This example demonstrates how to use IIR filters functionality from esp-dsp library. Example does the following steps:

1. Initialize the library
2. Initialize input signal
3. Show LPF filter with Q factor 1
    * Calculate iir filter coefficients
    * Filter the input test signal (delta function)
    * Shows impulse response on the plot
    * Shows frequency response on the plot
    * Calculate execution performance
4. The same for LPF filter with Q factor 10 

For more details please look to the :project_file:`examples/fir/README.md`

FIR
+++

This example demonstrates how to use FIR filter functionality from esp-dsp library. Example does the following steps:

1. Initialize the FFT library
2. Initialize input signal
3. Show input signal
4. Show filtered signal

For more details please look to the :project_file:`examples/fir/README.md`

Kalman Filter
+++++++++++++

This example emulate system with IMU sensors and show how to use Extended Kalman Filter (EKF), with 13 values states vector,
to estimate gyroscope errors and calculate system attitude.
Also, this example show how to use esp-dsp library to operate with matrices and vectors.

In real system, the emulated sensors values should be replace by the real sensors values. 
Then, in real system, a calibration phase should be implemented and after the calibration 
phase the state vector X and covariance matrix P should be saved and restored next time, when 
filter called. It will save time for initial phase.
 
For more details please look to the :project_file:`examples/kalman/README.md`

Matrix
++++++

This example demonstrates how to use Mat class functionality from esp-dsp library. Example does the following steps:

1. Initialize a matrix A and matirx x
2. Calculate matrix b: b = A*x
3. Find roots x1: A*x1 = b, with different methods
4. Print result 

