#include <OpenHome/Types.h>
#include <OpenHome/TestFramework/TestFramework.h>
#include <OpenHome/TestFramework/OptionParser.h>
#include <OpenHome/Net/OhNet.h>
#include <OpenHome/Net/Device/DviStack.h>

#include <vector>

using namespace OpenHome;
using namespace OpenHome::Net;

extern void TestDviDiscovery(DvStack& aDvStack);

void OpenHome::TestFramework::Runner::Main(TInt aArgc, TChar* aArgv[], Net::InitialisationParams* aInitParams)
{
    OptionParser parser;
    OptionBool loopback("-l", "--loopback", "Use the loopback adapter only");
    parser.AddOption(&loopback);
    if (!parser.Parse(aArgc, aArgv) || parser.HelpDisplayed()) {
        return;
    }
    if (loopback.Value()) {
        aInitParams->SetUseLoopbackNetworkAdapter();
    }
    aInitParams->SetDvUpnpServerPort(0);
    Library* lib = new Library(aInitParams);
    std::vector<NetworkAdapter*>* subnetList = lib->CreateSubnetList();
    TIpAddress subnet = (*subnetList)[0]->Subnet();
    Library::DestroySubnetList(subnetList);
    lib->SetCurrentSubnet(subnet);
    DvStack* dvStack = lib->StartDv();
    dvStack->Start();

    TestDviDiscovery(*dvStack);

    delete lib;
}
