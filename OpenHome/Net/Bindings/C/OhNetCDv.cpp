#include <OpenHome/Net/Bindings/C/OhNet.h>
#include <OpenHome/Net/OhNet.h>
#include <OpenHome/Network.h>
#include <OpenHome/Net/Private/Globals.h>
#include <OpenHome/Net/Private/DviStack.h>

using namespace OpenHome;
using namespace OpenHome::Net;

EOhNetLibraryInitError STDCALL OhNetLibraryStartDv()
{
    try {
        UpnpLibrary::StartDv();
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
