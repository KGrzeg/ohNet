#include <OpenHome/Net/ControlPoint/CpDeviceDv.h>
#include <OpenHome/Net/ControlPoint/CpiDeviceDv.h>
#include <OpenHome/Net/Device/DvDevice.h>
#include <OpenHome/Net/Device/DviDevice.h>
#include <OpenHome/Net/Globals.h>

using namespace OpenHome;
using namespace OpenHome::Net;

CpDeviceDvCpp* CpDeviceDvCpp::New(DvDeviceStd& aDevice)
{
    CpiDeviceDv* device = new CpiDeviceDv(*gCpStack, aDevice.Device());
    CpDeviceDvCpp* self = new CpDeviceDvCpp(*device);
    self->Device().RemoveRef();
    return self;
}

CpDeviceDvCpp::~CpDeviceDvCpp()
{
}

CpDeviceDvCpp::CpDeviceDvCpp(CpiDeviceDv& aDevice)
    : CpDeviceCpp(aDevice.Device())
{
}
