#include <OpenHome/TestFramework/TestFramework.h>
#include <OpenHome/Net/OhNet.h>

extern void TestQueue();

void OpenHome::TestFramework::Runner::Main(TInt /*aArgc*/, TChar* /*aArgv*/[], Net::InitialisationParams* aInitParams)
{
    Environment* env = Environment::Create(aInitParams->LogOutput(),
                                           aInitParams->TimerManagerPriority(),
                                           aInitParams->SchedulingPolicy(),
                                           aInitParams->LoopbackNetworkAdapter());
    TestQueue();
    delete env;
    delete aInitParams;
}
