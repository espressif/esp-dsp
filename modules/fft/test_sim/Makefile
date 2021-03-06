TEST_PROG=test_fir

CC = xt-xcc
AS = xt-as
LD = xt-ld

OBJECTS=main.o \
		test_fft2r.o \
		../float/dsps_fft2r_bitrev_tables_fc32.o \
		../float/dsps_fft2r_fc32_ansi.o \
		../float/dsps_fft2r_fc32_ae32_.o \
		../float/dsps_fft2r_fc32_aes3_.o

#		../float/dsps_fft2r_fc32_ae32.o


CFLAGS = -std=c99 -g -O2 -D__BSD_VISIBLE \
		-I../../common/include \
		-I../../common/include_sim \
		-I../../common/private_include \
		-I../../dotprod/include \
		-I../../dotprod/float \
		-I../../dotprod/fixed \
		-I../../matrix/include \
		-I../../fft/include \
		-I../../iir/include \
		-I../../fir/include 			


PROFILE_OUTPUT=$(TEST_PROG)_prof_out

CPPFLAGS= $(CFLAGS)			
ASFLAGS = $(CFLAGS) 
			
LIBS += -lm -lc -lgcc

# include standard C library
LDFLAGS += -lc
# include standard math library
LDFLAGS += -lm

all: $(TEST_PROG)

$(TEST_PROG): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

run: $(TEST_PROG)
	xt-run $(TEST_PROG)

debug: $(TEST_PROG)
	xt-gdb $(TEST_PROG) -ex "b main" -ex "run"

profile: $(TEST_PROG)
	xt-run --client_commands="profile --disable --all $(PROFILE_OUTPUT)" $(TEST_PROG)

# Generate GPROF targets: show-insn, show-cyc, show-bdelay, show-interlock
GPROF_TYPES := cyc bdelay interlock insn
GPROF_OUTPUTS := $(sort $(addprefix $(PROFILE_OUTPUT).,$(GPROF_TYPES)) $(addsuffix .incl,$(addprefix $(PROFILE_OUTPUT).,$(GPROF_TYPES))))
GPROF_TARGETS := $(addprefix show-,$(GPROF_TYPES))

define GenerateGprofTarget
# $(1) - profiling information type (insn, cyc, bdelay, interlock)
show-$(1): $$(PROFILE_OUTPUT).$(1)
	xt-gprof $$(TEST_PROG) $$^

$$(PROFILE_OUTPUT).$(1): profile
endef

$(foreach tp,$(GPROF_TYPES), $(eval $(call GenerateGprofTarget,$(tp))))

clean:
	rm -f $(OBJECTS) $(TEST_PROG) $(GPROF_OUTPUTS)

.PHONY: all clean run profile $(GPROF_TARGETS)
