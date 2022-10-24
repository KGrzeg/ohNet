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

list(APPEND src_core
  OpenHome/Ascii.cpp
  OpenHome/Net/Bindings/C/AsyncC.cpp
  OpenHome/Net/ControlPoint/AsyncPrivate.cpp
  OpenHome/Net/Device/Bonjour/Bonjour.cpp
  OpenHome/Buffer.cpp
  OpenHome/Converter.cpp
  OpenHome/Crc32.cpp
  thirdparty/libb64/cencode.c
  thirdparty/libb64/cdecode.c
  OpenHome/Net/Discovery.cpp
  OpenHome/Debug.cpp
  OpenHome/Net/ControlPoint/CpDeviceCore.cpp
  OpenHome/Net/Bindings/C/ControlPoint/CpDeviceC.cpp
  OpenHome/Net/Bindings/Cpp/ControlPoint/CpDeviceStd.cpp
  OpenHome/Net/ControlPoint/CpDeviceDvCore.cpp
  OpenHome/Net/Bindings/Cpp/ControlPoint/CpDeviceDvStd.cpp
  OpenHome/Net/Bindings/C/ControlPoint/CpDeviceDvC.cpp
  OpenHome/Net/ControlPoint/CpDeviceUpnpCore.cpp
  OpenHome/Net/Bindings/C/ControlPoint/CpDeviceUpnpC.cpp
  OpenHome/Net/Bindings/Cpp/ControlPoint/CpDeviceUpnpStd.cpp
  OpenHome/Net/ControlPoint/CpiDevice.cpp
  OpenHome/Net/ControlPoint/Dv/CpiDeviceDv.cpp
  OpenHome/Net/ControlPoint/Lpec/CpiDeviceLpec.cpp
  OpenHome/Net/ControlPoint/Upnp/CpiDeviceUpnp.cpp
  OpenHome/Net/ControlPoint/CpiService.cpp
  OpenHome/Net/ControlPoint/CpiStack.cpp
  OpenHome/Net/ControlPoint/CpiSubscription.cpp
  OpenHome/Net/ControlPoint/CpProxy.cpp
  OpenHome/Net/Bindings/C/ControlPoint/CpProxyC.cpp
  OpenHome/Net/Bindings/C/ControlPoint/CpServiceC.cpp
  OpenHome/Net/Device/DvDevice.cpp
  OpenHome/Net/Bindings/Cpp/Device/DvDeviceStd.cpp
  OpenHome/Net/Bindings/C/Device/DvDeviceC.cpp
  OpenHome/Net/Device/DviDevice.cpp
  OpenHome/Net/Bindings/C/Device/DvInvocationC.cpp
  OpenHome/Net/Bindings/Cpp/Device/DvInvocationStd.cpp
  OpenHome/Net/Device/Upnp/DviProtocolUpnp.cpp
  OpenHome/Net/Device/DviProviderSubscriptionLongPoll.cpp
  OpenHome/Net/Device/DviServer.cpp
  OpenHome/Net/Device/Upnp/DviServerUpnp.cpp
  OpenHome/Net/Device/Upnp/DviServerWebSocket.cpp
  OpenHome/Net/Device/Lpec/DviProtocolLpec.cpp
  OpenHome/Net/Device/Lpec/DviServerLpec.cpp
  OpenHome/Net/Device/DviService.cpp
  OpenHome/Net/Device/Upnp/DviSsdpNotifier.cpp
  OpenHome/Net/Device/DviStack.cpp
  OpenHome/Net/Device/DviSubscription.cpp
  OpenHome/Net/Device/DviPropertyUpdateCollection.cpp
  OpenHome/Net/Device/DvProvider.cpp
  OpenHome/Net/Bindings/C/Device/DvProviderC.cpp
  OpenHome/Net/Device/DvServerUpnp.cpp
  OpenHome/Net/Bindings/C/Device/DvServerUpnpC.cpp
  OpenHome/Net/ControlPoint/Upnp/DeviceXml.cpp
  OpenHome/Net/Error.cpp
  OpenHome/Net/ControlPoint/Upnp/EventUpnp.cpp
  OpenHome/Exception.cpp
  OpenHome/Fifo.cpp
  OpenHome/FileAnsi.cpp
  OpenHome/FileBrx.cpp
  OpenHome/FileStream.cpp
  OpenHome/Net/Globals.cpp
  OpenHome/Http.cpp
  # ${mDNSdir}/anonymous.c
  # ${mDNSdir}/CryptoAlg.c
  ${mDNSdir}/mDNSCore/DNSCommon.c
  ${mDNSdir}/mDNSCore/DNSDigest.c
  ${mDNSdir}/mDNSCore/mDNS.c
  # ${mDNSdir}/Clients/dnssdutil/nsec.c
  # ${mDNSdir}/nsec3.c
  ${mDNSdir}/mDNSCore/uDNS.c
  ${mDNSdir}/mDNSShared/dnssd_clientshim.c
  ${mDNSdir}/mDNSShared/dnssd_clientlib.c
  OpenHome/Net/Device/Bonjour/MdnsPlatform.cpp
  OpenHome/Net/Device/Bonjour/MdnsProvider.cpp
  OpenHome/md5.c
  OpenHome/NetworkAdapterList.cpp
  OpenHome/DnsChangeNotifier.cpp
  OpenHome/Network.cpp
  OpenHome/Parser.cpp
  OpenHome/Printer.cpp
  OpenHome/Net/ControlPoint/Upnp/ProtocolUpnp.cpp
  OpenHome/Queue.cpp
  OpenHome/Net/Service.cpp
  OpenHome/Net/Bindings/C/ServiceC.cpp
  OpenHome/sha1.c
  OpenHome/Net/Ssdp.cpp
  OpenHome/Net/SsdpDv.cpp
  OpenHome/Env.cpp
  OpenHome/Stream.cpp
  OpenHome/Net/Subscription.cpp
  OpenHome/Terminal.cpp
  OpenHome/Thread.cpp
  OpenHome/Timer.cpp
  OpenHome/TIpAddressUtils.cpp
  OpenHome/Uri.cpp
  OpenHome/Net/XmlParser.cpp
  OpenHome/Net/ControlPoint/Upnp/XmlFetcher.cpp
  OpenHome/Net/OhNet.cpp
  OpenHome/Net/OhNetCp.cpp
  OpenHome/Net/OhNetDv.cpp
  OpenHome/Net/OhNetCombined.cpp
  OpenHome/Net/Bindings/C/OhNetC.cpp
  OpenHome/Net/Bindings/C/OhNetCCp.cpp
  OpenHome/Net/Bindings/C/OhNetCDv.cpp
  OpenHome/Net/Bindings/C/OhNetCCombined.cpp
  Os/OsWrapper.cpp
  Os/${osdir}/Os.c
  Os/${osdir}/File.cpp
  Os/${osdir}/TerminalOs.cpp
  Os/${osdir}/SignalHandlers.cpp
  OpenHome/Shell/Shell.cpp
  OpenHome/Shell/InfoProvider.cpp
  OpenHome/Shell/ShellCommandDebug.cpp
)

