// Mimics Kazoo's use of device lists

#include <OpenHome/TestFramework/TestFramework.h>
#include <OpenHome/TestFramework/OptionParser.h>
#include <OpenHome/Types.h>
#include <OpenHome/Net/Discovery.h>
#include <OpenHome/Thread.h>
#include <OpenHome/Timer.h>
#include <Os/OsWrapper.h>
#include <OpenHome/Net/ControlPoint/CpDeviceUpnp.h>
#include <OpenHome/Net/ControlPoint/FunctorCpDevice.h>
#include <OpenHome/Env.h>
#include <OpenHome/Net/ControlPoint/CpiStack.h>
#include <Os/OsWrapper.h>
#include <OpenHome/Net/Globals.h>

using namespace OpenHome;
using namespace OpenHome::Net;
using namespace OpenHome::TestFramework;

class DeviceListLoggerBase
{
protected:
    DeviceListLoggerBase(const TChar* aType);
    void PrintDeviceInfo(const char* aPrologue, const CpDevice& aDevice);
private:
    Mutex iLock;
    const TChar* iType;
};

class DeviceListLogger : public DeviceListLoggerBase
{
public:
    DeviceListLogger(const TChar* aType);
    void Added(CpDevice& aDevice);
    void Removed(CpDevice& aDevice);
};


DeviceListLoggerBase::DeviceListLoggerBase(const TChar* aType)
    : iLock("DLLM")
    , iType(aType)
{
}

void DeviceListLoggerBase::PrintDeviceInfo(const char* aPrologue, const CpDevice& aDevice)
{
    iLock.Wait();
    Print("%s %s: ", aPrologue, iType);
    Brh val;
    aDevice.GetAttribute("Upnp.FriendlyName", val);
    Print(val);
    Print(", ");
    aDevice.GetAttribute("Upnp.Location", val);
    Print(val);
    Print("\n");
    iLock.Signal();
}


DeviceListLogger::DeviceListLogger(const TChar* aType)
    : DeviceListLoggerBase(aType)
{
}


void DeviceListLogger::Added(CpDevice& aDevice)
{
    PrintDeviceInfo("+", aDevice);
}

void DeviceListLogger::Removed(CpDevice& aDevice)
{
    PrintDeviceInfo("-", aDevice);
}


void OpenHome::TestFramework::Runner::Main(TInt aArgc, TChar* aArgv[], Net::InitialisationParams* aInitParams)
{
    OptionParser parser;
    OptionUint adapter("-a", "--adapter", 0, "Index of network adapter to use");
    parser.AddOption(&adapter);
    std::vector<Brn> args = OptionParser::ConvertArgs(aArgc, aArgv);
    if (!parser.Parse(args, true) || parser.HelpDisplayed()) {
        return;
    }

    Library* lib = new Library(aInitParams);
    std::vector<NetworkAdapter*>* subnetList = lib->CreateSubnetList();
    TIpAddress subnet = (*subnetList)[adapter.Value()]->Subnet();
    Library::DestroySubnetList(subnetList);
    CpStack* cpStack = lib->StartCp(subnet);

//    Debug::SetLevel(Debug::kSsdpMulticast | Debug::kSsdpUnicast);
    DeviceListLogger loggerCd("ContentDirectory");
    DeviceListLogger loggerProduct("Product");
    DeviceListLogger loggerSender("Sender");
    FunctorCpDevice added = MakeFunctorCpDevice(loggerCd, &DeviceListLogger::Added);
    FunctorCpDevice removed = MakeFunctorCpDevice(loggerCd, &DeviceListLogger::Removed);
    std::vector<CpDeviceList*> deviceLists;
    deviceLists.push_back(new CpDeviceListUpnpServiceType(*cpStack, Brn("upnp.org"), Brn("ContentDirectory"), 1, added, removed));

    added = MakeFunctorCpDevice(loggerProduct, &DeviceListLogger::Added);
    removed = MakeFunctorCpDevice(loggerProduct, &DeviceListLogger::Removed);
    deviceLists.push_back(new CpDeviceListUpnpServiceType(*cpStack, Brn("av.openhome.org"), Brn("Product"), 1, added, removed));

    added = MakeFunctorCpDevice(loggerSender, &DeviceListLogger::Added);
    removed = MakeFunctorCpDevice(loggerSender, &DeviceListLogger::Removed);
    deviceLists.push_back(new CpDeviceListUpnpServiceType(*cpStack, Brn("av.openhome.org"), Brn("Sender"), 1, added, removed));

    for (;;){
        int ch = getchar();
        if (ch == 'q') {
            break;
        }
        if (ch == 'r') {
            Print("=== %u Refresh...\n", Os::TimeInMs(gEnv->OsCtx()));
            for (auto it=deviceLists.begin(); it!=deviceLists.end(); ++it) {
                (*it)->Refresh();
            }
        }
        if (ch == 'l') {
            for (;;) {
                Print("=== %u Refresh...\n", Os::TimeInMs(gEnv->OsCtx()));
                for (auto it=deviceLists.begin(); it!=deviceLists.end(); ++it) {
                    (*it)->Refresh();
                }
                const TUint sleepMs = cpStack->Env().Random(4000, 3000);
                Print("=== %u Sleep for %ums\n", Os::TimeInMs(gEnv->OsCtx()), sleepMs);
                Thread::Sleep(sleepMs);
            }
        }
    }

    for (auto it=deviceLists.begin(); it!=deviceLists.end(); ++it) {
        delete *it;
    }
}
