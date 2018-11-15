# COMPONENT_EMBED_FILES := test_partition_v1.bin

COMPONENT_SRCDIRS := . \
					../modules/test \
					../modules/dotprod/test \
					../modules/matrix/test \
					../modules/iir/test \
					../modules/fir/test \
					../modules/math/mulC/test \
					../modules/math/addC/test \
					../modules/support/view/test \
					../modules/support/snr/test \
					../modules/support/sfdr/test \
					../modules/fft/test \
					../test 

COMPONENT_ADD_LDFLAGS = -Wl,--whole-archive -l$(COMPONENT_NAME) -Wl,--no-whole-archive

