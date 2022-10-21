project(ohNet LANGUAGES CXX C)
cmake_minimum_required(VERSION 3.15...3.19)

#
# Avoid source tree pollution
#
If(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files already created:\nrm -rf CMakeCache.txt CMakeFiles")
endif(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)

set(MODULES buffer)

add_library(buffer OpenHome/Buffer.cpp)
target_compile_definitions(buffer PUBLIC DEFINE_LITTLE_ENDIAN)

target_include_directories(
  buffer PUBLIC $<INSTALL_INTERFACE:include>
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>")
