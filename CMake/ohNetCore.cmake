
set(OHNETCORE_SOURCES
  OpenHome/Ascii.cpp
  OpenHome/Net/Bindings/C/AsyncC.cpp
  OpenHome/Net/ControlPoint/AsyncPrivate.cpp
  OpenHome/Net/Device/Bonjour/Bonjour.cpp
  OpenHome/Buffer.cpp
  OpenHome/Converter.cpp
  OpenHome/Crc32.cpp
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
  # $(mDNSdir)/anonymous.c
  # $(mDNSdir)/CryptoAlg.c
  ${mDNSdir}/mDNSCore/DNSCommon.c
  ${mDNSdir}/mDNSCore/DNSDigest.c
  ${mDNSdir}/mDNSCore/mDNS.c
  # $(mDNSdir)/nsec.c
  # $(mDNSdir)/nsec3.c
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
  Os/${platform}/File.cpp
  Os/${platform}/TerminalOs.cpp
  Os/${platform}/Os.c
  Os/${platform}/SignalHandlers.cpp

  OpenHome/Shell/Shell.cpp
  OpenHome/Shell/InfoProvider.cpp
  OpenHome/Shell/ShellCommandDebug.cpp
)

add_library(ohNetCore STATIC ${OHNETCORE_SOURCES})
add_dependencies(ohNetCore IncludeCopy)
target_include_directories(ohNetCore SYSTEM PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Temp)
target_include_directories(
  ohNetCore PUBLIC $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_link_libraries(ohNetCore PUBLIC b64::b64)
target_compile_definitions(ohNetCore PUBLIC ${ENDIANNESS})

