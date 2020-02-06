Espressif DSP Library API Reference
===================================

Header Files
------------

To use the library, include ``esp_dsp.h`` header file into the source code.

* :repo_file:`modules/common/include/esp_dsp.h`


Signal (1D) Processing APIs
----------------------------

Signal processing APIs use ``dsps`` prefix. The following modules are available:

- Dot-product_ - Calculates dot-product of two vectors
- FFT_ - Fast Fourier Transform functionality
- DCT_ - Discrete Cosine Transform functionality
- IIR_ - IIR filter functionality
- FIR_ - FIR filter functionality
- Math_ - Basic vector operations
- Conv_ - Convolution/correlation functionality 
- Support_ - Support functions
- `Window functions`_ - FFT window generation functions

Dot-product
+++++++++++

.. include:: /_build/inc/dsps_dotprod.inc

FFT
+++

.. include:: /_build/inc/dsps_fft2r.inc

DCT
+++

.. include:: /_build/inc/dsps_dct.inc

FIR
+++

.. include:: /_build/inc/dsps_fir.inc

IIR
+++

.. include:: /_build/inc/dsps_biquad_gen.inc
.. include:: /_build/inc/dsps_biquad.inc

Math
++++

.. include:: /_build/inc/dsps_add.inc
.. include:: /_build/inc/dsps_sub.inc
.. include:: /_build/inc/dsps_mul.inc
.. include:: /_build/inc/dsps_addc.inc
.. include:: /_build/inc/dsps_mulc.inc

Conv
++++

.. include:: /_build/inc/dsps_conv.inc
.. include:: /_build/inc/dsps_corr.inc

Support
+++++++

.. include:: /_build/inc/dsps_d_gen.inc
.. include:: /_build/inc/dsps_h_gen.inc
.. include:: /_build/inc/dsps_tone_gen.inc
.. include:: /_build/inc/dsps_view.inc
.. include:: /_build/inc/dsps_snr.inc
.. include:: /_build/inc/dsps_sfdr.inc

Window Functions
++++++++++++++++

.. include:: /_build/inc/dsps_wind_hann.inc

Matrix Operations APIs
----------------------

Matrix operations APIs use ``dspm`` prefix. The following modules are available:

- Multiplication - basic matrix multiplication operations

Matrix Multiplication
+++++++++++++++++++++

.. include:: /_build/inc/dspm_mult.inc

Matrix Operations
+++++++++++++++++++++

.. include:: /_build/inc/mat.inc

Miscellaneous
-------------

Various common functions used by other modules are included in this module.

Common APIs
+++++++++++

.. include:: /_build/inc/dsp_common.inc
