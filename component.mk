COMPONENT_ADD_INCLUDEDIRS := modules/dotprod/include \
							modules/support/include \
							modules/windows/Barrel/include \
							modules/iir/include \
							modules/fir/include \
							modules/math/include \
							modules/matrix/include \
							modules/fft/include \
							modules/common/include 

COMPONENT_SRCDIRS :=. \
					modules/common \
					modules/common/misc \
					modules/misc \
					modules/dotprod \
					modules/dotprod/float \
					modules/dotprod/fixed \
					modules/matrix \
					modules/matrix/float \
					modules/matrix/fixed \
					modules/math \
					modules/math/mulC \
					modules/math/mulC/float \
					modules/math/addC \
					modules/math/addC/float \
					modules/fft/float \
					modules/support \
					modules/support/snr \
					modules/support/sfdr \
					modules/support/misc \
					modules/support/view \
					modules/windows/Barrel \
					modules/windows/Barrel/float \
					modules/windows/Barrel/fixed \
					modules/iir \
					modules/iir/biquad \
					modules/fir \
					modules/fir/float
					
COMPONENT_PRIV_INCLUDEDIRS := 	modules/dotprod/float \
								modules/dotprod/fixed				
					
#COMPONENT_ADD_LDFLAGS = -g -O2 -c -fverbose-asm

# This is flags to compile assembler files without error: danger relocation...
#modules/fft/float/dsls_fft2r_32fc_ansi.o: CFLAGS += -mtext-section-literals
#modules/fft/float/dsls_fft2r_32fc_ansi.o: CPPFLAGS += -mtext-section-literals
#modules/fft/float/dsls_fft2r_32fc_ae32.o: CFLAGS += -mtext-section-literals
#modules/fft/float/dsls_fft2r_32fc_ae32.o: CPPFLAGS += -mtext-section-literals
#modules/fft/float/dsls_fft2r_32fc_ae32.o: ARFLAGS += -mtext-section-literals
#modules/fft/float/dsls_fft2r_32fc_ansi.o: CPPFLAGS += -mtext-section-literals

#-Wl,--whole-archive -l$(COMPONENT_NAME) -Wl,--no-whole-archive
