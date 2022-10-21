#include <OpenHome/Types.h>
#include <OpenHome/TestFramework.h>
#include <OpenHome/OptionParser.h>
#include <OpenHome/Net/OhNet.h>

#include <vector>

using namespace OpenHome;
using namespace OpenHome::Net;

extern void TestDeviceList(CpStack& aCpStack, const std::vector<Brn>& aArgs);

void OpenHome::TestFramework::Runner::Main(TInt aArgc, TChar* aArgv[], Net::InitialisationParams* aInitParams)
{
    std::vector<Brn> args = OptionParser::ConvertArgs(aArgc, aArgv);
    Library* lib = new Library(aInitParams);
    std::vector<NetworkAdapter*>* subnetList = lib->CreateSubnetList();
    TIpAddress subnet = (*subnetList)[0]->Subnet();
    Library::DestroySubnetList(subnetList);
    CpStack* cpStack = lib->StartCp(subnet);

    TestDeviceList(*cpStack, args);

    delete lib;
}