set(src_proxies
  a
  b
  c
)

set(src_devices
  a
  b
  c
)

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

file(GLOB all_files_prec
  "OpenHome/Net/*/*.h"
  "OpenHome/Net/*/*.cpp"
  "OpenHome/Shell/*/*.h"
  "OpenHome/Shell/*/*.h"
  "OpenHome/*.cpp"
  "OpenHome/*.cpp"

  "Os/Posix/*.h"
  "Os/Posix/*.cpp"
  "Os/*.h"
  "Os/*.cpp"
)

file(GLOB test_files
  OpenHome/TestFramework/*.cpp
)

add_library(ohNetCore ${all_files})

# add_library(exception OpenHome/Exception.cpp)
# add_library(testFramework OpenHome/TestFramework/TestFramework.cpp)
add_library(testFramework ${test_files})

target_include_directories(
  ohNetCore PUBLIC $<INSTALL_INTERFACE:include>
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSShared"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSCore"
)
# target_include_directories(
#   exception PUBLIC $<INSTALL_INTERFACE:include>
#   "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
#   "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSCore")
target_include_directories(
  testFramework PUBLIC $<INSTALL_INTERFACE:include>
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/thirdparty/mDNSResponder-1310.80.1/mDNSCore")

target_compile_definitions(ohNetCore PUBLIC DEFINE_LITTLE_ENDIAN)
target_compile_definitions(ohNetCore PUBLIC _GNU_SOURCE)
# target_compile_definitions(exception PUBLIC DEFINE_LITTLE_ENDIAN)
target_compile_definitions(testFramework PUBLIC DEFINE_LITTLE_ENDIAN)

add_executable(bufferTest OpenHome/Tests/TestBuffer.cpp)
target_link_libraries(bufferTest PUBLIC ohNetCore testFramework)
