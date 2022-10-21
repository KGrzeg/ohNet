#include <OpenHome/Net/Bindings/C/OhNet.h>
#include <OpenHome/Net/OhNet.h>
#include <OpenHome/Network.h>

using namespace OpenHome;
using namespace OpenHome::Net;

EOhNetLibraryInitError STDCALL OhNetLibraryStartCp(uint32_t aSubnetV4)
{
    TIpAddress subnet;
    subnet.iFamily = kFamilyV4;
    subnet.iV4 = aSubnetV4;
    try {
        UpnpLibrary::StartCp(subnet);
    }
    catch (NetworkAddressInUse& ) {
        return eOhNetInitErrorNetworkAddressInUse;
    }
    catch (std::bad_alloc& ) {
        return eOhNetInitErrorNoMemory;
    }
    catch(...) {
        return eOhNetInitErrorGeneral;
    }
    return eOhNetInitErrorNone;
}
