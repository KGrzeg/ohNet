#include <OpenHome/Net/OhNet.h>
#include <OpenHome/Net/Device/DviStack.h>
#include <OpenHome/Net/Globals.h>

using namespace OpenHome;
using namespace OpenHome::Net;

DvStack* Library::StartDv()
{
    return new DvStack(*iEnv);
}

void UpnpLibrary::StartDv()
{ // static
    gDvStack = new DvStack(*gEnv);
}
