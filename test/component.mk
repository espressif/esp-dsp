# COMPONENT_EMBED_FILES := test_partition_v1.bin

COMPONENT_SRCDIRS := . \
					../modules/test \
					../modules/dotprod/test \
					../modules/matrix/test \
					../test 

COMPONENT_ADD_LDFLAGS = -Wl,--whole-archive -l$(COMPONENT_NAME) -Wl,--no-whole-archive

