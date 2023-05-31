set(COMPONENT_SRCDIRS 
                    "."
                    "../modules/dotprod/test"
                    "../modules/matrix/mul/test"
                    "../modules/matrix/add/test"
                    "../modules/matrix/addc/test"
                    "../modules/matrix/mulc/test"
                    "../modules/matrix/sub/test"
                    "../modules/iir/test"
                    "../modules/fir/test"
                    "../modules/math/mulc/test"
                    "../modules/math/addc/test"
                    "../modules/math/add/test"
                    "../modules/math/sub/test"
                    "../modules/math/mul/test"
                    "../modules/math/sqrt/test"
                    "../modules/support/view/test"
                    "../modules/support/snr/test"
                    "../modules/support/sfdr/test"
                    "../modules/support/cplx_gen/test"
                    "../modules/fft/test"
                    "../modules/dct/test"
                    "../modules/conv/test"
                    "../modules/kalman/ekf_imu13states/test"

)

if(target STREQUAL "esp32s3")
    list(APPEND COMPONENT_SRCDIRS "../modules/support/mem/test")
endif()

set(COMPONENT_REQUIRES unity esp-dsp)

set(COMPONENT_ADD_LDFRAGMENTS linker.lf)

register_component()
 