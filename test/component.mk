COMPONENT_SRCDIRS := \
					. \
					../modules/dotprod/test \
					../modules/matrix/test \
					../modules/iir/test \
					../modules/fir/test \
					../modules/math/mulc/test \
					../modules/math/addc/test \
					../modules/support/view/test \
					../modules/support/snr/test \
					../modules/support/sfdr/test \
					../modules/fft/test

COMPONENT_ADD_LDFLAGS = -Wl,--whole-archive -l$(COMPONENT_NAME) -Wl,--no-whole-archive

