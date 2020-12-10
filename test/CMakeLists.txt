set(COMPONENT_SRCDIRS 
                    "."
                    "../modules/dotprod/test"
                    "../modules/matrix/test"
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
                    "../modules/fft/test"
                    "../modules/dct/test"
                    "../modules/conv/test"
)
set(COMPONENT_REQUIRES unity esp-dsp)

set(COMPONENT_ADD_LDFRAGMENTS linker.lf)

register_component()
 