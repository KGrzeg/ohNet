#include <OpenHome/TestFramework/TestFramework.h>
#include <OpenHome/Net/OhNet.h>

using namespace OpenHome;

extern void TestTimer(Environment& aEnv);

void OpenHome::TestFramework::Runner::Main(TInt /*aArgc*/, TChar* /*aArgv*/[], Net::InitialisationParams* aInitParams)
{
    Net::Library* lib = new Net::Library(aInitParams);
    TestTimer(lib->Env());
    delete lib;
}
