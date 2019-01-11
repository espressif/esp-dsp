#
# This is a project Makefile for the test subproject.
#

PROJECT_NAME := test_app

# Include the components directory of the main application:
#
EXTRA_COMPONENT_DIRS := $(realpath ..)
EXCLUDE_COMPONENTS := test_app
TEST_COMPONENTS ?= esp-dsp

include $(IDF_PATH)/make/project.mk
