#include <OpenHome/Net/Device/DviStack.h>
#include <OpenHome/Env.h>
#include <OpenHome/Net/Device/Upnp/DviServerUpnp.h>
#include <OpenHome/Net/Device/DviDevice.h>
#include <OpenHome/Net/Device/DviSubscription.h>
#include <OpenHome/Printer.h>
#include <OpenHome/Net/Device/Upnp/DviServerWebSocket.h>
#include <OpenHome/Net/Device/Bonjour/Bonjour.h>
#include <OpenHome/Net/Device/DviPropertyUpdateCollection.h>
#include <OpenHome/Net/Device/Upnp/DviProtocolLpec.h>

using namespace OpenHome;
using namespace OpenHome::Net;

// DvStack

DvStack::DvStack(OpenHome::Environment& aEnv)
    : iEnv(aEnv)
    , iLock("DStk")
    , iBootId(1)
    , iNextBootId(2)
    , iControlPoint(OpenHome::Brx::Empty())
{
    iEnv.SetDvStack(this);
    iSsdpNotifierManager = new DviSsdpNotifierManager(*this);
    iPropertyUpdateCollection = new DviPropertyUpdateCollection(*this);
    InitialisationParams* initParams = iEnv.InitParams();
    TUint port = initParams->DvUpnpServerPort();
    iDviDeviceMap = new DviDeviceMap;
    iDviServerUpnp = new DviServerUpnp(*this, port);
    const TUint pubThPriority = initParams->DvPublisherThreadPriority();
    iSubscriptionManager = new DviSubscriptionManager(*this, pubThPriority);
    iDviServerWebSocket = new DviServerWebSocket(*this);
    if (initParams->DvNumLpecThreads() > 0) {
        port = initParams->DvLpecServerPort();
        AddProtocolFactory(new DviProtocolFactoryLpec(*this, port));
    }
}

DvStack::~DvStack()
{
    for (TUint i=0; i<iProtocolFactories.size(); i++) {
        delete iProtocolFactories[i];
    }
    delete iDviServerWebSocket;
    delete iDviServerUpnp;
    delete iDviDeviceMap;
    delete iSubscriptionManager;
    delete iPropertyUpdateCollection;
    delete iSsdpNotifierManager;
    ASSERT(iControlPointObservers.size() == 0);
}

void DvStack::Start()
{
    iDviServerUpnp->Start();
    iDviServerWebSocket->Start();
    for (TUint i=0; i<iProtocolFactories.size(); i++) {
        iProtocolFactories[i]->Start();
    }
}

TUint DvStack::BootId()
{
    AutoMutex _(iLock);
    TUint id = iBootId;
    return id;
}

TUint DvStack::NextBootId()
{
    AutoMutex _(iLock);
    TUint id = iNextBootId;
    return id;
}

void DvStack::UpdateBootId()
{
    AutoMutex _(iLock);
    iBootId = iNextBootId;
    iNextBootId++;
}

void DvStack::AddControlPointObserver(IControlPointObserver& aObserver)
{
    iControlPointObservers.push_back(&aObserver);
}

void DvStack::RemoveControlPointObserver(IControlPointObserver& aObserver)
{
    for (TUint i=0; i<iControlPointObservers.size(); i++) {
        if (iControlPointObservers[i] == &aObserver) {
            iControlPointObservers.erase(iControlPointObservers.begin() + i);
            break;
        }
    }
}

void DvStack::NotifyControlPointUsed(const OpenHome::Brx& aControlPoint)
{
    iControlPoint.Replace(aControlPoint);
    for (TUint i=0; i<iControlPointObservers.size(); i++) {
        iControlPointObservers[i]->NotifyControlPoint(aControlPoint);
    }
}

DviServerUpnp& DvStack::ServerUpnp()
{
    return *iDviServerUpnp;
}

DviDeviceMap& DvStack::DeviceMap()
{
    return *iDviDeviceMap;
}

DviSubscriptionManager& DvStack::SubscriptionManager()
{
    return *iSubscriptionManager;
}

DviPropertyUpdateCollection& DvStack::PropertyUpdateCollection()
{
    return *iPropertyUpdateCollection;
}

DviSsdpNotifierManager& DvStack::SsdpNotifierManager()
{
    return *iSsdpNotifierManager;
}

void DvStack::AddProtocolFactory(IDvProtocolFactory* aProtocolFactory)
{
    iProtocolFactories.push_back(aProtocolFactory);
}

std::vector<IDvProtocolFactory*>& DvStack::ProtocolFactories()
{
    return iProtocolFactories;
}
