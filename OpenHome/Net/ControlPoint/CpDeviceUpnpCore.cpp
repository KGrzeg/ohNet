#include <OpenHome/Net/CpDeviceUpnp.h>
#include <OpenHome/Types.h>
#include <OpenHome/Buffer.h>
#include <OpenHome/Net/CpiDeviceUpnp.h>
#include <OpenHome/Net/ControlPoint/FunctorCpDevice.h>
#include <OpenHome/Net/ControlPoint/FunctorCpiDevice.h>

using namespace OpenHome;
using namespace OpenHome::Net;


// CpDeviceListUpnp

void CpDeviceListUpnp::TryAdd(const Brx& aLocation)
{
    static_cast<CpiDeviceListUpnp*>(iList)->TryAdd(aLocation);
}

CpDeviceListUpnp::CpDeviceListUpnp(FunctorCpDevice aAdded, FunctorCpDevice aRemoved)
    : CpDeviceList(aAdded, aRemoved)
{}


// CpDeviceListUpnpAll

CpDeviceListUpnpAll::CpDeviceListUpnpAll(CpStack& aCpStack, FunctorCpDevice aAdded, FunctorCpDevice aRemoved)
    : CpDeviceListUpnp(aAdded, aRemoved)
{
    FunctorCpiDevice added, removed;
    GetAddedFunctor(added);
    GetRemovedFunctor(removed);
    iList = new CpiDeviceListUpnpAll(aCpStack, added, removed);
    iList->Start();
}


// CpDeviceListUpnpRoot

CpDeviceListUpnpRoot::CpDeviceListUpnpRoot(CpStack& aCpStack, FunctorCpDevice aAdded, FunctorCpDevice aRemoved)
    : CpDeviceListUpnp(aAdded, aRemoved)
{
    FunctorCpiDevice added, removed;
    GetAddedFunctor(added);
    GetRemovedFunctor(removed);
    iList = new CpiDeviceListUpnpRoot(aCpStack, added, removed);
    iList->Start();
}


// CpDeviceListUpnpUuid

CpDeviceListUpnpUuid::CpDeviceListUpnpUuid(CpStack& aCpStack, const Brx& aUuid, FunctorCpDevice aAdded, FunctorCpDevice aRemoved)
    : CpDeviceListUpnp(aAdded, aRemoved)
{
    FunctorCpiDevice added, removed;
    GetAddedFunctor(added);
    GetRemovedFunctor(removed);
    iList = new CpiDeviceListUpnpUuid(aCpStack, aUuid, added, removed);
    iList->Start();
}


// CpDeviceListUpnpDeviceType

CpDeviceListUpnpDeviceType::CpDeviceListUpnpDeviceType(CpStack& aCpStack, const Brx& aDomainName, const Brx& aDeviceType,
                                                       TUint aVersion, FunctorCpDevice aAdded, FunctorCpDevice aRemoved)
    : CpDeviceListUpnp(aAdded, aRemoved)
{
    FunctorCpiDevice added, removed;
    GetAddedFunctor(added);
    GetRemovedFunctor(removed);
    iList = new CpiDeviceListUpnpDeviceType(aCpStack, aDomainName, aDeviceType, aVersion, added, removed);
    iList->Start();
}


// CpDeviceListUpnpServiceType

CpDeviceListUpnpServiceType::CpDeviceListUpnpServiceType(CpStack& aCpStack, const Brx& aDomainName, const Brx& aServiceType,
                                                         TUint aVersion, FunctorCpDevice aAdded, FunctorCpDevice aRemoved)
    : CpDeviceListUpnp(aAdded, aRemoved)
{
    FunctorCpiDevice added, removed;
    GetAddedFunctor(added);
    GetRemovedFunctor(removed);
    iList = new CpiDeviceListUpnpServiceType(aCpStack, aDomainName, aServiceType, aVersion, added, removed);
    iList->Start();

}
