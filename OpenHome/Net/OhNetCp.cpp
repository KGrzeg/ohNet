#include <OpenHome/Net/OhNet.h>
#include <OpenHome/Net/ControlPoint/CpiStack.h>
#include <OpenHome/Net/Globals.h>
#include <OpenHome/NetworkAdapterList.h>

using namespace OpenHome;
using namespace OpenHome::Net;

CpStack* Library::StartCp(TIpAddress aSubnet)
{
    iEnv->NetworkAdapterList().SetCurrentSubnet(aSubnet);
    return new CpStack(*iEnv);
}

void UpnpLibrary::StartCp(TIpAddress aSubnet)
{ // static
    gEnv->NetworkAdapterList().SetCurrentSubnet(aSubnet);
    gCpStack = new CpStack(*gEnv);
}
