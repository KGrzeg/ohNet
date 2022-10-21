project(ohNet LANGUAGES CXX C)
cmake_minimum_required(VERSION 3.15...3.19)

#
# Avoid source tree pollution
#
If(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files already created:\nrm -rf CMakeCache.txt CMakeFiles")
endif(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)

file(GLOB all_files
  "OpenHome/*/*.h"
  "OpenHome/*/*.cpp"
  "Os/*/*.h"
  "Os/*/*.cpp"
)

add_library(ohNetCore ${all_files})

add_library(exception OpenHome/Exception.cpp)
add_library(testFramework OpenHome/TestFramework/TestFramework.cpp)

target_include_directories(
  ohNetCore PUBLIC $<INSTALL_INTERFACE:include>
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSShared"
)
target_include_directories(
  exception PUBLIC $<INSTALL_INTERFACE:include>
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSCore")
target_include_directories(
  testFramework PUBLIC $<INSTALL_INTERFACE:include>
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSCore")

target_compile_definitions(ohNetCore PUBLIC DEFINE_LITTLE_ENDIAN)
target_compile_definitions(exception PUBLIC DEFINE_LITTLE_ENDIAN)
target_compile_definitions(testFramework PUBLIC DEFINE_LITTLE_ENDIAN)

add_executable(bufferTest OpenHome/Tests/TestBuffer.cpp)
target_link_libraries(bufferTest PUBLIC ohNetCore exception testFramework)
