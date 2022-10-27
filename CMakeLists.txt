project(ohNet LANGUAGES CXX C)
cmake_minimum_required(VERSION 3.15...3.19)

#
# Avoid source tree pollution
#
If(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files already created:\nrm -rf CMakeCache.txt CMakeFiles")
endif(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)

set(osdir Posix)
set(mDNSdir thirdparty/mDNSResponder-1310.80.1)


file(GLOB all_files
  "OpenHome/**/*.cpp"
  "OpenHome/Net/Device/Bonjour/*.cpp"
  "OpenHome/Net/Device/*.cpp"
  "OpenHome/Net/Tests/*.cpp"
  "OpenHome/*.cpp"

  "Os/Posix/*.cpp"
  "Os/Posix/*.c"
  "Os/*.cpp"

  thirdparty/libb64/cencode.c
  thirdparty/libb64/cdecode.c

  thirdparty/mDNSResponder-1310.80.1/mDNSCore/*.c
  thirdparty/mDNSResponder-1310.80.1/mDNSShared/dnssd_clientshim.c
  thirdparty/mDNSResponder-1310.80.1/mDNSShared/dnssd_clientlib.c
)


list(FILTER all_files EXCLUDE REGEX "OpenHome(/.*)?/Tests/.+")
list(FILTER all_files EXCLUDE REGEX ".*TestShell.cpp$")


file(GLOB test_files
  OpenHome/TestFramework/*.cpp
)

add_library(ohNetCore ${all_files})
add_library(testFramework ${test_files})
add_library(TestBufferMain OpenHome/Tests/TestBufferMain.cpp)

target_include_directories(
  ohNetCore PUBLIC $<INSTALL_INTERFACE:include>
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSShared"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSCore"
)
target_include_directories(
  testFramework PUBLIC $<INSTALL_INTERFACE:include>
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSCore")
target_include_directories(
  TestBufferMain PUBLIC $<INSTALL_INTERFACE:include>
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSCore")

target_compile_definitions(ohNetCore PUBLIC DEFINE_LITTLE_ENDIAN)
target_compile_definitions(ohNetCore PUBLIC _GNU_SOURCE)
target_compile_definitions(testFramework PUBLIC DEFINE_LITTLE_ENDIAN)
target_compile_definitions(TestBufferMain PUBLIC DEFINE_LITTLE_ENDIAN)

add_executable(TestBuffer OpenHome/Tests/TestBuffer.cpp)
target_link_libraries(TestBuffer PUBLIC ohNetCore testFramework TestBufferMain)

# sngorder
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
# set(DYLIB_EXT so)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package (Threads)

add_executable(sngorder sngorder/main.cpp)
target_link_libraries(sngorder PUBLIC ohNetCore)
target_compile_features(sngorder PRIVATE cxx_auto_type)
target_include_directories(
  sngorder PUBLIC $<INSTALL_INTERFACE:include>
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSShared"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSCore"
)
target_compile_definitions(sngorder PUBLIC DEFINE_LITTLE_ENDIAN)



