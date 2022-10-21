#include <OpenHome/Net/Device/DvServerUpnp.h>
#include <OpenHome/Net/Device/DvDevice.h>
#include <OpenHome/Net/Device/DviDevice.h>
#include <OpenHome/Net/Device/Upnp/DviServerUpnp.h>

using namespace OpenHome;
using namespace OpenHome::Net;

DvServerUpnp::DvServerUpnp(DvStack& aDvStack)
{
    iServer = new DviServerUpnp(aDvStack);
}

DvServerUpnp::~DvServerUpnp()
{
    delete iServer;
}

void DvServerUpnp::AddDevice(DvDevice& aDevice)
{
    aDevice.Device().SetCustomData("Upnp", "Server", iServer);
}
