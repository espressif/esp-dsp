# This is the project CMakeLists.txt file for the test subproject 
cmake_minimum_required(VERSION 3.5)

# Include the components directory of the main application:
#
set(EXTRA_COMPONENT_DIRS "../")
set(IDF_EXCLUDE_COMPONENTS "test" "test_app")

set(TEST_COMPONENTS "esp-dsp" CACHE STRING "List of components to test")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(test_app)
