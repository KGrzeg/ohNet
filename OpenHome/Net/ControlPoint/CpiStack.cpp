#include <OpenHome/Net/ControlPoint/CpiStack.h>
#include <OpenHome/Env.h>
#include <OpenHome/Net/ControlPoint/CpiService.h>
#include <OpenHome/Net/XmlFetcher.h>
#include <OpenHome/Net/ControlPoint/CpiSubscription.h>
#include <OpenHome/Net/ControlPoint/CpiDevice.h>
#include <OpenHome/Printer.h>

using namespace OpenHome;
using namespace OpenHome::Net;

// CpStack

CpStack::CpStack(OpenHome::Environment& aStack)
    : iEnv(aStack)
{
    iEnv.SetCpStack(this);
    iInvocationManager = new OpenHome::Net::InvocationManager(*this);
    iXmlFetchManager = new OpenHome::Net::XmlFetchManager(*this);
    iSubscriptionManager = new CpiSubscriptionManager(*this);
    iDeviceListUpdater = new CpiDeviceListUpdater();
}

CpStack::~CpStack()
{
    delete iDeviceListUpdater;
    delete iSubscriptionManager;
    delete iXmlFetchManager;
    delete iInvocationManager;
}

InvocationManager& CpStack::InvocationManager()
{
    return *iInvocationManager;
}

OpenHome::Net::XmlFetchManager& CpStack::XmlFetchManager()
{
    return *iXmlFetchManager;
}

CpiSubscriptionManager& CpStack::SubscriptionManager()
{
    return *iSubscriptionManager;
}

CpiDeviceListUpdater& CpStack::DeviceListUpdater()
{
    return *iDeviceListUpdater;
}
