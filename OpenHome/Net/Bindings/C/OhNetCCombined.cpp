#include <OpenHome/Net/Bindings/C/OhNet.h>
#include <OpenHome/Net/OhNet.h>
#include <OpenHome/Network.h>
#include <OpenHome/Net/Globals.h>
#include <OpenHome/Net/Device/DviStack.h>

using namespace OpenHome;
using namespace OpenHome::Net;

EOhNetLibraryInitError STDCALL OhNetLibraryStartCombined(uint32_t aSubnetV4)
{
    TIpAddress subnet;
    subnet.iFamily = kFamilyV4;
    subnet.iV4 = aSubnetV4;
    try {
        UpnpLibrary::StartCombined(subnet);
        gDvStack->Start();
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
