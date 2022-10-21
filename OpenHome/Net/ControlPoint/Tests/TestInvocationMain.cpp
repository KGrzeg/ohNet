#include <OpenHome/Types.h>
#include <OpenHome/TestFramework/TestFramework.h>
#include <OpenHome/TestFramework/OptionParser.h>
#include <OpenHome/Net/OhNet.h>

#include <vector>

using namespace OpenHome;
using namespace OpenHome::Net;

extern void TestInvocation(CpStack& aCpStack);

void OpenHome::TestFramework::Runner::Main(TInt /*aArgc*/, TChar* /*aArgv*/[], Net::InitialisationParams* aInitParams)
{
    Library* lib = new Library(aInitParams);
    std::vector<NetworkAdapter*>* subnetList = lib->CreateSubnetList();
    TIpAddress subnet = (*subnetList)[0]->Subnet();
    Library::DestroySubnetList(subnetList);
    CpStack* cpStack = lib->StartCp(subnet);

    TestInvocation(*cpStack);

    delete lib;
}
