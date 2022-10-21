#include <OpenHome/Net/Bindings/C/CpDeviceDv.h>
#include "../Device/DviDeviceC.h"
#include <OpenHome/Net/Device/DvDevice.h>
#include <OpenHome/Net/CpiDeviceDv.h>
#include <OpenHome/Net/Globals.h>

using namespace OpenHome;
using namespace OpenHome::Net;

CpDeviceC STDCALL CpDeviceDvCreate(DvDeviceC aDevice)
{
    DvDevice* dv = DviDeviceC::DeviceFromHandle(aDevice);
    ASSERT(dv != NULL);
    CpiDeviceDv* cp = new CpiDeviceDv(*gCpStack, dv->Device());
    ASSERT(cp != NULL);
    return (CpDeviceC)(&cp->Device());
}
