#include <OpenHome/TestFramework.h>
#include <OpenHome/Types.h>
#include <OpenHome/Net/DvDevice.h>
#include <OpenHome/Net/OhNet.h>
#include <OpenHome/Net/CpDeviceDv.h>
#include "../../Device/Tests/TestBasicDv.h"
#include "TestBasicCp.h"
#include <OpenHome/Net/Private/Globals.h>

#include <string>
#include <vector>

using namespace OpenHome;
using namespace OpenHome::Net;
using namespace OpenHome::TestFramework;


void OpenHome::TestFramework::Runner::Main(TInt /*aArgc*/, TChar* /*aArgv*/[], InitialisationParams* aInitParams)
{
    aInitParams->SetUseLoopbackNetworkAdapter();
    UpnpLibrary::Initialise(aInitParams);
    std::vector<NetworkAdapter*>* subnetList = UpnpLibrary::CreateSubnetList();
    TIpAddress subnet = (*subnetList)[0]->Subnet();
    UpnpLibrary::DestroySubnetList(subnetList);
    UpnpLibrary::StartCombined(subnet);

    Print("TestCpDeviceDvStd - starting\n");

    DeviceBasic* device = new DeviceBasic(*gEnv, DeviceBasic::eProtocolNone);
    CpDeviceDvCpp* cpDevice = CpDeviceDvCpp::New(device->Device());
    TestBasicCp::TestActions(*cpDevice);
    TestBasicCp::TestSubscriptions(*cpDevice);
    cpDevice->RemoveRef();
    delete device;

    Print("TestCpDeviceDvStd - completed\n");
    UpnpLibrary::Close();
}
