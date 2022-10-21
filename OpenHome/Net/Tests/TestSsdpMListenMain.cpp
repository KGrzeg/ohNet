#include <OpenHome/TestFramework/TestFramework.h>
#include <OpenHome/TestFramework/OptionParser.h>
#include <OpenHome/Net/OhNet.h>
#include <OpenHome/Buffer.h>

#include <vector>

using namespace OpenHome;

extern void TestSsdpMListen(Environment& aStack, const std::vector<Brn>& aArgs);

void OpenHome::TestFramework::Runner::Main(TInt aArgc, TChar* aArgv[], Net::InitialisationParams* aInitParams)
{
    Net::Library* lib = new Net::Library(aInitParams);
    std::vector<Brn> args = OptionParser::ConvertArgs(aArgc, aArgv);
    TestSsdpMListen(lib->Env(), args);
    delete lib;
}
