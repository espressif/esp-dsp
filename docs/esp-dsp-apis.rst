Espressif DSP Library APIs Description
=======================================

Header Files
-------------

To use Library header file `esp_dsp.h` have to be included into the source code. 

* :component_file:`esp_dsp.h`


Signal (1D) Applicatnios APIs 
-----------------------------

The Signal applicatnios APIs has name "dsps_". Now DSP Library contains next signal modules:
 - Common APIs - common APIs that could be used by any domain
 - Dotproduct - calculates potproduct of two vectors
 - FFT - Fast Frequency Transform functoina
 - IIR - IIR Filter functionality
 - FIR - FIR Filter functionality
 - Math- Basic Vector operatoins
 - Support - Support functions
 - Windows - window generating functions

Common APIs
^^^^^^^^^^^
.. include:: /_build/inc/dsp_common.inc

Dotproduct
^^^^^^^^^^^
.. include:: /_build/inc/dsps_dotprod.inc

FFT
^^^^^^^^^^^
.. include:: /_build/inc/dsps_fft2r.inc

FIR
^^^^^^^^^^^
.. include:: /_build/inc/dsps_fir.inc

IIR
^^^^^^^^^^^
.. include:: /_build/inc/dsps_biquad_gen.inc
.. include:: /_build/inc/dsps_biquad.inc

Math
^^^^^^^^^^^
.. include:: /_build/inc/dsps_addC.inc
.. include:: /_build/inc/dsps_mulC.inc

Support
^^^^^^^^^^^
.. include:: /_build/inc/dsps_d_gen.inc
.. include:: /_build/inc/dsps_h_gen.inc
.. include:: /_build/inc/dsps_tone_gen.inc
.. include:: /_build/inc/dsps_view.inc
.. include:: /_build/inc/dsps_snr.inc
.. include:: /_build/inc/dsps_sfdr.inc

Windows
^^^^^^^^^^^
.. include:: /_build/inc/dsps_wind_Barrel.inc

Matrix Operations APIs 
-----------------------------

 The Matrix operatoins APIs has name "dspm_". Now DSP Library contains next maxtrix operation modules:
 - Multiplication - basic matrix multiplication operatoins

Matrix Multiplication
^^^^^^^^^^^^^^^^^^^^^^

.. include:: /_build/inc/dspm_mult.inc






