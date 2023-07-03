Espressif DSP Library API Reference
===================================

Header Files
------------

To use the library, include ``esp_dsp.h`` header file into the source code.

* :project_file:`modules/common/include/esp_dsp.h`


Signal (1D) Processing APIs
----------------------------

Signal processing APIs use ``dsps`` prefix. The following modules are available:

- Dot-product_ - Calculates dot-product of two vectors
- FFT_ - Fast Fourier Transform functionality
- DCT_ - Discrete Cosine Transform functionality
- IIR_ - IIR filter functionality
- FIR_ - FIR filter functionality
- `Basic math`_ - Basic vector operations
- `Convolution and correlation`_ - Convolution/correlation functionality
- Generator_ - Complex tone generator 
- Support_ - Development support functions
- `Window functions`_ - FFT window generation functions
- `Kalman Filer`_ - Extended Kalman Filter

Dot-product
+++++++++++

.. include-build-file:: inc/dsps_dotprod.inc
.. include-build-file:: inc/dspi_dotprod.inc

FFT
+++

.. include-build-file:: inc/dsps_fft2r.inc
.. include-build-file:: inc/dsps_fft4r.inc

DCT
+++

.. include-build-file:: inc/dsps_dct.inc

FIR
+++

.. include-build-file:: inc/dsps_fir.inc

IIR
+++

.. include-build-file:: inc/dsps_biquad_gen.inc
.. include-build-file:: inc/dsps_biquad.inc

Basic math
++++++++++++

.. include-build-file:: inc/dsps_mul.inc
.. include-build-file:: inc/dsps_addc.inc
.. include-build-file:: inc/dsps_mulc.inc

Convolution and correlation
+++++++++++++++++++++++++++

.. include-build-file:: inc/dsps_conv.inc
.. include-build-file:: inc/dsps_corr.inc

Generator
++++++++++++++

.. include-build-file:: inc/dsps_cplx_gen.inc


Support
+++++++

.. include-build-file:: inc/dsps_d_gen.inc
.. include-build-file:: inc/dsps_h_gen.inc
.. include-build-file:: inc/dsps_tone_gen.inc
.. include-build-file:: inc/dsps_view.inc
.. include-build-file:: inc/dsps_snr.inc
.. include-build-file:: inc/dsps_sfdr.inc



Kalman Filer
+++++++++++++

.. include-build-file:: inc/ekf.inc

For more details about implementation of 13 states EKF, please read: :project_file:`modules/kalman/ekf_imu13states/docs/README.md`

.. include-build-file:: inc/ekf_imu13states.inc


Window Functions
++++++++++++++++

.. include-build-file:: inc/dsps_wind_hann.inc
.. include-build-file:: inc/dsps_wind_blackman.inc
.. include-build-file:: inc/dsps_wind_blackman_harris.inc
.. include-build-file:: inc/dsps_wind_blackman_nuttall.inc
.. include-build-file:: inc/dsps_wind_flat_top.inc
.. include-build-file:: inc/dsps_wind_nuttall.inc

Matrix Operations APIs
----------------------

Matrix operations APIs use ``dspm`` prefix. The following modules are available:

- Multiplication - basic matrix multiplication operations

Matrix Multiplication
+++++++++++++++++++++

.. include-build-file:: inc/dspm_mult.inc

Matrix Operations
+++++++++++++++++++++

.. include-build-file:: inc/mat.inc

Miscellaneous
-------------

Various common functions used by other modules are included in this module.

Common APIs
+++++++++++

.. include-build-file:: inc/dsp_common.inc
