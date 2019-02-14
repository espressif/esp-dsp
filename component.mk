COMPONENT_ADD_INCLUDEDIRS := modules/dotprod/include \
							modules/support/include \
							modules/windows/hann/include \
							modules/iir/include \
							modules/fir/include \
							modules/math/include \
							modules/matrix/include \
							modules/fft/include \
							modules/common/include 

COMPONENT_SRCDIRS :=. \
					modules/common \
					modules/common/misc \
					modules/dotprod \
					modules/dotprod/float \
					modules/dotprod/fixed \
					modules/matrix \
					modules/matrix/float \
					modules/matrix/fixed \
					modules/math \
					modules/math/mulc \
					modules/math/mulc/float \
					modules/math/addc \
					modules/math/addc/float \
					modules/fft/float \
					modules/fft/fixed \
					modules/support \
					modules/support/snr/float \
					modules/support/sfdr/float \
					modules/support/misc \
					modules/support/view \
					modules/windows/hann \
					modules/windows/hann/float \
					modules/windows/hann/fixed \
					modules/iir \
					modules/iir/biquad \
					modules/fir \
					modules/fir/float
					
COMPONENT_PRIV_INCLUDEDIRS := 	modules/dotprod/float \
								modules/dotprod/fixed \
								modules/common/private_include 
					
